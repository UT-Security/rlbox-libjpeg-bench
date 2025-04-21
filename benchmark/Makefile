NOTPARALLEL:

.DEFAULT_GOAL := build

SHELL := /bin/bash

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIR := $(dir $(MKFILE_PATH))
REPO_PATH=$(MKFILE_DIR)/..
PARALLEL_COUNT=$(shell nproc)

WASI_SDK_PATH=$(REPO_PATH)/benchmark/wasi-sdk
RLBOX_PATH=$(REPO_PATH)/benchmark/rlbox
RLBOX_WASM2C_PATH=$(REPO_PATH)/benchmark/rlbox_wasm2c_sandbox
WASM2C_SRC_PATH=$(REPO_PATH)/benchmark/wabt

CC=clang
CXX=clang++ -std=c++17
WASI_CC=$(WASI_SDK_PATH)/bin/clang --sysroot $(WASI_SDK_PATH)/share/wasi-sysroot/
WASI_CXX=$(WASI_SDK_PATH)/bin/clang++ --sysroot $(WASI_SDK_PATH)/share/wasi-sysroot/ -std=c++17
WASI_AR=$(WASI_SDK_PATH)/bin/ar
WASI_LD=$(WASI_SDK_PATH)/bin/wasm-ld
WASI_CFLAGS=-I $(MKFILE_DIR)/wasmcompat
WASI_LDFLAGS=-Wl,--export-all -Wl,--stack-first -Wl,-z,stack-size=1048576 -Wl,--no-entry -Wl,--import-memory -Wl,--import-table

WASM2C_RUNTIME=$(WASM2C_SRC_PATH)/wasm2c/wasm-rt-impl.c $(WASM2C_SRC_PATH)/wasm2c/wasm-rt-mem-impl.c $(RLBOX_WASM2C_PATH)/src/wasm2c_rt_mem.c $(RLBOX_WASM2C_PATH)/src/wasm2c_rt_minwasi.c

ifdef DEBUG
BUILD_TYPE_DIR=debug
BUILD_TYPE_CONFIG=Debug
BUILD_TYPE_FLAGS=-O0 -g -DWASM_RT_NOINLINE
BUILD_BINARY_FLAGS=-fsanitize=address
else
BUILD_TYPE_DIR=release
BUILD_TYPE_CONFIG=Release
BUILD_TYPE_FLAGS=-O3
BUILD_BINARY_FLAGS=
endif

LIBJPEG_INCLUDES=-I$(REPO_PATH)/ -I$(REPO_PATH)/build_nosimd_$(BUILD_TYPE_DIR)
LIBJPEG_NOSIMD_INCLUDES=-I$(REPO_PATH)/ -I$(REPO_PATH)/build_nosimd_$(BUILD_TYPE_DIR)
LIBJPEG_NOSIMD_WASM_INCLUDES=-I$(REPO_PATH)/ -I$(REPO_PATH)/build_nosimd_wasm_$(BUILD_TYPE_DIR)
LIBJPEG_LINK=$(REPO_PATH)/build_nosimd_$(BUILD_TYPE_DIR)/libjpeg.a
LIBJPEG_NOSIMD_LINK=$(REPO_PATH)/build_nosimd_$(BUILD_TYPE_DIR)/libjpeg.a
LIBJPEG_NOSIMD_WASM_LINK=-Wl,--whole-archive $(REPO_PATH)/build_nosimd_wasm_$(BUILD_TYPE_DIR)/libjpeg.a -Wl,--no-whole-archive
RLBOX_INCLUDES=-I $(RLBOX_PATH)/code/include -I $(RLBOX_WASM2C_PATH)/include -DRLBOX_WASM2C_MODULE_NAME=libjpeg
RLBOX_LINK=-lrt
WASM2C_BUILD_INCLUDES=-I $(WASM2C_SRC_PATH)/wasm2c -I$(REPO_PATH)/build_nosimd_wasm_$(BUILD_TYPE_DIR)/ -DWASM_RT_SKIP_SIGNAL_RECOVERY=1 -DWASM_RT_USE_STACK_DEPTH_COUNT=0 -DWASM_RT_USE_MMAP=1 -DWASM_RT_MEMCHECK_GUARD_PAGES=1

