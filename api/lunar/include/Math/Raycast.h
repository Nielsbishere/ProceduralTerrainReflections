namespace osomi {
	class vec3;
	class Raycast {
	public:
		static bool intersect(vec3 min, vec3 max, vec3 s, vec3 d);
	};
}