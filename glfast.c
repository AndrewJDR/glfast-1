#include "glfast.h"

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

internal u32 djb2(const c8 * _Nonnull s)
{
  u32 h = 5381;
  for (c8 c = *s; c != '\0'; c = *++s)
    h = h * 33 + (u8)c;
  return h & UINT32_MAX;
}

internal void SDLFreeSurface(SDL_Surface * _Nullable * _Nonnull surface)
{
  if (surface)
    SDL_FreeSurface(*surface);
}

internal void SDLFree(void * _Nullable * _Nonnull memory)
{
  if (memory)
    SDL_free(*memory);
}

internal void SDLRWClose(SDL_RWops * _Nullable * _Nonnull fd)
{
  if (fd)
    SDL_RWclose(*fd);
}

uint32_t
gfCreateArray(int32_t count, uint32_t format, struct gpu_array_t * _Nonnull tbo)
{
  var elem_width = 0;
  var elem_bytes = 0;

  switch (format)
  {
  // clang-format off
  case(x_b8):     elem_width = 1; elem_bytes = sizeof(u8);
  case(x_f16):    elem_width = 1; elem_bytes = sizeof(f16);
  case(x_f32):    elem_width = 1; elem_bytes = sizeof(f32);
  case(x_i8):     elem_width = 1; elem_bytes = sizeof(i8);
  case(x_i16):    elem_width = 1; elem_bytes = sizeof(i16);
  case(x_i32):    elem_width = 1; elem_bytes = sizeof(i32);
  case(x_u8):     elem_width = 1; elem_bytes = sizeof(u8);
  case(x_u16):    elem_width = 1; elem_bytes = sizeof(u16);
  case(x_u32):    elem_width = 1; elem_bytes = sizeof(u32);
  case(xy_b8):    elem_width = 2; elem_bytes = sizeof(u8);
  case(xy_f16):   elem_width = 2; elem_bytes = sizeof(f16);
  case(xy_f32):   elem_width = 2; elem_bytes = sizeof(f32);
  case(xy_i8):    elem_width = 2; elem_bytes = sizeof(i8);
  case(xy_i16):   elem_width = 2; elem_bytes = sizeof(i16);
  case(xy_i32):   elem_width = 2; elem_bytes = sizeof(i32);
  case(xy_u8):    elem_width = 2; elem_bytes = sizeof(u8);
  case(xy_u16):   elem_width = 2; elem_bytes = sizeof(u16);
  case(xy_u32):   elem_width = 2; elem_bytes = sizeof(u32);
  case(xyz_f32):  elem_width = 3; elem_bytes = sizeof(f32);
  case(xyz_i32):  elem_width = 3; elem_bytes = sizeof(i32);
  case(xyz_u32):  elem_width = 3; elem_bytes = sizeof(u32);
  case(xyzw_b8):  elem_width = 4; elem_bytes = sizeof(u8);
  case(xyzw_f16): elem_width = 4; elem_bytes = sizeof(f16);
  case(xyzw_f32): elem_width = 4; elem_bytes = sizeof(f32);
  case(xyzw_i8):  elem_width = 4; elem_bytes = sizeof(i8);
  case(xyzw_i16): elem_width = 4; elem_bytes = sizeof(i16);
  case(xyzw_i32): elem_width = 4; elem_bytes = sizeof(i32);
  case(xyzw_u8):  elem_width = 4; elem_bytes = sizeof(u8);
  case(xyzw_u16): elem_width = 4; elem_bytes = sizeof(u16);
  case(xyzw_u32): elem_width = 4; elem_bytes = sizeof(u32);
  // clang-format on
  default:
    error("WrongFormat");
  }

  let bytes = count * elem_width * elem_bytes;

  u32 buffer_id = 0;
  u32 id = 0;

  glCreateBuffers(1, &buffer_id);
  glCreateTextures(GL_TEXTURE_BUFFER, 1, &id);

  glNamedBufferStorage(
      buffer_id, bytes, NULL,
      GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

  let ptr = (u8 *)glMapNamedBufferRange(
      buffer_id, 0, bytes,
      GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

  glTextureBuffer(id, format, buffer_id);

  tbo->ptr = ptr;
  tbo->bytes = bytes;
  tbo->count = count;
  tbo->format = format;
  tbo->buffer_id = buffer_id;
  tbo->id = id;

  if (!ptr)
  {
    error("GLMapNamedBufferRangeReturnedNull");
  }

  SDL_memset(ptr, 0, (size_t)bytes);

  return 0;
}

void gfCreateTexture(
    int32_t width, int32_t height, int32_t mipmap_level, int32_t count,
    uint32_t format, uint8_t is_cubemap,
    struct gpu_texture_t * _Nonnull texture)
{
  u32 id = 0;

  glCreateTextures(
      is_cubemap ? GL_TEXTURE_CUBE_MAP_ARRAY : GL_TEXTURE_2D_ARRAY, 1, &id);

  glTextureStorage3D(
      id, mipmap_level, format, width, height, count * (is_cubemap ? 6 : 1));

  texture->w = width;
  texture->h = height;
  texture->mip = mipmap_level;
  texture->count = count;
  texture->format = format;
  texture->is_cubemap = is_cubemap;
  texture->id = id;
}

uint32_t gfCreateTextureFromBmp(
    int32_t width, int32_t height, int32_t mipmap_level, int32_t texture_count,
    const char * _Nonnull * _Nonnull texture_paths,
    struct gpu_texture_t * _Nonnull texture,
    size_t * _Nullable error_texture_index)
{
  gfCreateTexture(
      width, height, mipmap_level, texture_count, srgb_b8, 0, texture);

  let w = texture->w;
  let h = texture->h;
  let id = texture->id;

  forcount(i, (size_t)texture_count)
  {
    defer(SDLFreeSurface) SDL_Surface * bmp = SDL_LoadBMP(texture_paths[i]);

    if (!bmp)
    {
      if (error_texture_index)
      {
        *error_texture_index = i;
      }
      error("SDLLoadBMPReturnedNull");
    }

    gfSetTexturePixels(
        id, (i32)i, 0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, bmp->pixels);
  }

  glGenerateTextureMipmap(id);

  return 0;
}

uint32_t gfCreateCubemapFromBmp(
    int32_t width, int32_t height, int32_t mipmap_level, int32_t cubemap_count,
    const char * _Nonnull * _Nonnull pos_x_texture_paths,
    const char * _Nonnull * _Nonnull neg_x_texture_paths,
    const char * _Nonnull * _Nonnull pos_y_texture_paths,
    const char * _Nonnull * _Nonnull neg_y_texture_paths,
    const char * _Nonnull * _Nonnull pos_z_texture_paths,
    const char * _Nonnull * _Nonnull neg_z_texture_paths,
    struct gpu_texture_t * _Nonnull texture,
    int8_t * _Nullable error_texture_id, size_t * _Nullable error_texture_index)
{
  gfCreateTexture(
      width, height, mipmap_level, cubemap_count, srgb_b8, 1, texture);

  let w = texture->w;
  let h = texture->h;
  let id = texture->id;

  forcount(i, (size_t)cubemap_count)
  {
    // clang-format off
    defer(SDLFreeSurface) SDL_Surface * bmp_pos_x = SDL_LoadBMP(pos_x_texture_paths[i]);
    defer(SDLFreeSurface) SDL_Surface * bmp_neg_x = SDL_LoadBMP(neg_x_texture_paths[i]);
    defer(SDLFreeSurface) SDL_Surface * bmp_pos_y = SDL_LoadBMP(pos_y_texture_paths[i]);
    defer(SDLFreeSurface) SDL_Surface * bmp_neg_y = SDL_LoadBMP(neg_y_texture_paths[i]);
    defer(SDLFreeSurface) SDL_Surface * bmp_pos_z = SDL_LoadBMP(pos_z_texture_paths[i]);
    defer(SDLFreeSurface) SDL_Surface * bmp_neg_z = SDL_LoadBMP(neg_z_texture_paths[i]);

    if (!bmp_pos_x) { if (error_texture_id) *error_texture_id = 1; if (error_texture_index) *error_texture_index = i; error("SDLLoadBMPReturnedNull"); }
    if (!bmp_neg_x) { if (error_texture_id) *error_texture_id = 2; if (error_texture_index) *error_texture_index = i; error("SDLLoadBMPReturnedNull"); }
    if (!bmp_pos_y) { if (error_texture_id) *error_texture_id = 3; if (error_texture_index) *error_texture_index = i; error("SDLLoadBMPReturnedNull"); }
    if (!bmp_neg_y) { if (error_texture_id) *error_texture_id = 4; if (error_texture_index) *error_texture_index = i; error("SDLLoadBMPReturnedNull"); }
    if (!bmp_pos_z) { if (error_texture_id) *error_texture_id = 5; if (error_texture_index) *error_texture_index = i; error("SDLLoadBMPReturnedNull"); }
    if (!bmp_neg_z) { if (error_texture_id) *error_texture_id = 6; if (error_texture_index) *error_texture_index = i; error("SDLLoadBMPReturnedNull"); }

    gfSetTexturePixels(id, (i32)i * 6 + 0, 0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, bmp_pos_x->pixels);
    gfSetTexturePixels(id, (i32)i * 6 + 1, 0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, bmp_neg_x->pixels);
    gfSetTexturePixels(id, (i32)i * 6 + 2, 0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, bmp_pos_y->pixels);
    gfSetTexturePixels(id, (i32)i * 6 + 3, 0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, bmp_neg_y->pixels);
    gfSetTexturePixels(id, (i32)i * 6 + 4, 0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, bmp_pos_z->pixels);
    gfSetTexturePixels(id, (i32)i * 6 + 5, 0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, bmp_neg_z->pixels);
    // clang-format on
  }

  glGenerateTextureMipmap(id);

  return 0;
}

void gfCreateSampler(
    int32_t anisotropy_level, int32_t min_filter, int32_t mag_filter,
    int32_t s_wrapping, int32_t t_wrapping, int32_t r_wrapping,
    struct gpu_sampler_t * _Nonnull sampler)
{
  u32 id = 0;

  glCreateSamplers(1, &id);

  glSamplerParameteri(id, GL_TEXTURE_MAX_ANISOTROPY, anisotropy_level);
  glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, min_filter);
  glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, mag_filter);
  glSamplerParameteri(id, GL_TEXTURE_WRAP_S, s_wrapping);
  glSamplerParameteri(id, GL_TEXTURE_WRAP_T, t_wrapping);
  glSamplerParameteri(id, GL_TEXTURE_WRAP_R, r_wrapping);

  sampler->aniso = anisotropy_level;
  sampler->min = min_filter;
  sampler->mag = mag_filter;
  sampler->s_wrap = s_wrapping;
  sampler->t_wrap = t_wrapping;
  sampler->r_wrap = r_wrapping;
  sampler->id = id;
}

