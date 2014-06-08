#version 330 core

in VertexData {
  vec3 normal;
} i;

in vec4 gl_FragCoord;

uniform sampler2D tex0;

uniform float mul;

out vec4 o_color;
 
void main(){
  o_color.xyz = i.normal * mul;
  // o_color.xyz = vec3(mul, mul, mul) * (i.normal.x + i.normal.y + 0.5);
  // o_color.xyz = vec3(mul, mul, mul);
  // o_color.xyz = texture2D(tex0, i.normal).xyz * mul;
  o_color.w = 1.0;
}