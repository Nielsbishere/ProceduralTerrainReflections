#pragma once
typedef float GLfloat;
namespace osomi {
	class vec3;
	struct Light {
	private:
		vec3 pos, col;
		float power;
	public:
		Light(vec3 _pos, vec3 _col, float pow);
		vec3 getPosition();
		void setPosition(vec3 p);
		void move(vec3 m);
		GLfloat *getPos();
		GLfloat *getCol();
		void setColor(vec3 _col);
		vec3 getColor();
		float getPower();
	};
}