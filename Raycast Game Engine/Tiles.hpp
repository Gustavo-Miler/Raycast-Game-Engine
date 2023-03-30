#pragma once
#include <vector>

class Tiles {
public:
	Tiles(int cols, int rows);
	Tiles(int width, int height, int cellSize);
	void setTile(int value, int x, int y);
	int getTile(int x, int y);
	int getRows();
	int getCols();

private:
	std::vector<std::vector<int>> grid;
};