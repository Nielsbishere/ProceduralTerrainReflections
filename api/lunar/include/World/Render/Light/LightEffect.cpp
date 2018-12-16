#include "LightEffect.h"
#include "Light.h"

using namespace osomi;

void LightEffect::updateColor() {
	float perc = Math::clamp(timer / duration, 0.f, 1.f);
	if (pulse)perc = (float)abs(perc - 0.5) * 2;
	if (isLinear) current->setColor(first * (1 - perc) + last * perc);
	else {
		float nperc = pow(10.f, perc - 1) / 9 * 10;
		current->setColor(first * (1 - nperc) + last * nperc);
	}
}
LightEffect::LightEffect(Light *l, float _duration, vec3 start, vec3 end, bool linear, bool isPulsing) {
	duration = _duration;
	timer = 0;
	first = start;
	last = end;
	current = l;
	isLinear = linear;
	pulse = isPulsing;
}
bool LightEffect::update(float delta) {
	if (timer > duration) {
		timer = duration;
		updateColor();
		return true;
	}
	else if (timer == duration)return true;
	updateColor();
	timer += delta;
	return false;
}
Light *LightEffect::getLight() {
	return current;
}