#pragma once
#include "Types/mat4.h"
#include "Types/vec4.h"
#include "Types/vec3.h"
#include "Types/vec2.h"

#include "Noise/SimplexNoise.h"

#include "Raycast.h"

namespace osomi {
	class Math {
	private:
		static bool intersect(float a0, float a1, float b0, float b1);
	public:
		//Clamp value between min and max
		static float clamp(float f, float min, float max);

		//Picks the lowest number
		static float min(float a, float b);

		//Picks the highest number
		static float max(float a, float b);

		//Picks the modulus of a floating point division
		static float mod(float a, float b);

		//Checks if 3d aabb's collide
		static bool doesIntersect(vec3 mi0, vec3 ma0, vec3 mi1, vec3 ma1);

		//Checks if 2d aabb's collide (CHECKED FROM ABOVE; X,Z are compared! NOT X,Y!)
		static bool doesIntersect2D(vec3 mi0, vec3 ma0, vec3 mi1, vec3 ma1);
	};
}