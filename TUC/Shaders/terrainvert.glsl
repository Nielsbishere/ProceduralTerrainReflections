#version 450 core
in layout(location=0) vec3 pos;
in layout(location=1) vec2 vuv;
in layout(location=2) vec3 normal;

out vec2 uv;
out vec3 n;
out vec3 wpos;
out vec3 cpos;

uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;

uniform vec4 plane=vec4(9000.1, 9000.1, 9000.1, 9000.1);

void main(){
 gl_Position = MVP * vec4(pos, 1);
 wpos = vec3(model * vec4(pos,1));
 cpos = vec3(view * model * vec4(pos, 1));
 n = normalize(vec3(model * vec4(normal, 0)));
 uv = vuv;

 //Clipping
 if(plane != vec4(9000.1, 9000.1, 9000.1, 9000.1)){
  float dist = dot(vec3(plane), wpos) + plane.w;
  gl_CullDistance[0] = dist;
 }
}