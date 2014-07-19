#version 330 core

in VertexData {
  vec3 normal;
  vec2 uv;
} i;


uniform sampler2D tex0;

// uniform float mul;

layout (location = 0) out vec4 o_color;
 
void main(){
  o_color.xyz = texture(tex0, i.uv).xyz + vec3(0.2, 0.2, 0.2);
  o_color.w = 1.0;
}