void gfCreateFbo(
    uint32_t depth_texture_id_0, int32_t depth_texture_layer_0,
    uint32_t color_texture_id_0, int32_t color_texture_layer_0,
    uint32_t color_texture_id_1, int32_t color_texture_layer_1,
    uint32_t color_texture_id_2, int32_t color_texture_layer_2,
    uint32_t color_texture_id_3, int32_t color_texture_layer_3,
    uint32_t * _Nonnull fbo)
{
  u32 id = 0;

  glCreateFramebuffers(1, &id);

  // clang-format off
  glNamedFramebufferTextureLayer(id, GL_DEPTH_ATTACHMENT  + 0, depth_texture_id_0, 0, depth_texture_layer_0);
  glNamedFramebufferTextureLayer(id, GL_COLOR_ATTACHMENT0 + 0, color_texture_id_0, 0, color_texture_layer_0);
  glNamedFramebufferTextureLayer(id, GL_COLOR_ATTACHMENT0 + 1, color_texture_id_1, 0, color_texture_layer_1);
  glNamedFramebufferTextureLayer(id, GL_COLOR_ATTACHMENT0 + 2, color_texture_id_2, 0, color_texture_layer_2);
  glNamedFramebufferTextureLayer(id, GL_COLOR_ATTACHMENT0 + 3, color_texture_id_3, 0, color_texture_layer_3);

  u32 attachments[4] = {};

  attachments[0] = color_texture_id_0 ? GL_COLOR_ATTACHMENT0 + 0 : 0;
  attachments[1] = color_texture_id_1 ? GL_COLOR_ATTACHMENT0 + 1 : 0;
  attachments[2] = color_texture_id_2 ? GL_COLOR_ATTACHMENT0 + 2 : 0;
  attachments[3] = color_texture_id_3 ? GL_COLOR_ATTACHMENT0 + 3 : 0;

  glNamedFramebufferDrawBuffers(id, 4, attachments);
  // clang-format on

  *fbo = id;
}

