#version 460 core 
in vec3 color;
in vec2 tex_coord;
uniform sampler2D ourTexture;
out vec4 frag_color;
void main() {
   frag_color = texture(ourTexture, tex_coord);
}