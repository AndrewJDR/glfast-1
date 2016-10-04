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

layout(binding = 0) uniform  samplerBuffer s_mesh_data;
layout(binding = 1) uniform isamplerBuffer s_mesh_id;
layout(binding = 2) uniform isamplerBuffer s_ins_first;
layout(binding = 3) uniform  samplerBuffer s_ins_pos;

layout(location = 0) uniform vec3  cam_pos;
layout(location = 1) uniform vec4  cam_rot;
layout(location = 2) uniform vec4  cam_prj;
layout(location = 3) uniform float fcoef;
layout(location = 4) uniform float fnear;

layout(location = 0) flat   out int   id;
layout(location = 1) smooth out vec2  uv;
layout(location = 2) smooth out vec3  nor;
layout(location = 3) smooth out vec3  pos;
layout(location = 4) smooth out float flogz;

void main()
{
  id = texelFetch(s_mesh_id, gl_VertexID).x;

  vec4 mesh_data_1 = texelFetch(s_mesh_data, gl_VertexID * 2 + 0);
  vec4 mesh_data_2 = texelFetch(s_mesh_data, gl_VertexID * 2 + 1);

  pos = mesh_data_1.xyz;
  nor = mesh_data_2.xyz;
  uv  = vec2(mesh_data_1.w, mesh_data_2.w);

  int  ins_first = texelFetch(s_ins_first, id).x;
  vec3 ins_pos   = texelFetch(s_ins_pos, ins_first + gl_InstanceID).xyz;

  pos += ins_pos;

  vec3 mv = pos;
  mv -= cam_pos;
  mv  = qrot(mv, qconj(cam_rot));
  gl_Position = proj(mv, cam_prj);

  gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w)) * fcoef - fnear;
  flogz = 1.0 + gl_Position.w;
}