uint32_t gfCreateProgramFromFile(
    uint32_t shader_type, const char * _Nonnull shader_filepath,
    uint32_t * _Nonnull program)
{
  defer(SDLRWClose) SDL_RWops * fd = SDL_RWFromFile(shader_filepath, "rb");

  if (!fd)
  {
    error("SDLRWFromFileReturnedNull");
  }

  SDL_RWseek(fd, 0, RW_SEEK_END);
  let bytes = SDL_RWtell(fd);
  SDL_RWseek(fd, 0, RW_SEEK_SET);

  c8 src[bytes + 1];
  src[bytes] = 0;
  SDL_RWread(fd, src, bytes, 1);
  let source = &src[0];

  *program = glCreateShaderProgramv(shader_type, 1, (const c8 **)&source);

  return 0;
}

void gfCreateProgramFromString(
    uint32_t shader_type, const char * _Nonnull shader_string,
    uint32_t * _Nonnull program)
{
  *program =
      glCreateShaderProgramv(shader_type, 1, (const c8 **)&shader_string);
}

void gfCreateProgramPipeline(
    uint32_t vert_program_id, uint32_t frag_program_id, uint32_t * _Nonnull ppo)
{
  u32 id = 0;

  glCreateProgramPipelines(1, &id);

  glUseProgramStages(id, GL_VERTEX_SHADER_BIT, vert_program_id);
  glUseProgramStages(id, GL_FRAGMENT_SHADER_BIT, frag_program_id);

  *ppo = id;
}

