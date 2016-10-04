#include "../../glfast.h"
#include "flycamera/flycamera.h"
#include <stdio.h>

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

#ifndef RELEASE
#define error(name)                                                            \
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[%s] %s\n", __FUNCTION__, name); \
  return djb2(name)
#else
#define error(name) return djb2(name)
#endif

#define MAX_STR 5000

typedef struct
{
  f32 x, y, z;
} vec3;

typedef struct
{
  f32 x, y, z, w;
} vec4;

typedef struct
{
  f32 sd_x, up_x, fw_x;
  f32 sd_y, up_y, fw_y;
  f32 sd_z, up_z, fw_z;
} mat9;

// clang-format off
union
{
  c8 c[1];
  struct
  {
    c8 monkey_ibo [MAX_STR];
    c8 sphere_ibo [MAX_STR];
    c8 teapot_ibo [MAX_STR];
    c8 texture_1  [MAX_STR];
    c8 texture_2  [MAX_STR];
    c8 texture_3  [MAX_STR];
    c8 cubemap_px [MAX_STR];
    c8 cubemap_nx [MAX_STR];
    c8 cubemap_py [MAX_STR];
    c8 cubemap_ny [MAX_STR];
    c8 cubemap_pz [MAX_STR];
    c8 cubemap_nz [MAX_STR];
    c8 vs_cubemap [MAX_STR];
    c8 fs_cubemap [MAX_STR];
    c8 vs_mesh    [MAX_STR];
    c8 fs_mesh    [MAX_STR];
    c8 vs_quad    [MAX_STR];
    c8 fs_quad    [MAX_STR];
  };
} RESRC =
{
  .monkey_ibo = "data/meshes/monkey.ibo",
  .sphere_ibo = "data/meshes/sphere.ibo",
  .teapot_ibo = "data/meshes/teapot.ibo",
  .texture_1  = "data/textures/texture_1.bmp",
  .texture_2  = "data/textures/texture_2.bmp",
  .texture_3  = "data/textures/texture_3.bmp",
  .cubemap_px = "data/textures/cubemap_right.bmp",
  .cubemap_nx = "data/textures/cubemap_left.bmp",
  .cubemap_py = "data/textures/cubemap_top.bmp",
  .cubemap_ny = "data/textures/cubemap_bottom.bmp",
  .cubemap_pz = "data/textures/cubemap_front.bmp",
  .cubemap_nz = "data/textures/cubemap_back.bmp",
  .vs_cubemap = "shaders/cubemap.vert",
  .fs_cubemap = "shaders/cubemap.frag",
  .vs_mesh    = "shaders/mesh.vert",
  .fs_mesh    = "shaders/mesh.frag",
  .vs_quad    = "shaders/quad.vert",
  .fs_quad    = "shaders/quad.frag"
};
// clang-format on

internal u32 djb2(const c8 * s)
{
  u32 h = 5381;
  for (c8 c = *s; c != '\0'; c = *++s)
    h = h * 33 + (u8)c;
  return h & UINT32_MAX;
}

internal inline void
Perspective(f32 * proj, f32 * fcoef, f32 aspect, f32 fov_y_rad, f32 n, f32 f)
{
  let d = 1.f / (f32)tan(fov_y_rad * 0.5f);
  proj[0] = d / aspect;
  proj[1] = d;
  proj[2] = n / (n - f);
  proj[3] = (f * n) / (n - f);
  *fcoef = 2.f / (f32)(log(f + n) / log(2.f));
}

internal inline f32 Aspect(SDL_Window * sdl_window)
{
  i32 w, h;
  SDL_GetWindowSize(sdl_window, &w, &h);
  return (f32)w / (f32)h;
}

internal void SDLRWClose(SDL_RWops ** fd)
{
  if (fd)
    SDL_RWclose(*fd);
}

