#version 460 core 
layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 v_tex;
out vec3 color;
out vec2 tex_coord;
uniform float xset;
void main() {
   color = vertex_color;
   gl_Position = vec4(v_pos.x + xset, v_pos.y, v_pos.z , 1.0);
   tex_coord = v_tex;
}
