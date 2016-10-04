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
out gl_PerVertex { vec4 gl_Position; };

vec4 qconj(vec4 q)         { return vec4(-q.xyz, q.w); }
vec3 qrot(vec3 v, vec4 q)  { return fma(cross(q.xyz, fma(v, vec3(q.w), cross(q.xyz, v))), vec3(2.0), v); }
vec4 proj(vec3 mv, vec4 p) { return vec4(mv.xy * p.xy, fma(mv.z, p.z, p.w), -mv.z); }

const vec3 cube[] = vec3[]
(
  vec3( -1.0,  1.0, -1.0 ),
  vec3( -1.0, -1.0, -1.0 ),
  vec3(  1.0, -1.0, -1.0 ),
  vec3(  1.0, -1.0, -1.0 ),
  vec3(  1.0,  1.0, -1.0 ),
  vec3( -1.0,  1.0, -1.0 ),

  vec3( -1.0, -1.0,  1.0 ),
  vec3( -1.0, -1.0, -1.0 ),
  vec3( -1.0,  1.0, -1.0 ),
  vec3( -1.0,  1.0, -1.0 ),
  vec3( -1.0,  1.0,  1.0 ),
  vec3( -1.0, -1.0,  1.0 ),

  vec3(  1.0, -1.0, -1.0 ),
  vec3(  1.0, -1.0,  1.0 ),
  vec3(  1.0,  1.0,  1.0 ),
  vec3(  1.0,  1.0,  1.0 ),
  vec3(  1.0,  1.0, -1.0 ),
  vec3(  1.0, -1.0, -1.0 ),

  vec3( -1.0, -1.0,  1.0 ),
  vec3( -1.0,  1.0,  1.0 ),
  vec3(  1.0,  1.0,  1.0 ),
  vec3(  1.0,  1.0,  1.0 ),
  vec3(  1.0, -1.0,  1.0 ),
  vec3( -1.0, -1.0,  1.0 ),

  vec3( -1.0,  1.0, -1.0 ),
  vec3(  1.0,  1.0, -1.0 ),
  vec3(  1.0,  1.0,  1.0 ),
  vec3(  1.0,  1.0,  1.0 ),
  vec3( -1.0,  1.0,  1.0 ),
  vec3( -1.0,  1.0, -1.0 ),

  vec3( -1.0, -1.0, -1.0 ),
  vec3( -1.0, -1.0,  1.0 ),
  vec3(  1.0, -1.0, -1.0 ),
  vec3(  1.0, -1.0, -1.0 ),
  vec3( -1.0, -1.0,  1.0 ),
  vec3(  1.0, -1.0,  1.0 )
);

layout(location = 0) uniform vec4 cam_rot;
layout(location = 1) uniform vec4 cam_prj;

layout(location = 0) smooth out vec3 pos;

void main()
{
  pos = cube[gl_VertexID];

  vec3 mv = pos;
  mv = qrot(mv, qconj(cam_rot));
  gl_Position = proj(mv, cam_prj);
}
