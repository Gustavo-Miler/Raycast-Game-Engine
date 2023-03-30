#include "Game.hpp"
#include <iostream>
#include "VectorMath.hpp"

Game::Game()
: mWindow(sf::VideoMode(800, 600), "Main Window")
, mPlayer()
, tiles(mWindow.getSize().x, mWindow.getSize().y, 32) {
	mPlayer.setPosition(float(mWindow.getSize().x) / 2, float(mWindow.getSize().y) / 2);
	if (!texture.loadFromFile("C:/Users/GUSTAVO/Downloads/Clock.png")) {
		std::cout << "Failed to load image" << std::endl;
	}
	mPlayer.setTexture(texture);
	mPlayer.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
	previousMouse = sf::Mouse::getPosition();
	speed = 3.f;
	memset(keyStates, 0, sizeof(keyStates));
	cellSize = mWindow.getSize().x / tiles.getCols();
	fov = 60;
	mapEditorOpen = true;
	isPressed = false;
}

void Game::run() {

	while (mWindow.isOpen()) {
		sf::Clock clock;
		sf::Time timeSinceLastUpdate = sf::Time::Zero;
		sf::Time timePerFrame = sf::seconds(1.f / 60.f);
		
		while (mWindow.isOpen()) {
			processEvents();

			timeSinceLastUpdate += clock.restart();
			while (timeSinceLastUpdate > timePerFrame) {
				timeSinceLastUpdate -= timePerFrame;
				processEvents();
				update(timePerFrame);
			}
			render();
		}
	}
}

void Game::processEvents() {
	sf::Event event;
	while (mWindow.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			mWindow.close();
			break;
		case sf::Event::KeyPressed:
			handleKeyboardInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			handleKeyboardInput(event.key.code, false);
			break;
		}
	}
}

void Game::handleKeyboardInput(sf::Keyboard::Key key, bool isPressed) {
	keyStates[key] = isPressed;
}

void Game::update(sf::Time deltaTime) {
	mouseDelta = sf::Mouse::getPosition() - previousMouse;
	mPlayer.rotate(mouseDelta.x);

	sf::Vector2f axis(keyStates[sf::Keyboard::Key::D] - keyStates[sf::Keyboard::Key::A], keyStates[sf::Keyboard::Key::S] - keyStates[sf::Keyboard::Key::W]);
	norm(axis);
	float direction = mPlayer.getRotation();
	velocity = sf::Vector2f(-cos(degToRad(direction)) * axis.y * speed + cos(degToRad(direction + 90)) * axis.x * speed
							, -sin(degToRad(direction)) * axis.y * speed + sin(degToRad(direction + 90)) * axis.x * speed);
	mPlayer.move(velocity);

	previousMouse = sf::Mouse::getPosition();

	sf::Vector2i mpos = sf::Mouse::getPosition(mWindow);
	if (mpos.x >= 0 && mpos.x < mWindow.getSize().x && mpos.y >= 0 && mpos.y < mWindow.getSize().y) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			tiles.setTile(1, sf::Mouse::getPosition(mWindow).x / cellSize, sf::Mouse::getPosition(mWindow).y / cellSize);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			tiles.setTile(0, sf::Mouse::getPosition(mWindow).x / cellSize, sf::Mouse::getPosition(mWindow).y / cellSize);
		}
	}

	if (keyStates[sf::Keyboard::Space] && !isPressed) {
		mapEditorOpen = !mapEditorOpen;
		isPressed = true;
	}
	else if (!keyStates[sf::Keyboard::Space]) {
		isPressed = false;
	}
}

void Game::render() {

	if (mapEditorOpen) {
		mWindow.clear();
		sf::RectangleShape rect(sf::Vector2f(cellSize, cellSize));
		for (int i = 0; i < tiles.getRows(); i++) {
			for (int j = 0; j < tiles.getCols(); j++) {
				if (tiles.getTile(i, j) == 1) {
					rect.setPosition(i * cellSize, j * cellSize);
					mWindow.draw(rect);
				}
			}
		}

		std::vector<sf::Vertex> line;
		for (int i = -fov / 2; i < fov / 2; i++) {
			sf::Vector2f dir = sf::Vector2f(cos(degToRad(mPlayer.getRotation() + i)), sin(degToRad(mPlayer.getRotation() + i)));
			norm(dir);
			raycaster.run(sf::Vector2f(mPlayer.getPosition().x / cellSize, mPlayer.getPosition().y / cellSize), dir, 30.f, tiles);
			line.push_back(sf::Vertex(mPlayer.getPosition(), sf::Color::Green));
			if (raycaster.hasHit()) {
				line.push_back(sf::Vertex(mPlayer.getPosition() + dir * raycaster.getDistance() * float(cellSize), sf::Color::Green));
			}
			else {
				line.push_back(sf::Vertex(mPlayer.getPosition() + dir * 30.f * float(cellSize), sf::Color::Green));
			}
		}
		mWindow.draw(&line[0], line.size(), sf::Lines);
	}
	else {
		mWindow.clear(sf::Color::Cyan);
		sf::RectangleShape floor(sf::Vector2f(mWindow.getSize().x, mWindow.getSize().y));
		floor.setFillColor(sf::Color::Green);
		floor.setPosition(0, 300);
		mWindow.draw(floor);

		float lineWidth = mWindow.getSize().x / fov;
		for (int i = -fov / 2; i < fov / 2; i++) {
			sf::Vector2f dir = sf::Vector2f(cos(degToRad(mPlayer.getRotation() + i)), sin(degToRad(mPlayer.getRotation() + i)));
			norm(dir);
			raycaster.run(sf::Vector2f(mPlayer.getPosition().x / cellSize, mPlayer.getPosition().y / cellSize), dir, 30.f, tiles);
			if (raycaster.hasHit()) {
				float ca = i; if (ca < 0) ca += PI * 2; if (ca > 0) ca -= PI * 2;
				float dist = raycaster.getDistance() * cos(degToRad(ca));
				float lineHeight = mWindow.getSize().y / dist;
				sf::RectangleShape rect(sf::Vector2f(lineWidth, lineHeight));
				rect.setOrigin(0, lineHeight / 2);
				rect.setPosition((i + fov / 2) * lineWidth, 300);
				if (raycaster.hasHitHorizontal()) {
					rect.setFillColor(sf::Color(0, 0, 120));
				}
				else {
					rect.setFillColor(sf::Color(0, 0, 150));
				}
				mWindow.draw(rect);
			}
		}
	}


	mWindow.display();
}