internal u32 ReadIBO(const c8 * ibo_filepath, size_t * bytes, vec4 * data)
{
  defer(SDLRWClose) SDL_RWops * fd = SDL_RWFromFile(ibo_filepath, "rb");

  if (!fd)
  {
    error("SDLRWFromFileReturnedNull");
  }

  SDL_RWseek(fd, 0, RW_SEEK_END);
  *bytes = (size_t)SDL_RWtell(fd);
  SDL_RWseek(fd, 0, RW_SEEK_SET);

  if (data)
    SDL_RWread(fd, data, *bytes, 1);

  return 0;
}

i32 main(i32 ArgCount, c8 ** Args)
{
  let path_exe = SDL_GetBasePath();

  for (size_t i = 0, size = sizeof(RESRC); i < size; i += MAX_STR)
  {
    c8 path_res[MAX_STR];
    SDL_memcpy(path_res, &RESRC.c[i], MAX_STR);
    SDL_snprintf(&RESRC.c[i], MAX_STR, "%s%s", path_exe, path_res);
  }

  SDL_Window * sdl_window;
  SDL_GLContext sdl_glcontext;

  gfWindow(&sdl_window, &sdl_glcontext, 0, 0, "App", 1280, 720, 4);

  size_t monkey_bytes = 0;
  size_t sphere_bytes = 0;
  size_t teapot_bytes = 0;

  ReadIBO(RESRC.monkey_ibo, &monkey_bytes, NULL);
  ReadIBO(RESRC.sphere_ibo, &sphere_bytes, NULL);
  ReadIBO(RESRC.teapot_ibo, &teapot_bytes, NULL);

  let monkey_vec4_count = (i32)(monkey_bytes / sizeof(vec4));
  let sphere_vec4_count = (i32)(sphere_bytes / sizeof(vec4));
  let teapot_vec4_count = (i32)(teapot_bytes / sizeof(vec4));

  struct gpu_array_t meshes = {};

  gfCreateArray(
      monkey_vec4_count + sphere_vec4_count + teapot_vec4_count, xyzw_f32,
      &meshes);

  let meshes_vec4 = (vec4 *)meshes.ptr;

  // clang-format off
  ReadIBO(RESRC.monkey_ibo, &monkey_bytes, (vec4 *)&meshes_vec4[0]);
  ReadIBO(RESRC.sphere_ibo, &sphere_bytes, (vec4 *)&meshes_vec4[monkey_vec4_count]);
  ReadIBO(RESRC.teapot_ibo, &teapot_bytes, (vec4 *)&meshes_vec4[monkey_vec4_count + sphere_vec4_count]);
  // clang-format on

  let monkey_count = monkey_vec4_count / 2;
  let sphere_count = sphere_vec4_count / 2;
  let teapot_count = teapot_vec4_count / 2;

  let monkey_first = 0;
  let sphere_first = monkey_count;
  let teapot_first = monkey_count + sphere_count;

  // clang-format off
  const struct gpu_cmd_t cmd[3] =
  {
    [0].first = monkey_first,
    [1].first = sphere_first,
    [2].first = teapot_first,

    [0].count = monkey_count,
    [1].count = sphere_count,
    [2].count = teapot_count,

    [0].instance_first = 0,
    [1].instance_first = 30,
    [2].instance_first = 60,

    [0].instance_count = 30,
    [1].instance_count = 30,
    [2].instance_count = 30
  };
  // clang-format on

  struct gpu_array_t meshes_id = {};

  gfCreateArray(monkey_count + sphere_count + teapot_count, x_i8, &meshes_id);

  SDL_memset(&meshes_id.ptr[monkey_first], 0, (size_t)monkey_count);
  SDL_memset(&meshes_id.ptr[sphere_first], 1, (size_t)sphere_count);
  SDL_memset(&meshes_id.ptr[teapot_first], 2, (size_t)teapot_count);

  struct gpu_array_t ins_first = {};
  struct gpu_array_t ins_pos = {};

  gfCreateArray(countof(cmd), x_i32, &ins_first);
  gfCreateArray(90, xyz_f32, &ins_pos);

  let ins_first_i32 = (i32 *)ins_first.ptr;
  let ins_pos_vec3 = (vec3 *)ins_pos.ptr;

  ins_first_i32[0] = cmd[0].instance_first;
  ins_first_i32[1] = cmd[1].instance_first;
  ins_first_i32[2] = cmd[2].instance_first;

  for (i32 i = 0, row = 10, space = 3; i < 90; ++i)
  {
    ins_pos_vec3[i].x = (f32)(i * space - (i / row) * row * space);
    ins_pos_vec3[i].z = (f32)((i / row) * space);
  }

  const c8 * bmps[] = {
      RESRC.texture_1, RESRC.texture_2, RESRC.texture_3,
  };

  struct gpu_texture_t textures = {};
  gfCreateTextureFromBmp(512, 512, 4, countof(bmps), bmps, &textures, NULL);

  const c8 * cubemap_px[] = {RESRC.cubemap_px};
  const c8 * cubemap_nx[] = {RESRC.cubemap_nx};
  const c8 * cubemap_py[] = {RESRC.cubemap_py};
  const c8 * cubemap_ny[] = {RESRC.cubemap_ny};
  const c8 * cubemap_pz[] = {RESRC.cubemap_pz};
  const c8 * cubemap_nz[] = {RESRC.cubemap_nz};

  struct gpu_texture_t cubemaps = {};
  gfCreateCubemapFromBmp(
      512, 512, 4, countof(cubemap_px), cubemap_px, cubemap_nx, cubemap_py,
      cubemap_ny, cubemap_pz, cubemap_nz, &cubemaps, NULL, NULL);

  struct gpu_sampler_t s_textures = {};
  struct gpu_sampler_t s_fbo = {};

  gfCreateSampler(
      4, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT,
      &s_textures);
  gfCreateSampler(
      1, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT, &s_fbo);

  i32 win_w = 0;
  i32 win_h = 0;

  SDL_GetWindowSize(sdl_window, &win_w, &win_h);

  struct gpu_texture_t fbo_depth = {};
  struct gpu_texture_t fbo_color = {};

  gfCreateTexture(win_w, win_h, 1, 1, depth_f32, 0, &fbo_depth);
  gfCreateTexture(win_w, win_h, 1, 1, srgba_b8, 0, &fbo_color);

  u32 fbo = 0;

  gfCreateFbo(fbo_depth.id, 0, fbo_color.id, 0, 0, 0, 0, 0, 0, 0, &fbo);

  // clang-format off
  u32 state_textures[16] =
  {
    [0] = meshes.id,
    [1] = meshes_id.id,
    [2] = ins_first.id,
    [3] = ins_pos.id,
    [4] = textures.id,
    [5] = cubemaps.id,
    [6] = fbo_color.id
  };

  u32 state_samplers[16] =
  {
    [4] = s_textures.id,
    [5] = s_textures.id,
    [6] = s_fbo.id
  };
  // clang-format on

  u32 vs_mesh = 0;
  u32 fs_mesh = 0;
  u32 pp_mesh = 0;

  u32 vs_quad = 0;
  u32 fs_quad = 0;
  u32 pp_quad = 0;

  u32 vs_cubemap = 0;
  u32 fs_cubemap = 0;
  u32 pp_cubemap = 0;

  gfCreateProgramFromFile(GL_VERTEX_SHADER, RESRC.vs_mesh, &vs_mesh);
  gfCreateProgramFromFile(GL_FRAGMENT_SHADER, RESRC.fs_mesh, &fs_mesh);
  gfCreateProgramPipeline(vs_mesh, fs_mesh, &pp_mesh);

  gfCreateProgramFromFile(GL_VERTEX_SHADER, RESRC.vs_quad, &vs_quad);
  gfCreateProgramFromFile(GL_FRAGMENT_SHADER, RESRC.fs_quad, &fs_quad);
  gfCreateProgramPipeline(vs_quad, fs_quad, &pp_quad);

  gfCreateProgramFromFile(GL_VERTEX_SHADER, RESRC.vs_cubemap, &vs_cubemap);
  gfCreateProgramFromFile(GL_FRAGMENT_SHADER, RESRC.fs_cubemap, &fs_cubemap);
  gfCreateProgramPipeline(vs_cubemap, fs_cubemap, &pp_cubemap);

  vec3 cam_pos = {23.518875f, 5.673130f, 26.64900f};
  vec4 cam_rot = {-0.351835f, 0.231701f, 0.090335f, 0.902411f};
  vec4 cam_prj = {};
  mat9 cam_mat = {};

  var fcoef = 0.f;
  var fnear = 0.005f;
  let torad = 0.017453f;

  Perspective(
      &cam_prj.x, &fcoef, Aspect(sdl_window), 85.f * torad, fnear, 1000000.f);

  SDL_SetRelativeMouseMode(1);
  u32 t_prev = SDL_GetTicks();

  while (1)
  {
    u32 t_curr = SDL_GetTicks();
    f64 dt = ((t_curr - t_prev) * 60.0) / 1000.0;

    SDL_PumpEvents();
    i32 mouse_x_rel = 0;
    i32 mouse_y_rel = 0;
    SDL_GetRelativeMouseState(&mouse_x_rel, &mouse_y_rel);
    const u8 * key = SDL_GetKeyboardState(NULL);

    flycamera(
        &cam_pos.x, &cam_rot.x, &cam_mat.sd_x, (f32)(0.10), (f32)(0.05 * dt),
        -(f32)mouse_x_rel, -(f32)mouse_y_rel, (f32)key[SDL_SCANCODE_W],
        (f32)key[SDL_SCANCODE_A], (f32)key[SDL_SCANCODE_S],
        (f32)key[SDL_SCANCODE_D], (f32)key[SDL_SCANCODE_E],
        (f32)key[SDL_SCANCODE_Q]);

    static int show_pass = 0;

    if (key[SDL_SCANCODE_0])
      show_pass = 0;
    if (key[SDL_SCANCODE_1])
      show_pass = 1;
    if (key[SDL_SCANCODE_2])
      show_pass = 2;
    if (key[SDL_SCANCODE_3])
      show_pass = 3;
    if (key[SDL_SCANCODE_4])
      show_pass = 4;
    if (key[SDL_SCANCODE_5])
      show_pass = 5;
    if (key[SDL_SCANCODE_6])
      show_pass = 6;
    if (key[SDL_SCANCODE_7])
      show_pass = 7;

    glProgramUniform3fv(vs_mesh, 0, 1, &cam_pos.x);
    glProgramUniform4fv(vs_mesh, 1, 1, &cam_rot.x);
    glProgramUniform4fv(vs_mesh, 2, 1, &cam_prj.x);
    glProgramUniform1fv(vs_mesh, 3, 1, &fcoef);
    glProgramUniform1fv(vs_mesh, 4, 1, &fnear);

    glProgramUniform3fv(fs_mesh, 0, 1, &cam_pos.x);
    glProgramUniform1iv(fs_mesh, 1, 1, &show_pass);
    glProgramUniform1fv(fs_mesh, 2, 1, &fcoef);

    glProgramUniform4fv(vs_cubemap, 0, 1, &cam_rot.x);
    glProgramUniform4fv(vs_cubemap, 1, 1, &cam_prj.x);

    forcount(i, 90) ins_pos_vec3[i].y =
        (f32)sin(((f32)t_curr * 0.0015f) + ((f32)i * 0.5f)) * 0.3f;

    glBindTextures(0, 16, state_textures);
    glBindSamplers(0, 16, state_samplers);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindProgramPipeline(pp_mesh);
    gfDraw(countof(cmd), cmd);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (show_pass == 0)
    {
      glDisable(GL_DEPTH_TEST);
      glBindProgramPipeline(pp_cubemap);
      gfDraw(1, &(struct gpu_cmd_t){.count = 36, .instance_count = 1});
      glEnable(GL_DEPTH_TEST);
    }

    glBindProgramPipeline(pp_quad);
    gfDraw(1, &(struct gpu_cmd_t){.count = 6, .instance_count = 1});

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        goto exit;
    }

    SDL_GL_SwapWindow(sdl_window);
    glFinish();

    t_prev = t_curr;
  }

exit:
  return 0;
}
