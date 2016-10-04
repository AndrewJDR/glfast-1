#version 330
#extension GL_ARB_gpu_shader5               : enable
#extension GL_ARB_gpu_shader_fp64           : enable
#extension GL_ARB_shader_precision          : enable
#extension GL_ARB_conservative_depth        : enable
#extension GL_ARB_texture_cube_map_array    : enable
#extension GL_ARB_separate_shader_objects   : enable
#extension GL_ARB_shading_language_420pack  : enable
#extension GL_ARB_shading_language_packing  : enable
#extension GL_ARB_explicit_uniform_location : enable
layout(depth_less) out float gl_FragDepth;

int ColorToInt(vec3 c)
{
  int r = int(c.r * 255.0);
  int g = int(c.g * 255.0);
  int b = int(c.b * 255.0);
  return b + (g << 8) + (r << 16);
}

vec3 ScreenSpaceDither(vec2 vScreenPos)
{
  vec3 vDither = vec3(dot(vec2(171.0, 231.0), vScreenPos.xy));
  vDither.rgb = fract(vDither.rgb / vec3(103.0, 71.0, 97.0)) - vec3(0.5, 0.5, 0.5);
  return vDither.rgb / 255.0;
}

layout(binding = 6) uniform sampler2DArray s_fbo_color;

layout(location = 0) smooth in vec2 uv;

out vec4 color;

void main()
{
  color = texture(s_fbo_color, vec3(uv, 0));
  color += vec4(ScreenSpaceDither(gl_FragCoord.xy), 0);
}
