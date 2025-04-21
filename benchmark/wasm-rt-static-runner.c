#include <stdio.h>
#include <stdlib.h>

#include "libjpeg.wasm.h"
#include "wasm2c_rt_minwasi.h"

int main(int argc, char** argv) {

  /* Initialize the Wasm runtime. */
  wasm_rt_init();
  if (!minwasi_init()) {
    printf("MinWasi init fail!\n");
    abort();
  }

  /* Declare an instance of the `inst` module. */
  w2c_libjpeg inst = { 0 };
  struct w2c_wasi__snapshot__preview1 wasi_env = { 0 };
  wasi_env.instance_memory = &inst.w2c_memory;

  if(!minwasi_init_instance(&wasi_env)) {
    printf("MinWasi init instance fail!\n");
    abort();
  }

  /* Construct the module instance. */
  wasm2c_libjpeg_instantiate(&inst, &wasi_env);

  /* Call `inst`, using the mangled name. */
  w2c_libjpeg_0x5Fstart(&inst);

  /* Free the inst module. */
  wasm2c_libjpeg_free(&inst);

  minwasi_cleanup_instance(&wasi_env);
  /* Free the Wasm runtime state. */
  wasm_rt_free();

  return 0;
}
