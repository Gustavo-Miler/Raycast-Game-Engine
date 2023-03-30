#include "Raycaster.hpp"
#include "VectorMath.hpp"
#include <iostream>

Raycaster::Raycaster() {
	hasHitTile = false;
	horizontalHit = false;
	distance = 0;
}

void Raycaster::run(sf::Vector2f startPoint, sf::Vector2f direction, float maxDistance, Tiles tile) {
	hasHitTile = false, distance = 0;

	sf::Vector2f unitStepSize(sqrt(1 + (direction.y / direction.x) * (direction.y / direction.x)), sqrt(1 + (direction.x / direction.y) * (direction.x / direction.y)));

	sf::Vector2i mapCheck = static_cast<sf::Vector2i>(startPoint);
	sf::Vector2f length1D;

	sf::Vector2i step;

	if (direction.x < 0) {
		step.x = -1;
		length1D.x = (startPoint.x - float(mapCheck.x)) * unitStepSize.x;
	}
	else {
		step.x = 1;
		length1D.x = (float(mapCheck.x + 1) - startPoint.x) * unitStepSize.x;
	}
	if (direction.y < 0) {
		step.y = -1;
		length1D.y = (startPoint.y - float(mapCheck.y)) * unitStepSize.y;
	}
	else {
		step.y = 1;
		length1D.y = (float(mapCheck.y + 1) - startPoint.y) * unitStepSize.y;
	}

	while (!hasHitTile && distance < maxDistance) {
		if (length1D.x < length1D.y) {
			mapCheck.x += step.x;
			distance = length1D.x;
			length1D.x += unitStepSize.x;
			horizontalHit = true;
		}
		else {
			mapCheck.y += step.y;
			distance = length1D.y;
			length1D.y += unitStepSize.y;
			horizontalHit = false;
		}

		if (mapCheck.x >= 0 && mapCheck.x < tile.getRows() && mapCheck.y >= 0 && mapCheck.y < tile.getCols()) {

			if (tile.getTile(mapCheck.x, mapCheck.y) == 1) {
				hasHitTile = true;
			}
		}
	}
}

float Raycaster::getDistance() {
	return distance;
}

bool Raycaster::hasHit() {
	return hasHitTile;
}

bool Raycaster::hasHitHorizontal() {
	return horizontalHit;
}

sf::Vector2f Raycaster::getIntersection() {
	return intersection;
}