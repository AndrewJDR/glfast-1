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

internal u32 djb2(const c8 * s)
{
  u32 h = 5381;
  for (c8 c = *s; c != '\0'; c = *++s)
    h = h * 33 + (u8)c;
  return h & UINT32_MAX;
}

typedef struct
{
  f32 x, y, z;
} vec3;

i32 main()
{
  SDL_Window * sdl_window;
  SDL_GLContext sdl_glcontext;

  gfWindow(&sdl_window, &sdl_glcontext, 0, 0, "App", 1280, 720, 4);

  struct gpu_array_t mesh = {};
  gfCreateArray(3, xyz_f32, &mesh);

  var mesh_vec3 = (vec3 *)mesh.ptr;

  mesh_vec3[0].x = 0.0f;
  mesh_vec3[0].y = 0.5f;
  mesh_vec3[0].z = 0.0f;

  mesh_vec3[1].x = -0.5f;
  mesh_vec3[1].y = -0.5f;
  mesh_vec3[1].z = 0.0f;

  mesh_vec3[2].x = 0.5f;
  mesh_vec3[2].y = -0.5f;
  mesh_vec3[2].z = 0.0f;

  u32 texture_state[16] = {[0] = mesh.id};

  struct gpu_cmd_t cmd[1] = {[0].count = mesh.count, [0].instance_count = 1};

  let vs_string =
      GF_VERT_HEAD " layout(binding = 0) uniform samplerBuffer s_pos; \n"
                   "                                                  \n"
                   " void main()                                      \n"
                   " {                                                \n"
                   "   vec3 pos = texelFetch(s_pos, gl_VertexID).xyz; \n"
                   "   gl_Position = vec4(pos, 1.f);                  \n"
                   " }                                                \n";

  let fs_string = GF_FRAG_HEAD " out vec4 color;      \n"
                               "                      \n"
                               " void main()          \n"
                               " {                    \n"
                               "   color = vec4(1.f); \n"
                               " }                    \n";

  u32 vs = 0;
  u32 fs = 0;
  u32 pp = 0;

  gfCreateProgramFromString(GL_VERTEX_SHADER, vs_string, &vs);
  gfCreateProgramFromString(GL_FRAGMENT_SHADER, fs_string, &fs);
  gfCreateProgramPipeline(vs, fs, &pp);

  while (1)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTextures(0, 16, texture_state);
    glBindProgramPipeline(pp);
    gfDraw(countof(cmd), cmd);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        goto exit;
    }

    SDL_GL_SwapWindow(sdl_window);
    glFinish();
  }

exit:
  return 0;
}
