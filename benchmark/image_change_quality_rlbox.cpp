#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <stdio.h>

using namespace std::chrono;

#include "jpeglib.h"

#include "test_bytes.h"

#define RLBOX_SINGLE_THREADED_INVOCATIONS
#include "rlbox_types.hpp"

#ifdef RLBOX_WASM2C

#include "libjpeg.wasm.h"
#define RLBOX_USE_STATIC_CALLS() rlbox_wasm2c_sandbox_lookup_symbol
#include "rlbox_wasm2c_sandbox.hpp"
RLBOX_DEFINE_BASE_TYPES_FOR(jpeg, wasm2c)

#else

#define RLBOX_USE_STATIC_CALLS() rlbox_noop_sandbox_lookup_symbol
#include "rlbox_noop_sandbox.hpp"
RLBOX_DEFINE_BASE_TYPES_FOR(jpeg, noop)

#endif

#include "rlbox.hpp"

#define sandbox_fields_reflection_jpeg_class_jpeg_compress_struct(f, g, ...)           \
  f(struct jpeg_error_mgr *       ,err                     , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_memory_mgr *      ,mem                     , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_progress_mgr *    ,progress                , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(void *                        ,client_data             , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                       ,is_decompressor         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                           ,global_state            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_destination_mgr * ,dest                    , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION                    ,image_width             , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION                    ,image_height            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                           ,input_components        , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(J_COLOR_SPACE                 ,in_color_space          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(double, input_gamma                                    , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, data_precision                                    , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, num_components                                    , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(J_COLOR_SPACE, jpeg_color_space                        , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(jpeg_component_info*, comp_info                        , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JQUANT_TBL*[NUM_QUANT_TBLS], quant_tbl_ptrs            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JHUFF_TBL*[NUM_HUFF_TBLS], dc_huff_tbl_ptrs            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JHUFF_TBL*[NUM_HUFF_TBLS], ac_huff_tbl_ptrs            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8[NUM_ARITH_TBLS], arith_dc_L                      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8[NUM_ARITH_TBLS], arith_dc_U                      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8[NUM_ARITH_TBLS], arith_ac_K                      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, num_scans                                         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(const jpeg_scan_info*, scan_info                       , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean, raw_data_in                                   , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean, arith_code                                    , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean, optimize_coding                               , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean, CCIR601_sampling                              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, smoothing_factor                                  , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(J_DCT_METHOD, dct_method                               , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(unsigned int, restart_interval                         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, restart_in_rows                                   , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean, write_JFIF_header                             , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8, JFIF_major_version                              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8, JFIF_minor_version                              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8, density_unit                                    , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT16, X_density                                      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT16, Y_density                                      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean, write_Adobe_marker                            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION, next_scanline                              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean, progressive_mode                              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, max_h_samp_factor                                 , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, max_v_samp_factor                                 , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION, total_iMCU_rows                            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, comps_in_scan                                     , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(jpeg_component_info*[MAX_COMPS_IN_SCAN], cur_comp_info , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION, MCUs_per_row                               , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION, MCU_rows_in_scan                           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, blocks_in_MCU                                     , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int[C_MAX_BLOCKS_IN_MCU], MCU_membership               , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, Ss                                                , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, Se                                                , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, Ah                                                , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, Al                                                , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_comp_master*, master                       , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_c_main_controller*, main                   , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_c_prep_controller*, prep                   , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_c_coef_controller*, coef                   , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_marker_writer*, marker                     , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_color_converter*, cconvert                 , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_downsampler*, downsample                   , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_forward_dct*, fdct                         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_entropy_encoder*, entropy                  , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(jpeg_scan_info*, script_space                          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, script_space_size                                 , FIELD_NORMAL, ##__VA_ARGS__) g()

#define sandbox_fields_reflection_jpeg_class_jpeg_decompress_struct(f, g, ...)         \
  f(struct jpeg_error_mgr *       ,err              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_memory_mgr *      ,mem              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_progress_mgr *    ,progress         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(void *                        ,client_data      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                       ,is_decompressor  , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                           ,global_state     , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_source_mgr *      ,src              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION                    ,image_width      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION                    ,image_height     , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                           ,num_components   , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(J_COLOR_SPACE                 ,jpeg_color_space , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(J_COLOR_SPACE                          , out_color_space         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(unsigned int                           , scale_num               , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(unsigned int                           , scale_denom             , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(double                                 , output_gamma            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , buffered_image          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , raw_data_out            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(J_DCT_METHOD                           , dct_method              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , do_fancy_upsampling     , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , do_block_smoothing      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , quantize_colors         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(J_DITHER_MODE                          , dither_mode             , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , two_pass_quantize       , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , desired_number_of_colors, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , enable_1pass_quant      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , enable_external_quant   , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , enable_2pass_quant      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION                             , output_width            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION                             , output_height           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , out_color_components    , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , output_components       , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , rec_outbuf_height       , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , actual_number_of_colors , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JSAMPARRAY                             , colormap                , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION                             , output_scanline         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , input_scan_number       , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION                             , input_iMCU_row          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , output_scan_number      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION                             , output_iMCU_row         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int(*)[DCTSIZE2]                       ,coef_bits                , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JQUANT_TBL*[NUM_QUANT_TBLS]            , quant_tbl_ptrs          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JHUFF_TBL*[NUM_HUFF_TBLS]              , dc_huff_tbl_ptrs        , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JHUFF_TBL*[NUM_HUFF_TBLS]              , ac_huff_tbl_ptrs        , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , data_precision          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(jpeg_component_info*                   , comp_info               , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , progressive_mode        , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , arith_code              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8[NUM_ARITH_TBLS]                  , arith_dc_L              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8[NUM_ARITH_TBLS]                  , arith_dc_U              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8[NUM_ARITH_TBLS]                  , arith_ac_K              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(unsigned int                           , restart_interval        , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , saw_JFIF_marker         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8                                  , JFIF_major_version      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8                                  , JFIF_minor_version      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8                                  , density_unit            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT16                                 , X_density               , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT16                                 , Y_density               , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , saw_Adobe_marker        , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(UINT8                                  , Adobe_transform         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(boolean                                , CCIR601_sampling        , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(jpeg_saved_marker_ptr                  , marker_list             , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , max_h_samp_factor       , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , max_v_samp_factor       , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , min_DCT_scaled_size     , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION                             , total_iMCU_rows         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JSAMPLE*                               , sample_range_limit      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , comps_in_scan           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(jpeg_component_info*[MAX_COMPS_IN_SCAN], cur_comp_info           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION                             , MCUs_per_row            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(JDIMENSION                             , MCU_rows_in_scan        , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , blocks_in_MCU           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int[D_MAX_BLOCKS_IN_MCU]               , MCU_membership          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , Ss                      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , Se                      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , Ah                      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , Al                      , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int                                    , unread_marker           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_decomp_master*             , master                  , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_d_main_controller*         , main                    , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_d_coef_controller*         , coef                    , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_d_post_controller*         , post                    , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_input_controller*          , inputctl                , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_marker_reader*             , marker                  , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_entropy_decoder*           , entropy                 , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_inverse_dct*               , idct                    , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_upsampler*                 , upsample                , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_color_deconverter*         , cconvert                , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(struct jpeg_color_quantizer*           , cquantize               , FIELD_NORMAL, ##__VA_ARGS__) g()


#define sandbox_fields_reflection_jpeg_allClasses(f, ...) \
  f(jpeg_compress_struct, jpeg, ##__VA_ARGS__)            \
  f(jpeg_decompress_struct, jpeg, ##__VA_ARGS__)

rlbox_load_structs_from_library(jpeg);

#define RELEASE_ASSERT(cond, msg)           \
  if(!(cond))                               \
  {                                         \
    printf("FAILED: " #cond ". " msg "\n"); \
    exit(1);                                \
  }

struct jpeg_parsed_data {
  tainted_jpeg<JSAMPLE*> image_buffer;
  tainted_jpeg<size_t> image_buffer_size;
  tainted_jpeg<JDIMENSION> image_height;
  tainted_jpeg<JDIMENSION> image_width;
};

struct jpeg_parsed_data read_jpeg(rlbox_sandbox_jpeg& sandbox, tainted_jpeg<unsigned char *> fileBuff, unsigned long fsize) {
  struct jpeg_parsed_data ret {0};

  tainted_jpeg<struct jpeg_decompress_struct*> p_cinfo = sandbox.malloc_in_sandbox<struct jpeg_decompress_struct>();
  tainted_jpeg<struct jpeg_error_mgr*> p_jerr = sandbox.malloc_in_sandbox<struct jpeg_error_mgr>();

  tainted_volatile_jpeg<struct jpeg_decompress_struct>& cinfo = *p_cinfo;

  cinfo.err = sandbox_invoke(sandbox, jpeg_std_error, p_jerr);

  sandbox_invoke(sandbox, jpeg_CreateDecompress, p_cinfo, JPEG_LIB_VERSION, (size_t)sizeof(tainted_volatile_jpeg<struct jpeg_decompress_struct>));
  sandbox_invoke(sandbox, jpeg_mem_src, p_cinfo, fileBuff, fsize);
  sandbox_invoke(sandbox, jpeg_read_header, p_cinfo, TRUE);
  sandbox_invoke(sandbox, jpeg_start_decompress, p_cinfo);

  tainted_jpeg<JDIMENSION> row_stride = cinfo.output_width * cinfo.output_components;

  ret.image_height = cinfo.output_height;
  ret.image_width = cinfo.output_width;
  ret.image_buffer_size = ret.image_width * ret.image_height * 3 * sizeof(JSAMPLE);
  ret.image_buffer = sandbox.malloc_in_sandbox<JSAMPLE>(ret.image_buffer_size.unverified_safe_because("malloc size"));

  RELEASE_ASSERT(ret.image_buffer, "Memory alloc failure");

  int curr_image_row = 0;

  tainted_jpeg<JSAMPLE**> p_target = sandbox.malloc_in_sandbox<JSAMPLE*>();
  while ((cinfo.output_scanline < cinfo.output_height).unverified_safe_because("loop")) {
    tainted_jpeg<JSAMPLE*> target = &(ret.image_buffer[curr_image_row * row_stride]);
    *p_target = target;
    sandbox_invoke(sandbox, jpeg_read_scanlines, p_cinfo, p_target, 1);
    curr_image_row++;
  }

  sandbox.free_in_sandbox(p_target);

  sandbox_invoke(sandbox, jpeg_finish_decompress, p_cinfo);
  sandbox_invoke(sandbox, jpeg_destroy_decompress, p_cinfo);

  sandbox.free_in_sandbox(p_jerr);
  sandbox.free_in_sandbox(p_cinfo);

  return ret;
}

struct jpeg_parsed_data write_jpeg(rlbox_sandbox_jpeg& sandbox, int quality, struct jpeg_parsed_data input) {
  struct jpeg_parsed_data ret {0};

  tainted_jpeg<struct jpeg_compress_struct*> p_cinfo = sandbox.malloc_in_sandbox<struct jpeg_compress_struct>();
  tainted_jpeg<struct jpeg_error_mgr*> p_jerr = sandbox.malloc_in_sandbox<struct jpeg_error_mgr>();

  tainted_volatile_jpeg<struct jpeg_compress_struct>& cinfo = *p_cinfo;

  cinfo.err = sandbox_invoke(sandbox, jpeg_std_error, p_jerr);
  sandbox_invoke(sandbox, jpeg_CreateCompress, p_cinfo, JPEG_LIB_VERSION, (size_t)sizeof(tainted_volatile_jpeg<struct jpeg_compress_struct>));

  tainted_jpeg<unsigned char **> p_outbuffer = sandbox.malloc_in_sandbox<unsigned char *>();
  tainted_jpeg<unsigned long *> p_outsize = sandbox.malloc_in_sandbox<unsigned long>();
  *p_outbuffer = nullptr;
  *p_outsize = 0;
  sandbox_invoke(sandbox, jpeg_mem_dest, p_cinfo, p_outbuffer, p_outsize);

  cinfo.image_width = input.image_width;
  cinfo.image_height = input.image_height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  sandbox_invoke(sandbox, jpeg_set_defaults, p_cinfo);
  sandbox_invoke(sandbox, jpeg_set_quality, p_cinfo, quality, TRUE /* limit to baseline-JPEG values */);
  sandbox_invoke(sandbox, jpeg_start_compress, p_cinfo, TRUE);

  tainted_jpeg<JDIMENSION> row_stride = input.image_width * 3;

  tainted_jpeg<JSAMPROW*> row_pointer = sandbox.malloc_in_sandbox<JSAMPROW>();
  while ((cinfo.next_scanline < cinfo.image_height).unverified_safe_because("loop")) {
    row_pointer[0] = (& input.image_buffer[cinfo.next_scanline * row_stride]);
    (void) sandbox_invoke(sandbox, jpeg_write_scanlines, p_cinfo, row_pointer, 1);
  }
  sandbox.free_in_sandbox(row_pointer);

  sandbox_invoke(sandbox, jpeg_finish_compress, p_cinfo);

  ret.image_width = cinfo.image_width;
  ret.image_height = cinfo.image_height;
  ret.image_buffer_size = *p_outsize;
  ret.image_buffer = *p_outbuffer;

  sandbox_invoke(sandbox, jpeg_destroy_compress, p_cinfo);

  sandbox.free_in_sandbox(p_outbuffer);
  sandbox.free_in_sandbox(p_outsize);
  sandbox.free_in_sandbox(p_jerr);
  sandbox.free_in_sandbox(p_cinfo);

  return ret;
}

int main(int argc, char** argv) {
  unsigned long input_size = sizeof(inputData) - 1;
  unsigned long output_size = sizeof(outputData) - 1;

  rlbox_sandbox_jpeg sandbox;
  sandbox.create_sandbox();

  tainted_jpeg<unsigned char*> t_inputData = sandbox.malloc_in_sandbox<unsigned char>(input_size);
  rlbox::memcpy(sandbox, t_inputData, inputData, input_size);

  const int test_iterations = 100;
  struct jpeg_parsed_data in_jpeg_data {0}, out_jpeg_data {0};

  auto enter_time = high_resolution_clock::now();

  for (int i = 0; i < test_iterations; i++) {
    if (in_jpeg_data.image_buffer) {
      sandbox.free_in_sandbox(in_jpeg_data.image_buffer);
    }
    if (out_jpeg_data.image_buffer) {
      sandbox.free_in_sandbox(out_jpeg_data.image_buffer);
    }
    in_jpeg_data = read_jpeg(sandbox, t_inputData, input_size);
    out_jpeg_data = write_jpeg(sandbox, 30, in_jpeg_data);
  }

  auto exit_time = high_resolution_clock::now();

  sandbox.free_in_sandbox(t_inputData);

  // std::cout << "W" << in_jpeg_data.image_width << " L" << in_jpeg_data.image_height << "\n";

  RELEASE_ASSERT(output_size == out_jpeg_data.image_buffer_size.unverified_safe_because("check"), "Size mismatch");

  for(unsigned long i = 0; i < output_size; i++) {
    if (out_jpeg_data.image_buffer[i].unverified_safe_because("check") != outputData[i]) {
      printf("Output data doesn't match at index: %lu!\n", i);
      exit(1);
    }
  }

  int64_t ns = duration_cast<nanoseconds>(exit_time - enter_time).count();
  printf("JPEG recoding time: %lld\n", (long long) (ns / test_iterations));

  if (in_jpeg_data.image_buffer) {
    sandbox.free_in_sandbox(in_jpeg_data.image_buffer);
  }
  if (out_jpeg_data.image_buffer) {
    sandbox.free_in_sandbox(out_jpeg_data.image_buffer);
  }

  sandbox.destroy_sandbox();
}