void gfSetTexturePixels(
    uint32_t texture_id, int32_t texture_layer, int32_t x, int32_t y,
    int32_t width, int32_t height, uint32_t pixels_format, uint32_t pixels_type,
    const void * _Nonnull pixels)
{
  glTextureSubImage3D(
      texture_id, 0, x, y, texture_layer, width, height, 1, pixels_format,
      pixels_type, pixels);
}

void gfGetTexturePixels(
    uint32_t texture_id, int32_t texture_layer, int32_t x, int32_t y,
    int32_t width, int32_t height, uint32_t pixels_format, uint32_t pixels_type,
    int32_t pixels_bytes, void * _Nonnull pixels)
{
  glGetTextureSubImage(
      texture_id, 0, x, y, texture_layer, width, height, 1, pixels_format,
      pixels_type, pixels_bytes, pixels);
}

uint32_t gfSaveTextureToBmp(
    uint32_t texture_id, int32_t texture_layer, int32_t width, int32_t height,
    const char * _Nonnull bmp_filepath)
{
  let bytes = width * height * 3;

  defer(SDLFree) void * pixels = SDL_malloc((size_t)bytes);

  if (!pixels)
  {
    error("SDLMallocReturnedNull");
  }

  gfGetTexturePixels(
      texture_id, texture_layer, 0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE,
      bytes, pixels);

  defer(SDLFreeSurface) SDL_Surface * bmp = SDL_CreateRGBSurfaceFrom(
      pixels, width, height, 3 * 8, 3 * width, 0, 0, 0, 0);

  if (!bmp)
  {
    error("SDLCreateRGBSurfaceFromReturnedNull");
  }

  SDL_SaveBMP(bmp, bmp_filepath);

  return 0;
}

void gfDraw(int32_t gpu_cmd_count, const struct gpu_cmd_t * _Nonnull gpu_cmd)
{
  let draw = glDrawArraysInstancedBaseInstance;
  forcount(i, (size_t)gpu_cmd_count)
  {
    let cmd = gpu_cmd[i];
    draw(
        GL_TRIANGLES, cmd.first, cmd.count, cmd.instance_count,
        (u32)cmd.instance_first);
  }
}

