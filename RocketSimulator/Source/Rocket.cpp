#include "..\Include\Rocket.hpp"
#include "../Include/CelestialBody.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Include/Utils.hpp"

Rocket::Rocket(const float gravitationalForce)
	: gravitationalForce(gravitationalForce)
	, Body({0.f, 0.f}, {0.f, 0.f})
{
	// Initialize sprite	
	if (!texture.loadFromFile("Resources/rocket.png")) {
		std::cout << "Couldn't load the rocket texture\n";
	}
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setScale({ 0.25f, 0.25f });
	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
}

void Rocket::update(const std::vector<CelestialBody>& celestialBodies, float deltaTime)
{
	for (const auto& otherBody :celestialBodies) {
		// Calculate velocity using Newton's law of universal gravitation
		const float dist = utils::getDistanceBetweenPoints(otherBody.getCurrentPosition(), currentPosition);
		const float distSquared = dist * dist;
		const sf::Vector2f forceDirection = utils::getNormalizedVector(sf::Vector2f(otherBody.getCurrentPosition() - currentPosition));
		const sf::Vector2f force = forceDirection * gravitationalForce * otherBody.getMass() / distSquared;
		const sf::Vector2f acceleration = force;
		currentVelocity += acceleration * deltaTime;
	}

	currentPosition += currentVelocity * deltaTime;
	updateDrawablesPosition();
}

void Rocket::draw(sf::RenderWindow& window, bool drawArrowShape)
{
	if (drawArrowShape) {
		window.draw(arrowShape);
	}
	window.draw(sprite);
}

void Rocket::orbitCelestialBody(const CelestialBody& celestialBody)
{
	const float distanceFromBody = celestialBody.getRadius() / 2.f + 30.f;

	const sf::Vector2f newInitialPosition = { celestialBody.getInitialPosition().x + distanceFromBody, celestialBody.getInitialPosition().y };
	setInitialPosition(newInitialPosition);

	const float PI = 3.14159f;
	const float theta = 90.f * (PI / 180.f);
	const float cos = std::cos(theta);
	const float sin = std::sin(theta);
	const sf::Vector2f direction = utils::getNormalizedVector(sf::Vector2f(celestialBody.getInitialPosition() - initialPosition));
	const sf::Vector2f directionRotated = { direction.x * cos - direction.y * sin, direction.x * sin + direction.y * cos };
	const sf::Vector2f newInitialVelocity = directionRotated * sqrt((gravitationalForce * celestialBody.getMass()) / distanceFromBody);
	setInitialVelocity(newInitialVelocity);
}

void Rocket::updateDrawablesPosition()
{
	sprite.setPosition(currentPosition);
}
