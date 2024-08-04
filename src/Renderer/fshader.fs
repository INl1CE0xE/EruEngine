#version 460 core 
in vec3 color;
in vec2 tex_coord;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;
out vec4 frag_color;
void main() {
   frag_color = mix(texture(texture1, tex_coord), texture(texture2, tex_coord)* vec4(color, 1.0), mixValue);
}