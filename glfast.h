#pragma once

#include "required.h"

uint32_t gfWindow(
    SDL_Window * _Nonnull * _Nullable sdl_window,
    void * _Nonnull * _Nullable sdl_glcontext, uint32_t sdl_init_flags,
    uint32_t sdl_window_flags, const char * _Nonnull window_title,
    int32_t window_width, int32_t window_height, int32_t msaa_samples);

void gfCheckExtensions(
    int32_t extensions_count, const char * _Nonnull * _Nonnull extensions);

void gfDebugCallback(
    uint32_t source, uint32_t type, uint32_t id, uint32_t severity,
    int32_t length, const char * _Null_unspecified message,
    void * _Nullable userdata);

void gfDraw(int32_t gpu_cmd_count, const struct gpu_cmd_t * _Nonnull gpu_cmd);

uint32_t gfSaveTextureToBmp(
    uint32_t texture_id, int32_t texture_layer, int32_t width, int32_t height,
    const char * _Nonnull bmp_filepath);

void gfGetTexturePixels(
    uint32_t texture_id, int32_t texture_layer, int32_t x, int32_t y,
    int32_t width, int32_t height, uint32_t pixels_format, uint32_t pixels_type,
    int32_t pixels_bytes, void * _Nonnull pixels);

void gfCreateProgramPipeline(
    uint32_t vert_program_id, uint32_t frag_program_id,
    uint32_t * _Nonnull ppo);

void gfCreateProgramFromString(
    uint32_t shader_type, const char * _Nonnull shader_string,
    uint32_t * _Nonnull program);

uint32_t gfCreateProgramFromFile(
    uint32_t shader_type, const char * _Nonnull shader_filepath,
    uint32_t * _Nonnull program);

void gfCreateFbo(
    uint32_t depth_texture_id_0, int32_t depth_texture_layer_0,
    uint32_t color_texture_id_0, int32_t color_texture_layer_0,
    uint32_t color_texture_id_1, int32_t color_texture_layer_1,
    uint32_t color_texture_id_2, int32_t color_texture_layer_2,
    uint32_t color_texture_id_3, int32_t color_texture_layer_3,
    uint32_t * _Nonnull fbo);

void gfCreateSampler(
    int32_t anisotropy_level, int32_t min_filter, int32_t mag_filter,
    int32_t s_wrapping, int32_t t_wrapping, int32_t r_wrapping,
    struct gpu_sampler_t * _Nonnull sampler);

uint32_t gfCreateCubemapFromBmp(
    int32_t width, int32_t height, int32_t mipmap_level, int32_t cubemap_count,
    const char * _Nonnull * _Nonnull pos_x_texture_paths,
    const char * _Nonnull * _Nonnull neg_x_texture_paths,
    const char * _Nonnull * _Nonnull pos_y_texture_paths,
    const char * _Nonnull * _Nonnull neg_y_texture_paths,
    const char * _Nonnull * _Nonnull pos_z_texture_paths,
    const char * _Nonnull * _Nonnull neg_z_texture_paths,
    struct gpu_texture_t * _Nonnull texture,
    int8_t * _Nullable error_texture_id,
    size_t * _Nullable error_texture_index);

void gfSetTexturePixels(
    uint32_t texture_id, int32_t texture_layer, int32_t x, int32_t y,
    int32_t width, int32_t height, uint32_t pixels_format, uint32_t pixels_type,
    const void * _Nonnull pixels);

uint32_t gfCreateTextureFromBmp(
    int32_t width, int32_t height, int32_t mipmap_level, int32_t texture_count,
    const char * _Nonnull * _Nonnull texture_paths,
    struct gpu_texture_t * _Nonnull texture,
    size_t * _Nullable error_texture_index);

void gfCreateTexture(
    int32_t width, int32_t height, int32_t mipmap_level, int32_t count,
    uint32_t format, uint8_t is_cubemap,
    struct gpu_texture_t * _Nonnull texture);

uint32_t gfCreateArray(
    int32_t count, uint32_t format, struct gpu_array_t * _Nonnull tbo);
