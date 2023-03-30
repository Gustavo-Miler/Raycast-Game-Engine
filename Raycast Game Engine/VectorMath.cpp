#include "VectorMath.hpp"

float degToRad(float x) {
	return x * PI / 180.0f;
}

float getMag(sf::Vector2f v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

void norm(sf::Vector2f& v) {
	if (getMag(v) != 0) {
		v.x /= getMag(v);
		v.y /= getMag(v);
	}
	else v = sf::Vector2f();
}