################Deps#####################

test_out.jpeg: test.jpeg ../build_nosimd_$(BUILD_TYPE_DIR)/libjpeg.a
	$(CC) -O0 -g generate_test_out.c -o ./generate_test_out $(LIBJPEG_INCLUDES) $(LIBJPEG_LINK)
	./generate_test_out test.jpeg test_out.jpeg

test_bytes.h: test.jpeg test_out.jpeg
	if [ ! -e "test_bytes.h" ]; then \
		echo "unsigned char inputData[] = {" > tmp_$@; \
		hexdump -ve '1/1 "0x%.2x, "' test.jpeg >> tmp_$@; \
		echo "0x00 };" >> tmp_$@; \
		echo "unsigned char outputData[] = {" >> tmp_$@; \
		hexdump -ve '1/1 "0x%.2x, "' test_out.jpeg >> tmp_$@; \
		echo "0x00 };" >> tmp_$@; \
		mv tmp_$@ $@; \
	fi

wasi-sdk-21.0-linux.tar.gz:
	wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-21/wasi-sdk-21.0-linux.tar.gz

wasi-sdk: wasi-sdk-21.0-linux.tar.gz
	mkdir -p $@
	tar -zxf $< -C $@ --strip-components 1

wabt:
	git clone --recursive https://github.com/WebAssembly/wabt/

rlbox:
	git clone --recursive https://github.com/PLSysSec/rlbox

rlbox_wasm2c_sandbox:
	git clone --recursive https://github.com/PLSysSec/rlbox_wasm2c_sandbox

$(WASM2C_SRC_PATH)/build_$(BUILD_TYPE_DIR)/wasm2c: $(WASM2C_SRC_PATH)/src/template/wasm2c.declarations.c $(WASM2C_SRC_PATH)/wasm2c/wasm-rt-impl.c $(WASM2C_SRC_PATH)/wasm2c/wasm-rt.h
	cmake -S $(WASM2C_SRC_PATH) -B $(WASM2C_SRC_PATH)/build_$(BUILD_TYPE_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE_CONFIG)
	cd $(WASM2C_SRC_PATH)/build_$(BUILD_TYPE_DIR) && make -j$(PARALLEL_COUNT)

#####################################


../build_nosimd_$(BUILD_TYPE_DIR)/libjpeg.a:
	cmake -S ../ -B ../build_nosimd_$(BUILD_TYPE_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE_CONFIG) -DWITH_SIMD=0
	cd ../build_nosimd_$(BUILD_TYPE_DIR) && make -j$(PARALLEL_COUNT)

../build_nosimd_wasm_$(BUILD_TYPE_DIR)/libjpeg.wasm.c: $(WASM2C_SRC_PATH)/build_$(BUILD_TYPE_DIR)/wasm2c
	CC="$(WASI_CC)" CFLAGS="$(WASI_CFLAGS)" LDFLAGS="$(WASI_LDFLAGS)" \
		CXX="$(WASI_CXX)" CXXFLAGS="$(WASI_CFLAGS)" LDFLAGS="$(WASI_LDFLAGS)" \
		AR="$(WASI_AR)" LD="$(WASI_LD)" \
		cmake -S ../ -B ../build_nosimd_wasm_$(BUILD_TYPE_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE_CONFIG) -DENABLE_SHARED=0 -DWITH_SIMD=0
	cd ../build_nosimd_wasm_$(BUILD_TYPE_DIR) && make -j$(PARALLEL_COUNT)
	$(WASI_CC) $(BUILD_TYPE_FLAGS) $(WASI_CFLAGS) $(WASI_LDFLAGS) -o ../build_nosimd_wasm_$(BUILD_TYPE_DIR)/libjpeg.wasm $(RLBOX_WASM2C_PATH)/c_src/wasm2c_sandbox_wrapper.c $(LIBJPEG_NOSIMD_WASM_LINK)
	$(WASM2C_SRC_PATH)/build_$(BUILD_TYPE_DIR)/wasm2c ../build_nosimd_wasm_$(BUILD_TYPE_DIR)/libjpeg.wasm -o ../build_nosimd_wasm_$(BUILD_TYPE_DIR)/libjpeg.wasm.c


