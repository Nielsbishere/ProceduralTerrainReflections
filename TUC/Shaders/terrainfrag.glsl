#version 450 core
#define LIGHTS 16

out layout(location=0) vec4 color;

in vec2 uv;
in vec3 n;
in vec3 wpos;
in vec3 cpos;

uniform sampler2D t;
uniform sampler2D t1;
uniform sampler2D t2;

uniform vec3 ambient;
uniform vec3 eye;
struct Light {
    vec3 position;
	vec3 color;
	float power;
};
uniform Light lights[LIGHTS];
uniform int actualLights;

uniform vec3 fogColor;
uniform float fogDensity;

uniform vec3 sunLight=vec3(0,-1,0);
uniform vec3 sunColor=vec3(1,1,1);
uniform float sunIntensity=1;

void main(){
 vec3 col = vec3(1, 1, 1);

 if(wpos.y > 15){
	col = texture(t2, uv).rgb;
 }else if(wpos.y > 10){
	float perc = (wpos.y - 10) / 5;
	col = (1 - perc) * texture(t1, uv).rgb + perc * texture(t2, uv).rgb;
 }else if(wpos.y > 8){
	col = texture(t1, uv).rgb;
 }else if(wpos.y > 0){
	float perc = wpos.y / 8;
	col = (1 - perc) * texture(t, uv).rgb + perc * texture(t1, uv).rgb;
 }else{
	col = texture(t, uv).rgb;
 }

 vec3 computedLight;
 int lightAm = min(actualLights, LIGHTS);
 for(int i = 0; i < lightAm + 1; i++){
  if(i == lightAm){
	vec3 ldir = normalize(-sunLight);
    float cosTheta = clamp(dot(ldir, normalize(n)), 0, 1);
//  float reflected = ref * pow(clamp(dot(reflect(-ldir, n), normalize(eye - wpos)), 0, 1), 9);
	computedLight = computedLight + cosTheta * sunColor * sunIntensity;
  }else{
    vec3 ldir = normalize(lights[i].position - wpos);
    float cosTheta = clamp(dot(ldir, normalize(n)), 0, 1);
    float d = length(lights[i].position - wpos);
//  float reflected = ref * pow(clamp(dot(reflect(-ldir, n), normalize(eye - wpos)), 0, 1), 9);
    computedLight = computedLight + cosTheta * lights[i].color * lights[i].power / d / d;
  }
 }
 computedLight = computedLight / (lightAm+1);
 vec3 light = computedLight * col + ambient * col;

 //Fog calculation
 if(fogDensity <= 0){
  color = vec4(light, 1);
 }else{
   float d = length(cpos);
   float fogFactor = clamp(1.0 / exp((d * fogDensity) * (d * fogDensity)), 0, 1);
   color = vec4(mix(fogColor, light, fogFactor), 1);
 }
}