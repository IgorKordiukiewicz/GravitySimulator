#include "..\Include\Rocket.hpp"
#include "../Include/CelestialBody.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Rocket::Rocket()
{
	if (!texture.loadFromFile("Resources/rocket.png")) {
		std::cout << "Couldn't load the rocket texture\n";
	}
	sprite.setTexture(texture);
	sprite.setScale({ 0.3f, 0.3f });
}

void Rocket::update(const std::vector<CelestialBody>& celestialBodies, const float gravitationalForce, float deltaTime)
{
}

void Rocket::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}
