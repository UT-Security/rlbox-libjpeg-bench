#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "jpeglib.h"

void my_error_exit (j_common_ptr cinfo) {
  fprintf(stderr, "my_error_exit exit handler called\n");
  exit(1);
}

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Expeceted %s [inputfile] [outputfile]", argv[0]);
    return 1;
  }

  ////////////////////////////////

  const char* infilename = argv[1];
  FILE *infile = fopen(infilename, "rb");

  if (infile == NULL) {
    fprintf(stderr, "can't open %s\n", infilename);
    return 1;
  }

  struct jpeg_decompress_struct dinfo = {0};
  struct jpeg_error_mgr djerr = {0};

  dinfo.err = jpeg_std_error(&djerr);
  djerr.error_exit = my_error_exit;

  jpeg_create_decompress(&dinfo);
  jpeg_stdio_src(&dinfo, infile);

  jpeg_read_header(&dinfo, TRUE);
  jpeg_start_decompress(&dinfo);

  ////////////////////////////////

  const char* outfilename = argv[2];
  FILE *outfile = fopen(outfilename, "wb");
  if (outfile == NULL) {
    fprintf(stderr, "can't open %s\n", outfilename);
    return 1;
  }

  struct jpeg_compress_struct cinfo = {0};
  struct jpeg_error_mgr cjerr = {0};

  cinfo.err = jpeg_std_error(&cjerr);
  cjerr.error_exit = my_error_exit;

  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = dinfo.output_width;
  cinfo.image_height = dinfo.output_height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, 30 /* quality */, TRUE);

  jpeg_start_compress(&cinfo, TRUE);

  ////////////////////////////////

  int row_stride = dinfo.output_width * dinfo.output_components;
  JSAMPROW row_buffer = malloc(sizeof(JSAMPLE) * row_stride);
  JSAMPARRAY buffer = &row_buffer;

  while (cinfo.next_scanline < cinfo.image_height) {
    if (dinfo.output_scanline >= dinfo.output_height) {
      fprintf(stderr, "Unexpected: ran out of input lines\n");
      return 1;
    }

    jpeg_read_scanlines(&dinfo, buffer, 1);
    jpeg_write_scanlines(&cinfo, buffer, 1);
  }

  ////////////////////////////////

  jpeg_finish_compress(&cinfo);
  fclose(outfile);
  jpeg_destroy_compress(&cinfo);

  jpeg_finish_decompress(&dinfo);
  jpeg_destroy_decompress(&dinfo);
  fclose(infile);

  free(row_buffer);
}