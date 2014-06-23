#version 330 core

in VertexData {
  vec3 color;
} i;

uniform float mul;
uniform mat4 matrix;
uniform vec3 tmp;

out vec4 o_color;
 
void main(){
  o_color = vec4(0.2f, 0.2f, 0.2f + mul/1000, 1.0) * matrix;
  o_color.xyz = o_color.xyz * 0.0001 + tmp;
}