#version 330 core

in VertexData {
  vec3 normal;
  vec2 uv;
} i;

out vec4 color;

void main() {
  float xe = (i.uv.x > 0.5) ? 1 - i.uv.x : i.uv.x;
  float ye = (i.uv.y > 0.5) ? 1 - i.uv.y : i.uv.y;
  float edgeMul = 0.05/xe * 0.05/ye;
  color.xyz = vec3(1,1,1)*edgeMul + i.normal;
  if(color.x < 0) color.x = - color.x;
  if(color.y < 0) color.y = - color.y;
  if(color.z < 0) color.z = - color.z;
  color.w = 1;
}
