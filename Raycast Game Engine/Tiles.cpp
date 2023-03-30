#include "Tiles.hpp"

Tiles::Tiles(int cols, int rows) : grid(cols, std::vector<int>(rows, 0)) {
}
Tiles::Tiles(int width, int height, int cellSize) : grid(width / cellSize, std::vector<int>(height / cellSize, 0)) {
}

void Tiles::setTile(int value, int x, int y) {
	grid[x][y] = value;
}

int Tiles::getTile(int x, int y) {
	return grid[x][y];
}

int Tiles::getCols() {
	return grid[0].size();
}

int Tiles::getRows() {
	return grid.size();
}