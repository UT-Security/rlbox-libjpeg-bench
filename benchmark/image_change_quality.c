#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef __wasm64__
// Need to stub in a few things due to lack of wasi

typedef struct FILE{
  int dummy;
} FILE;

int printf(const char* fmt, ...) {
return -1;
}

#define CLOCK_REALTIME 0
#define clockid_t int

struct timespec {
  int64_t tv_sec;
  long int tv_nsec;
};

int clock_gettime(clockid_t clockid, struct timespec *tp) {
return -1;
}

#else
#include <stdio.h>
#include <time.h>
#endif

#include "jpeglib.h"

#include "test_bytes.h"

#define RELEASE_ASSERT(cond, msg)           \
  if(!(cond))                               \
  {                                         \
    printf("FAILED: " #cond ". " msg "\n"); \
    exit(1);                                \
  }

void my_error_exit (j_common_ptr cinfo) {
  RELEASE_ASSERT(false, "my_error_exit exit handler called");
}

struct jpeg_parsed_data {
  JSAMPLE* image_buffer;
  size_t image_buffer_size;
  int image_height;
  int image_width;
};

struct jpeg_parsed_data read_jpeg(unsigned char *fileBuff, unsigned long fsize) {
  struct jpeg_parsed_data ret = {0};

  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jerr.error_exit = my_error_exit;

  jpeg_create_decompress(&cinfo);

  jpeg_mem_src(&cinfo, fileBuff, fsize);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  int row_stride = cinfo.output_width * cinfo.output_components;

  ret.image_height = cinfo.output_height;
  ret.image_width = cinfo.output_width;
  ret.image_buffer_size = ret.image_width * ret.image_height * 3 * sizeof(JSAMPLE);
  ret.image_buffer = (JSAMPLE *) malloc(ret.image_buffer_size);

  RELEASE_ASSERT(ret.image_buffer, "Memory alloc failure");

  int curr_image_row = 0;

  while (cinfo.output_scanline < cinfo.output_height) {
    JSAMPLE* target = &(ret.image_buffer[curr_image_row * row_stride]);
    jpeg_read_scanlines(&cinfo, &target, 1);
    curr_image_row++;
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  return ret;
}

struct jpeg_parsed_data write_jpeg(int quality, struct jpeg_parsed_data input) {
  struct jpeg_parsed_data ret = {0};

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);

  unsigned char * outbuffer = 0;
  unsigned long outsize = 0;
  jpeg_mem_dest(&cinfo, &outbuffer, &outsize);

  cinfo.image_width = input.image_width;
  cinfo.image_height = input.image_height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
  jpeg_start_compress(&cinfo, TRUE);

  int row_stride = input.image_width * 3;

  while (cinfo.next_scanline < cinfo.image_height) {
    JSAMPROW row_pointer[1];
    row_pointer[0] = & input.image_buffer[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);

  ret.image_width = cinfo.image_width;
  ret.image_height = cinfo.image_height;
  ret.image_buffer_size = outsize;
  ret.image_buffer = outbuffer;

  jpeg_destroy_compress(&cinfo);

  return ret;
}

__attribute__((used, noinline))
uint32_t pointer_deref(uint32_t* ptr) {
  return *ptr;
}

int main(int argc, char** argv) {
  unsigned long input_size = sizeof(inputData) - 1;
  unsigned long output_size = sizeof(outputData) - 1;

  struct timespec warmup_time = { 0 };
  for(int i = 0; i < 10; i++) {
    clock_gettime(CLOCK_REALTIME, &warmup_time);
    if(warmup_time.tv_nsec == 0 && warmup_time.tv_sec == 0) {
      printf("Clock not working\n");
      exit(1);
    }
  }

  struct timespec enter_time = { 0 };
  clock_gettime(CLOCK_REALTIME, &enter_time);

  const int test_iterations = 100;
  struct jpeg_parsed_data in_jpeg_data = {0};
  struct jpeg_parsed_data out_jpeg_data = {0};

  for (int i = 0; i < test_iterations; i++) {
    if (in_jpeg_data.image_buffer) {
      free(in_jpeg_data.image_buffer);
    }
    if (out_jpeg_data.image_buffer) {
      free(out_jpeg_data.image_buffer);
    }
    in_jpeg_data = read_jpeg(inputData, input_size);
    out_jpeg_data = write_jpeg(30, in_jpeg_data);
  }

  struct timespec exit_time = { 0 };
  clock_gettime(CLOCK_REALTIME, &exit_time);

  // std::cout << "W" << in_jpeg_data.image_width << " L" << in_jpeg_data.image_height << "\n";

  RELEASE_ASSERT(output_size == out_jpeg_data.image_buffer_size, "Size mismatch");

  for(unsigned long i = 0; i < output_size; i++) {
    if (out_jpeg_data.image_buffer[i] != outputData[i]) {
      printf("Output data doesn't match at index: %lu!\n", i);
      exit(1);
    }
  }

  const int64_t nanos = 1000000000;
  int64_t ns =  (nanos * (exit_time.tv_sec - enter_time.tv_sec))
    + ((int64_t)(exit_time.tv_nsec - enter_time.tv_nsec));

  printf("JPEG recoding time: %lld\n", (long long) (ns / test_iterations));

  if (in_jpeg_data.image_buffer) {
    free(in_jpeg_data.image_buffer);
  }
  if (out_jpeg_data.image_buffer) {
    free(out_jpeg_data.image_buffer);
  }
}
