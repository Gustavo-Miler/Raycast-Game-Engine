#pragma once
#include <SFML/Graphics.hpp>
#include "Tiles.hpp"
#include "Raycaster.hpp"

class Game {
public:
	Game();
	void run();

private:
	void processEvents();
	void handleKeyboardInput(sf::Keyboard::Key key, bool isPressed);
	void update(sf::Time deltaTime);
	void render();

private:
	sf::RenderWindow mWindow;
	sf::Sprite mPlayer;
	sf::Texture texture;
	sf::Vector2f velocity;
	sf::Vector2i mouseDelta, previousMouse;
	float speed;
	bool keyStates[256];
	Tiles tiles;
	int cellSize;
	Raycaster raycaster;
	float fov;
	bool mapEditorOpen;
	bool isPressed;
};