#include "Math.h"
using namespace osomi;

bool Math::intersect(float a0, float a1, float b0, float b1) {
	return a0 < b1 && a1 > b0;
}
float Math::clamp(float f, float min, float max) {
	if (f < min)f = min;
	if (f > max)f = max;
	return f;
}
float Math::min(float a, float b) {
	if (a < b)return a;
	return b;
}
float Math::max(float a, float b) {
	if (a > b)return a;
	return b;
}
bool Math::doesIntersect(vec3 mi0, vec3 ma0, vec3 mi1, vec3 ma1) {
	return intersect(mi0.x, ma0.x, mi1.x, ma1.x) && intersect(mi0.y, ma0.y, mi1.y, ma1.y) && intersect(mi0.z, ma0.z, mi1.z, ma1.z);
}
bool Math::doesIntersect2D(vec3 mi0, vec3 ma0, vec3 mi1, vec3 ma1) {
	return intersect(mi0.x, ma0.x, mi1.x, ma1.x) && intersect(mi0.z, ma0.z, mi1.z, ma1.z);
}
float Math::mod(float a, float b) {
	float fn = floor(a / b) * b;
	return a - fn;
}