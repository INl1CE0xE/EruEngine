#version 460 core 
layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 v_tex;
out vec3 color;
out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
   color = vertex_color;
   gl_Position = projection * view * model * vec4(v_pos, 1.0);
   tex_coord = v_tex;
}
