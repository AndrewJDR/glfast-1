#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

void (*glBindFramebuffer)(uint32_t target, uint32_t framebuffer);

void (*glBindProgramPipeline)(uint32_t pipeline);

void (*glBindSamplers)(
    uint32_t first, int32_t count, const uint32_t * samplers);

void (*glBindTextures)(
    uint32_t first, int32_t count, const uint32_t * textures);

void (*glClear)(uint32_t mask);

void (*glCreateBuffers)(int32_t n, uint32_t * buffers);

void (*glCreateFramebuffers)(int32_t n, uint32_t * framebuffers);

void (*glCreateProgramPipelines)(int32_t n, uint32_t * pipelines);

void (*glCreateSamplers)(int32_t n, uint32_t * samplers);

uint32_t (*glCreateShaderProgramv)(
    uint32_t type, int32_t count, const char ** strings);

void (*glCreateTextures)(uint32_t target, int32_t n, uint32_t * textures);

void (*glDeleteBuffers)(int32_t n, const uint32_t * buffers);

void (*glDeleteFramebuffers)(int32_t n, const uint32_t * framebuffers);

void (*glDeleteProgram)(uint32_t program);

void (*glDeleteProgramPipelines)(int32_t n, const uint32_t * pipelines);

void (*glDeleteSamplers)(int32_t n, const uint32_t * samplers);

void (*glDeleteTextures)(int32_t n, const uint32_t * textures);

void (*glDisable)(uint32_t cap);

void (*glDrawArraysInstancedBaseInstance)(
    uint32_t mode, int32_t first, int32_t count, int32_t instancecount,
    uint32_t baseinstance);

void (*glEnable)(uint32_t cap);

void (*glFinish)();

void (*glGenerateTextureMipmap)(uint32_t texture);

void (*glGetTextureSubImage)(
    uint32_t texture, int32_t level, int32_t xoffset, int32_t yoffset,
    int32_t zoffset, int32_t width, int32_t height, int32_t depth,
    uint32_t format, uint32_t type, int32_t bufSize, void * pixels);

void * (*glMapNamedBufferRange)(
    uint32_t buffer, int32_t offset, int32_t length, uint32_t access);

void (*glNamedBufferStorage)(
    uint32_t buffer, int32_t size, const void * data, uint32_t flags);

void (*glNamedFramebufferDrawBuffers)(
    uint32_t framebuffer, int32_t n, const uint32_t * bufs);

void (*glNamedFramebufferTextureLayer)(
    uint32_t framebuffer, uint32_t attachment, uint32_t texture, int32_t level,
    int32_t layer);

void (*glProgramUniform1dv)(
    uint32_t program, int32_t location, int32_t count, const double * value);

void (*glProgramUniform1fv)(
    uint32_t program, int32_t location, int32_t count, const float * value);

void (*glProgramUniform1iv)(
    uint32_t program, int32_t location, int32_t count, const int32_t * value);

void (*glProgramUniform1uiv)(
    uint32_t program, int32_t location, int32_t count, const uint32_t * value);

void (*glProgramUniform2fv)(
    uint32_t program, int32_t location, int32_t count, const float * value);

void (*glProgramUniform3fv)(
    uint32_t program, int32_t location, int32_t count, const float * value);

void (*glProgramUniform4fv)(
    uint32_t program, int32_t location, int32_t count, const float * value);

void (*glSamplerParameteri)(uint32_t sampler, uint32_t pname, int32_t param);

void (*glTextureBuffer)(
    uint32_t texture, uint32_t internalformat, uint32_t buffer);

void (*glTextureStorage3D)(
    uint32_t texture, int32_t levels, uint32_t internalformat, int32_t width,
    int32_t height, int32_t depth);

void (*glTextureSubImage3D)(
    uint32_t texture, int32_t level, int32_t xoffset, int32_t yoffset,
    int32_t zoffset, int32_t width, int32_t height, int32_t depth,
    uint32_t format, uint32_t type, const void * pixels);

void (*glUseProgramStages)(
    uint32_t pipeline, uint32_t stages, uint32_t program);

void (*glViewport)(int32_t x, int32_t y, int32_t width, int32_t height);

#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1
#define GL_BLEND 0x0BE2
#define GL_BYTE 0x1400
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_CULL_FACE 0x0B44
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7
#define GL_DEPTH_COMPONENT32F 0x8CAC
#define GL_DEPTH_TEST 0x0B71
#define GL_FLOAT 0x1406
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_FRAGMENT_SHADER_BIT 0x00000002
#define GL_FRAMEBUFFER 0x8D40
#define GL_FRAMEBUFFER_SRGB 0x8DB9
#define GL_INT 0x1404
#define GL_LINEAR 0x2601
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_MAP_COHERENT_BIT 0x0080
#define GL_MAP_PERSISTENT_BIT 0x0040
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MIRRORED_REPEAT 0x8370
#define GL_NEAREST 0x2600
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_R16F 0x822D
#define GL_R16I 0x8233
#define GL_R16UI 0x8234
#define GL_R32F 0x822E
#define GL_R32I 0x8235
#define GL_R32UI 0x8236
#define GL_R8 0x8229
#define GL_R8I 0x8231
#define GL_R8UI 0x8232
#define GL_REPEAT 0x2901
#define GL_RG16F 0x822F
#define GL_RG16I 0x8239
#define GL_RG16UI 0x823A
#define GL_RG32F 0x8230
#define GL_RG32I 0x823B
#define GL_RG32UI 0x823C
#define GL_RG8 0x822B
#define GL_RG8I 0x8237
#define GL_RG8UI 0x8238
#define GL_RGB 0x1907
#define GL_RGB16F 0x881B
#define GL_RGB32F 0x8815
#define GL_RGB32I 0x8D83
#define GL_RGB32UI 0x8D71
#define GL_RGB8 0x8051
#define GL_RGBA 0x1908
#define GL_RGBA16F 0x881A
#define GL_RGBA16I 0x8D88
#define GL_RGBA16UI 0x8D76
#define GL_RGBA32F 0x8814
#define GL_RGBA32I 0x8D82
#define GL_RGBA32UI 0x8D70
#define GL_RGBA8 0x8058
#define GL_RGBA8I 0x8D8E
#define GL_RGBA8UI 0x8D7C
#define GL_SHORT 0x1402
#define GL_SRGB8 0x8C41
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MAX_ANISOTROPY 0x84FE
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TRIANGLES 0x0004
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_INT 0x1405
#define GL_UNSIGNED_SHORT 0x1403
#define GL_VERTEX_SHADER 0x8B31
#define GL_VERTEX_SHADER_BIT 0x00000001

