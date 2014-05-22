#version 330 core

in VertexData {
  vec3 color;
} i;

uniform float mul;

out vec4 o_color;
 
void main(){
  o_color.xyz = i.color * mul;
  o_color.w = 1.0;
}