void gfDebugCallback(
    uint32_t source, uint32_t type, uint32_t id, uint32_t severity,
    int32_t length, const char * _Null_unspecified message,
    void * _Nullable userdata)
{
  // clang-format off
  const c8 * GL_ERROR_SOURCE[] =
  {
    "API",
    "WINDOW SYSTEM",
    "SHADER COMPILER",
    "THIRD PARTY",
    "APPLICATION",
    "OTHER"
  };
  
  const c8 * GL_ERROR_SEVERITY[] =
  {
    "HIGH",
    "MEDIUM",
    "LOW",
    "NOTIFICATION"
  };
  
  const c8 * GL_ERROR_TYPE[] =
  {
    "ERROR",
    "DEPRECATED BEHAVIOR",
    "UNDEFINED DEHAVIOUR",
    "PORTABILITY",
    "PERFORMANCE",
    "OTHER"
  };

  SDL_Log(
    "\n" "OPENGL DEBUG"
    "\n"
    "\n" "ID:       %u"
    "\n" "SOURCE:   %s"
    "\n" "SEVERITY: %s"
    "\n" "TYPE:     %s"
    "\n" "MESSAGE:  %s"
    "\n",
    id,
    GL_ERROR_SOURCE[source - GL_DEBUG_SOURCE_API],
    GL_ERROR_SEVERITY[
      severity != GL_DEBUG_SEVERITY_NOTIFICATION ?
      severity  - GL_DEBUG_SEVERITY_HIGH : 3
    ],
    GL_ERROR_TYPE[type - GL_DEBUG_TYPE_ERROR],
    message
  );
  // clang-format on
}

void gfCheckExtensions(
    int32_t extensions_count, const char * _Nonnull * _Nonnull extensions)
{
  forcount(i, (size_t)extensions_count)
  {
    if (!SDL_GL_ExtensionSupported(extensions[i]))
      SDL_ShowSimpleMessageBox(
          SDL_MESSAGEBOX_ERROR, "ERROR: Unsupported OpenGL Extension",
          extensions[i], NULL);
  }
}

