#version 330 core

in VertexData {
  vec3 normal;
  vec2 uv;
} i;


uniform sampler2D tex0;

uniform float mul;

out vec4 o_color;
 
void main(){
  // o_color.xyz = i.color;
  // o_color.xyz = i.uv.xyx * mul;
  // o_color.xyz = i.normal.zzz * mul;
  // o_color.xyz = vec3(mul, mul, mul) * (i.normal.x + i.normal.y + 0.5);
  // o_color.xyz = vec3(mul, mul, mul);
  o_color.xyz = texture2D(tex0, i.uv).xyz * mul;
  o_color.w = 1.0;
}