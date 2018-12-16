#version 450 core

in vec2 uv;
in vec3 normal;
in vec3 cpos;

out layout(location=0) vec4 color;

uniform sampler2D t;

uniform vec3 ambient;

uniform vec3 fogColor;
uniform float fogDensity;

void main(){
 vec3 light = texture(t, uv).rgb;
 light = ambient * light;
 
 if(fogDensity > 0){
  float d = length(cpos);
  float fogFactor = clamp(1.0 / exp((d * fogDensity) * (d * fogDensity)), 0, 1);
  color = vec4(mix(fogColor, light, fogFactor), 1);
 }else{
  color = vec4(light, 1);
 }
}