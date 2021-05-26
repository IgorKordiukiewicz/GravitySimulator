#include "..\Include\Rocket.hpp"
#include "../Include/CelestialBody.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Rocket::Rocket()
	: Body({0.f, 0.f}, {0.f, 0.f})
{
	// Initialize sprite	
	if (!texture.loadFromFile("Resources/rocket.png")) {
		std::cout << "Couldn't load the rocket texture\n";
	}
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setScale({ 0.3f, 0.3f });
	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
}

void Rocket::update(const std::vector<CelestialBody>& celestialBodies, const float gravitationalForce, float deltaTime)
{
}

void Rocket::draw(sf::RenderWindow& window, bool drawArrowShape)
{
	if (drawArrowShape) {
		window.draw(arrowShape);
	}
	window.draw(sprite);
}

void Rocket::onInitialPositionUpdated()
{
	sprite.setPosition(initialPosition);
}
