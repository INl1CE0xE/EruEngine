#version 460 core 
layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 vertex_color;
out vec3 color;
uniform float xset;
void main() {
   color = vertex_color;
   gl_Position = vec4(v_pos.x + xset, v_pos.y, v_pos.z , 1.0);
   if (v_pos + xset == 1){
		gl_Position = vec4(v_pos.x - xset, v_pos.y, v_pos.z , 1.0);
   }
}