#####################################

../build_nosimd_$(BUILD_TYPE_DIR)/image_change_quality: image_change_quality.c test_bytes.h ../build_nosimd_$(BUILD_TYPE_DIR)/libjpeg.a
	$(CC) $(BUILD_TYPE_FLAGS) $(BUILD_BINARY_FLAGS) -o $@ $< $(LIBJPEG_NOSIMD_INCLUDES) $(LIBJPEG_NOSIMD_LINK)

#####################################

../build_nosimd_$(BUILD_TYPE_DIR)/image_change_quality_rlbox_noop: image_change_quality_rlbox.cpp test_bytes.h ../build_nosimd_$(BUILD_TYPE_DIR)/libjpeg.a
	$(CXX) $(BUILD_TYPE_FLAGS) $(BUILD_BINARY_FLAGS) -o $@ $< $(LIBJPEG_NOSIMD_INCLUDES) $(RLBOX_INCLUDES) $(LIBJPEG_NOSIMD_LINK) $(RLBOX_LINK)

../build_nosimd_wasm_$(BUILD_TYPE_DIR)/image_change_quality_rlbox_wasm2c: image_change_quality_rlbox.cpp test_bytes.h ../build_nosimd_wasm_$(BUILD_TYPE_DIR)/libjpeg.wasm.c
	rm -rf ../build_nosimd_wasm_$(BUILD_TYPE_DIR)/objs
	mkdir -p ../build_nosimd_wasm_$(BUILD_TYPE_DIR)/objs
	cd ../build_nosimd_wasm_$(BUILD_TYPE_DIR)/objs && \
		$(CC) $(BUILD_TYPE_FLAGS) $(BUILD_BINARY_FLAGS) $(WASM2C_RUNTIME) $(RLBOX_INCLUDES) $(REPO_PATH)/build_nosimd_wasm_$(BUILD_TYPE_DIR)/libjpeg.wasm.c -c $(WASM2C_BUILD_INCLUDES)
	$(CXX) $(BUILD_TYPE_FLAGS) $(BUILD_BINARY_FLAGS) ../build_nosimd_wasm_$(BUILD_TYPE_DIR)/objs/*.o -o $@ $< -DRLBOX_WASM2C $(LIBJPEG_NOSIMD_WASM_INCLUDES) $(WASM2C_BUILD_INCLUDES) $(RLBOX_INCLUDES) $(RLBOX_LINK)


#####################################

build: wasi-sdk wabt rlbox rlbox_wasm2c_sandbox
build: test_bytes.h
build: ../build_nosimd_$(BUILD_TYPE_DIR)/image_change_quality
build: ../build_nosimd_$(BUILD_TYPE_DIR)/image_change_quality_rlbox_noop
build: ../build_nosimd_wasm_$(BUILD_TYPE_DIR)/image_change_quality_rlbox_wasm2c

test:
	cd $(REPO_PATH)/build_nosimd_$(BUILD_TYPE_DIR) && echo -n "Stock NoSIMD " && ./image_change_quality
	sleep 1
	cd $(REPO_PATH)/build_nosimd_$(BUILD_TYPE_DIR) && echo -n "RLBox Noop NoSIMD " && ./image_change_quality_rlbox_noop
	sleep 1
	cd $(REPO_PATH)/build_nosimd_wasm_$(BUILD_TYPE_DIR) && echo -n "RLBox Wasm2c NoSIMD " && ./image_change_quality_rlbox_wasm2c

clean:
	rm -f test_out.jpeg
	rm -f test_bytes.h
	rm -rf ../build_*/
	rm -rf $(WASM2C_SRC_PATH)/build_*