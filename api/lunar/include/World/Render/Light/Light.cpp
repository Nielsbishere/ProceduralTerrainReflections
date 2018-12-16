#include "../../../Math/Math.h"
#include "Light.h"

#include <GL/glew.h>

using namespace osomi;

Light::Light(vec3 _pos, vec3 _col, float pow) : power(pow) {
	pos = _pos;
	col = _col;
	power = pow;
}
vec3 Light::getPosition() {
	return pos;
}
void Light::setPosition(vec3 p) {
	pos = p;
}
void Light::move(vec3 m) {
	setPosition(pos + m);
}
void Light::setColor(vec3 _col) {
	col = _col;
}
vec3 Light::getColor() {
	return col;
}
float Light::getPower() {
	return power;
}