#define x_b8 GL_R8
#define x_f16 GL_R16F
#define x_f32 GL_R32F
#define x_i8 GL_R8I
#define x_i16 GL_R16I
#define x_i32 GL_R32I
#define x_u8 GL_R8UI
#define x_u16 GL_R16UI
#define x_u32 GL_R32UI
#define xy_b8 GL_RG8
#define xy_f16 GL_RG16F
#define xy_f32 GL_RG32F
#define xy_i8 GL_RG8I
#define xy_i16 GL_RG16I
#define xy_i32 GL_RG32I
#define xy_u8 GL_RG8UI
#define xy_u16 GL_RG16UI
#define xy_u32 GL_RG32UI
#define xyz_f32 GL_RGB32F
#define xyz_i32 GL_RGB32I
#define xyz_u32 GL_RGB32UI
#define xyzw_b8 GL_RGBA8
#define xyzw_f16 GL_RGBA16F
#define xyzw_f32 GL_RGBA32F
#define xyzw_i8 GL_RGBA8I
#define xyzw_i16 GL_RGBA16I
#define xyzw_i32 GL_RGBA32I
#define xyzw_u8 GL_RGBA8UI
#define xyzw_u16 GL_RGBA16UI
#define xyzw_u32 GL_RGBA32UI

#define rgb_b8 GL_RGB8
#define rgba_b8 GL_RGBA8
#define srgb_b8 GL_SRGB8
#define srgba_b8 GL_SRGB8_ALPHA8
#define rgba_f16 GL_RGBA16F
#define rgba_f32 GL_RGBA32F
#define depth_b16 GL_DEPTH_COMPONENT16
#define depth_b24 GL_DEPTH_COMPONENT24
#define depth_b32 GL_DEPTH_COMPONENT32
#define depth_f32 GL_DEPTH_COMPONENT32F

#define GF_VERT_HEAD                                                           \
  "#version 330\n"                                                             \
  "#extension GL_ARB_gpu_shader5               : enable\n"                     \
  "#extension GL_ARB_gpu_shader_fp64           : enable\n"                     \
  "#extension GL_ARB_shader_precision          : enable\n"                     \
  "#extension GL_ARB_conservative_depth        : enable\n"                     \
  "#extension GL_ARB_texture_cube_map_array    : enable\n"                     \
  "#extension GL_ARB_separate_shader_objects   : enable\n"                     \
  "#extension GL_ARB_shading_language_420pack  : enable\n"                     \
  "#extension GL_ARB_shading_language_packing  : enable\n"                     \
  "#extension GL_ARB_explicit_uniform_location : enable\n"                     \
  "out gl_PerVertex { vec4 gl_Position; };\n"

#define GF_FRAG_HEAD                                                           \
  "#version 330\n"                                                             \
  "#extension GL_ARB_gpu_shader5               : enable\n"                     \
  "#extension GL_ARB_gpu_shader_fp64           : enable\n"                     \
  "#extension GL_ARB_shader_precision          : enable\n"                     \
  "#extension GL_ARB_conservative_depth        : enable\n"                     \
  "#extension GL_ARB_texture_cube_map_array    : enable\n"                     \
  "#extension GL_ARB_separate_shader_objects   : enable\n"                     \
  "#extension GL_ARB_shading_language_420pack  : enable\n"                     \
  "#extension GL_ARB_shading_language_packing  : enable\n"                     \
  "#extension GL_ARB_explicit_uniform_location : enable\n"                     \
  "layout(depth_less) out float gl_FragDepth;\n"

struct gpu_array_t
{
  uint8_t * ptr;
  int32_t bytes;
  int32_t count;
  uint32_t format;
  uint32_t buffer_id;
  uint32_t id;
};

struct gpu_texture_t
{
  int32_t w;
  int32_t h;
  int32_t mip;
  int32_t count;
  uint32_t format;
  uint8_t is_cubemap;
  uint32_t id;
};

struct gpu_sampler_t
{
  int32_t aniso;
  int32_t min;
  int32_t mag;
  int32_t s_wrap;
  int32_t t_wrap;
  int32_t r_wrap;
  uint32_t id;
};

struct gpu_cmd_t
{
  int32_t count;
  int32_t instance_count;
  int32_t first;
  int32_t instance_first;
};
