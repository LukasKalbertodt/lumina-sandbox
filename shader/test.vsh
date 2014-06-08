#version 330 core

layout(location = 0) in vec3 i_pos3D;
layout(location = 1) in vec3 i_normal;

out VertexData {
  vec3 normal;
} o;

void main() {
  gl_Position.xyz = i_pos3D ;//* color.x;
  gl_Position.w = 1.0;
  o.normal = i_normal;
}