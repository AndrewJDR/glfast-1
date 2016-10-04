#include "../../glfast.h"

// clang-format off
typedef bool     b8;
typedef char     c8;
typedef int8_t   i8;
typedef uint8_t  u8;
typedef int16_t  i16;
typedef uint16_t u16;
typedef int32_t  i32;
typedef uint32_t u32;
typedef int64_t  i64;
typedef uint64_t u64;
typedef uint16_t f16;
typedef float    f32;
typedef double   f64;
// clang-format on

// clang-format off
#define internal static
#define var __auto_type
#define let __auto_type const
#define case break; case
#define default break; default
#define _Merge(x, y) x##y
#define _Anyname(x) _Merge(_Anyname_, x)
#define _ _Anyname(__COUNTER__)
#define defer(x) __attribute__((cleanup(x)))
#define streq(x, y) (strcmp(x, y) == 0)
#define use __attribute__((warn_unused_result))
#define countof(array) (sizeof(array) / sizeof((array)[0]))
#define forcount(index, count) for (size_t index = 0, size = count; index < size; ++index)
#define foruntil(index, end, array) for (size_t index = 0; (array)[index] != end; ++index)
#define forrange(index, start, end) for (size_t index = start, stop = end; index != stop; ++index)
// clang-format on

typedef struct
{
  f32 x, y, z, w;
} vec4;

internal void SDLFree(void ** memory)
{
  if (memory)
    SDL_free(*memory);
}

i32 main()
{
  SDL_Window * sdl_window;
  SDL_GLContext sdl_glcontext;

  gfWindow(&sdl_window, &sdl_glcontext, 0, 0, "GPGPU", 1280, 720, 4);

  let dim_x = 4096;
  let dim_y = 4096;

  struct gpu_array_t mat_1 = {};
  struct gpu_array_t mat_2 = {};

  gfCreateArray(dim_x * dim_y, x_f32, &mat_1);
  gfCreateArray(dim_x * dim_y, x_f32, &mat_2);

  var mat_1_f32 = (f32 *)mat_1.ptr;
  var mat_2_f32 = (f32 *)mat_2.ptr;

  forcount(i, (size_t)mat_1.count) mat_1_f32[i] = (f32)i + 1.f;
  forcount(i, (size_t)mat_2.count) mat_2_f32[i] = (f32)i + 1.f;

  const char * vs_string =
      GF_VERT_HEAD " const vec2 quad[] = vec2[]                     \n"
                   " (                                              \n"
                   "   vec2( -1.0, -1.0 ),                          \n"
                   "   vec2(  1.0, -1.0 ),                          \n"
                   "   vec2( -1.0,  1.0 ),                          \n"
                   "   vec2( -1.0,  1.0 ),                          \n"
                   "   vec2(  1.0, -1.0 ),                          \n"
                   "   vec2(  1.0,  1.0 )                           \n"
                   " );                                             \n"
                   "                                                \n"
                   " void main()                                    \n"
                   " {                                              \n"
                   "   gl_Position = vec4(quad[gl_VertexID], 0, 1); \n"
                   " }                                              \n";

  const char * fs_string = GF_FRAG_HEAD
      " layout(binding = 0) uniform samplerBuffer s_mat_1;               \n"
      " layout(binding = 1) uniform samplerBuffer s_mat_2;               \n"
      "                                                                  \n"
      " layout(location = 0) out vec4 fbo_color;                         \n"
      "                                                                  \n"
      " void main()                                                      \n"
      " {                                                                \n"
      "   int x_coord = int(floor(gl_FragCoord.x));                      \n"
      "   int y_coord = int(floor(gl_FragCoord.y));                      \n"
      "                                                                  \n"
      "   float mat_1 = texelFetch(s_mat_1, y_coord * 4096 + x_coord).x; \n"
      "   float mat_2 = texelFetch(s_mat_2, y_coord * 4096 + x_coord).x; \n"
      "                                                                  \n"
      "   float mat_sum = mat_1 + mat_2;                                 \n"
      "                                                                  \n"
      "   fbo_color = vec4(mat_sum, 0, 0, 1);                            \n"
      " }                                                                \n";

  u32 vs = 0;
  u32 fs = 0;
  u32 pp = 0;

  gfCreateProgramFromString(GL_VERTEX_SHADER, vs_string, &vs);
  gfCreateProgramFromString(GL_FRAGMENT_SHADER, fs_string, &fs);
  gfCreateProgramPipeline(vs, fs, &pp);

  struct gpu_texture_t fbo_color = {};
  gfCreateTexture(dim_x, dim_y, 1, 1, rgba_f32, 0, &fbo_color);

  u32 fbo = 0;

  gfCreateFbo(0, 0, fbo_color.id, 0, 0, 0, 0, 0, 0, 0, &fbo);

  u32 state_textures[16] = {0};
  state_textures[0] = mat_1.id;
  state_textures[1] = mat_2.id;

  glBindTextures(0, 16, state_textures);

  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(sdl_window);

  i32 win_w = 0;
  i32 win_h = 0;

  SDL_GetWindowSize(sdl_window, &win_w, &win_h);

  glViewport(0, 0, dim_x, dim_y);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindProgramPipeline(pp);
  gfDraw(1, &(struct gpu_cmd_t){.count = 6, .instance_count = 1});
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, win_w, win_h);

  let pixels_bytes = dim_x * dim_y * (i32)sizeof(vec4);
  defer(SDLFree) void * pixels = SDL_malloc((size_t)pixels_bytes);

  gfGetTexturePixels(
      fbo_color.id, 0, 0, 0, dim_x, dim_y, GL_RGBA, GL_FLOAT, pixels_bytes,
      pixels);

  let pix = (vec4 *)pixels;

  char print_str[10000] = {};
  SDL_snprintf(
      print_str, 10000, "pixels[%d].rgba: %f %f %f %f\n", dim_x * dim_y - 1,
      pix[dim_x * dim_y - 1].x, pix[dim_x * dim_y - 1].y,
      pix[dim_x * dim_y - 1].z, pix[dim_x * dim_y - 1].w);

  SDL_ShowSimpleMessageBox(
      SDL_MESSAGEBOX_INFORMATION, "Completed", print_str, NULL);

  return 0;
}
