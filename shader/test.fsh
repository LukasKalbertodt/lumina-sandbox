#version 330 core

in VertexData {
  vec3 normal;
  vec2 uv;
} i;


uniform sampler2D tex0;

// uniform float mul;

out vec4 o_color;
 
void main(){
  o_color.xyz = texture(tex0, i.uv).xyz;
  o_color.w = 1.0;
}