#version 430 core
in layout(location=0) vec3 pos;
in layout(location=1) vec2 vuv;
in layout(location=2) vec3 vn;

out vec2 uv;
out vec3 normal;
out vec3 cpos;

uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;

void main(){
 gl_Position = MVP * vec4(pos, 1);
 cpos = vec3(view * model * vec4(pos, 1));
 uv = vuv;
 normal = vn;
}