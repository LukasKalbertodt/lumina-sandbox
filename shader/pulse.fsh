#version 330 core

in VertexData {
  vec3 color;
} i;

uniform float mul;

out vec4 o_color;
 
void main(){
  o_color = vec4(mul, mul, mul, 1.0);
}