uint32_t gfWindow(
    SDL_Window * _Nonnull * _Nullable sdl_window,
    void * _Nonnull * _Nullable sdl_glcontext, uint32_t sdl_init_flags,
    uint32_t sdl_window_flags, const char * _Nonnull window_title,
    int32_t window_width, int32_t window_height, int32_t msaa_samples)
{
  if (SDL_Init(SDL_INIT_VIDEO | sdl_init_flags))
  {
    error("SDLInitError");
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msaa_samples);
  SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);

  *sdl_window = SDL_CreateWindow(
      window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      window_width, window_height, SDL_WINDOW_OPENGL | sdl_window_flags);

  if (*sdl_window == NULL)
  {
    error("SDLCreateWindowError");
  }

  *sdl_glcontext = SDL_GL_CreateContext(*sdl_window);

  if (*sdl_glcontext == NULL)
  {
    error("SDLGLCreateContextError");
  }

  if (SDL_GL_LoadLibrary(NULL))
  {
    error("SDLGLLoadLibraryError");
  }

  // clang-format off
  const c8 * extensions[] =
  {
#ifndef RELEASE
    "GL_KHR_debug",
#endif
    "GL_ARB_multi_bind",
    "GL_ARB_gpu_shader5",
    "GL_ARB_base_instance",
    "GL_ARB_buffer_storage",
    "GL_ARB_texture_storage",
    "GL_ARB_gpu_shader_fp64",
    "GL_ARB_shader_precision",
    "GL_ARB_conservative_depth",
    "GL_ARB_direct_state_access",
    "GL_ARB_get_texture_sub_image",
    "GL_ARB_texture_cube_map_array",
    "GL_ARB_separate_shader_objects",
    "GL_ARB_shading_language_420pack",
    "GL_ARB_shading_language_packing",
    "GL_ARB_explicit_uniform_location",
    "GL_EXT_texture_filter_anisotropic",
    "GL_ARB_texture_buffer_object_rgb32"
  };

  gfCheckExtensions(sizeof(extensions) / sizeof(extensions[0]), extensions);

  glBindFramebuffer = SDL_GL_GetProcAddress("glBindFramebuffer");
  glBindProgramPipeline = SDL_GL_GetProcAddress("glBindProgramPipeline");
  glBindSamplers = SDL_GL_GetProcAddress("glBindSamplers");
  glBindTextures = SDL_GL_GetProcAddress("glBindTextures");
  glClear = SDL_GL_GetProcAddress("glClear");
  glCreateBuffers = SDL_GL_GetProcAddress("glCreateBuffers");
  glCreateFramebuffers = SDL_GL_GetProcAddress("glCreateFramebuffers");
  glCreateProgramPipelines = SDL_GL_GetProcAddress("glCreateProgramPipelines");
  glCreateSamplers = SDL_GL_GetProcAddress("glCreateSamplers");
  glCreateShaderProgramv = SDL_GL_GetProcAddress("glCreateShaderProgramv");
  glCreateTextures = SDL_GL_GetProcAddress("glCreateTextures");
  glDeleteBuffers = SDL_GL_GetProcAddress("glDeleteBuffers");
  glDeleteFramebuffers = SDL_GL_GetProcAddress("glDeleteFramebuffers");
  glDeleteProgram = SDL_GL_GetProcAddress("glDeleteProgram");
  glDeleteProgramPipelines = SDL_GL_GetProcAddress("glDeleteProgramPipelines");
  glDeleteSamplers = SDL_GL_GetProcAddress("glDeleteSamplers");
  glDeleteTextures = SDL_GL_GetProcAddress("glDeleteTextures");
  glDisable = SDL_GL_GetProcAddress("glDisable");
  glDrawArraysInstancedBaseInstance = SDL_GL_GetProcAddress("glDrawArraysInstancedBaseInstance");
  glEnable = SDL_GL_GetProcAddress("glEnable");
  glFinish = SDL_GL_GetProcAddress("glFinish");
  glGenerateTextureMipmap = SDL_GL_GetProcAddress("glGenerateTextureMipmap");
  glGetTextureSubImage = SDL_GL_GetProcAddress("glGetTextureSubImage");
  glMapNamedBufferRange = SDL_GL_GetProcAddress("glMapNamedBufferRange");
  glNamedBufferStorage = SDL_GL_GetProcAddress("glNamedBufferStorage");
  glNamedFramebufferDrawBuffers = SDL_GL_GetProcAddress("glNamedFramebufferDrawBuffers");
  glNamedFramebufferTextureLayer = SDL_GL_GetProcAddress("glNamedFramebufferTextureLayer");
  glProgramUniform1dv = SDL_GL_GetProcAddress("glProgramUniform1dv");
  glProgramUniform1fv = SDL_GL_GetProcAddress("glProgramUniform1fv");
  glProgramUniform1iv = SDL_GL_GetProcAddress("glProgramUniform1iv");
  glProgramUniform1uiv = SDL_GL_GetProcAddress("glProgramUniform1uiv");
  glProgramUniform2fv = SDL_GL_GetProcAddress("glProgramUniform2fv");
  glProgramUniform3fv = SDL_GL_GetProcAddress("glProgramUniform3fv");
  glProgramUniform4fv = SDL_GL_GetProcAddress("glProgramUniform4fv");
  glSamplerParameteri = SDL_GL_GetProcAddress("glSamplerParameteri");
  glTextureBuffer = SDL_GL_GetProcAddress("glTextureBuffer");
  glTextureStorage3D = SDL_GL_GetProcAddress("glTextureStorage3D");
  glTextureSubImage3D = SDL_GL_GetProcAddress("glTextureSubImage3D");
  glUseProgramStages = SDL_GL_GetProcAddress("glUseProgramStages");
  glViewport = SDL_GL_GetProcAddress("glViewport");
// clang-format on

#ifndef RELEASE
  glEnable(0x92E0);
  glEnable(0x8242);
  typedef void (*GLDEBUGPROC)(
      u32 source, u32 type, u32 id, u32 severity, i32 length,
      const c8 * message, const void * userParam);
  void (*glDebugMessageCallback)(GLDEBUGPROC callback, const void * userParam) =
      SDL_GL_GetProcAddress("glDebugMessageCallback");
  glDebugMessageCallback((GLDEBUGPROC)gfDebugCallback, NULL);
#endif

  void (*glBlendFunc)(u32 sfactor, u32 dfactor) =
      SDL_GL_GetProcAddress("glBlendFunc");
  void (*glDepthRange)(f64 nearVal, f64 farVal) =
      SDL_GL_GetProcAddress("glDepthRange");
  void (*glCreateVertexArrays)(i32 n, u32 * arrays) =
      SDL_GL_GetProcAddress("glCreateVertexArrays");
  void (*glBindVertexArray)(u32 array) =
      SDL_GL_GetProcAddress("glBindVertexArray");

  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
  glEnable(GL_FRAMEBUFFER_SRGB);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);

  glBlendFunc(0x0302, 0x0303);
  glDepthRange(-1.0, 1.0);

  u32 vao = 0;
  glCreateVertexArrays(1, &vao);
  glBindVertexArray(vao);

  return 0;
}
