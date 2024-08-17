#version 460 core 
in vec3 color;
in vec2 tex_coord;
uniform sampler2D texture1;

out vec4 frag_color;
void main() {
	
   frag_color = texture(texture1, tex_coord);
}