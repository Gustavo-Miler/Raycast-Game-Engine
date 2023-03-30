#pragma once
#include <SFML/Graphics.hpp>
#include "Tiles.hpp"

class Raycaster {
public:
	Raycaster();
	void run(sf::Vector2f startPoint, sf::Vector2f direction, float maxDistance, Tiles tile);
	float getDistance();
	bool hasHit();
	bool hasHitHorizontal();
	sf::Vector2f getIntersection();

private:
	bool hasHitTile;
	bool horizontalHit;
	float distance;
	sf::Vector2f intersection;
};