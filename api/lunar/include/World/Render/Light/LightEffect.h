#pragma once
#include "../../../Math/Math.h"
namespace osomi {
	class Light;
	class LightEffect {
	private:
		Light *current;
		float duration, timer;
		vec3 first, last;
		bool isLinear, pulse;
		void updateColor();
	public:
		LightEffect(Light *l, float _duration, vec3 start, vec3 end, bool linear = true, bool isPulsing = true);
		bool update(float delta);
		Light *getLight();
	};
}