#include "..\Include\Rocket.hpp"
#include "../Include/CelestialBody.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Include/Utils.hpp"

Rocket::Rocket(const float gravitationalForce)
	: gravitationalForce(gravitationalForce)
	, Body({0.f, 0.f}, {0.f, 0.f})
{
	// Load texture	
	if (!texture.loadFromFile("Resources/rocket.png")) {
		std::cout << "Couldn't load the rocket texture\n";
	}
	texture.setSmooth(true);
	// Initialize sprite
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

	// Update trail
	if (trail.getVertexCount() >= 1) {
		// Compare body and last trail point positions casted to int vector to avoid unnecessary trail points
		const sf::Vector2i posInt(currentPosition);
		const sf::Vector2i lastTrailPosInt(trail[trail.getVertexCount() - 1].position);
		if (posInt != lastTrailPosInt) {
			trail.append({ currentPosition, sf::Color::White });
		}
	}
	else {
		trail.append({ currentPosition, sf::Color::White });
	}
}

void Rocket::draw(sf::RenderWindow& window, bool drawArrowShape)
{
	if (drawArrowShape) {
		window.draw(arrowShape);
	}
	window.draw(sprite);
}

void Rocket::setInitialBodyToOrbit(CelestialBody* newInitialBodyToOrbit)
{
	initialBodyToOrbit = newInitialBodyToOrbit;
	updateInitialOrbit();
}

void Rocket::updateInitialOrbit()
{
	if (!initialBodyToOrbit) {
		return;
	}
	
	const float distanceFromBody = initialBodyToOrbit->getRadius() / 2.f + initialOrbitHeight;

	const sf::Vector2f newInitialPosition = { initialBodyToOrbit->getInitialPosition().x + distanceFromBody, initialBodyToOrbit->getInitialPosition().y };
	setInitialPosition(newInitialPosition);

	const float PI = 3.14159f;
	const float theta = 90.f * (PI / 180.f);
	const float cos = std::cos(theta);
	const float sin = std::sin(theta);
	const sf::Vector2f direction = utils::getNormalizedVector(sf::Vector2f(initialBodyToOrbit->getInitialPosition() - initialPosition));
	const sf::Vector2f directionRotated = { direction.x * cos - direction.y * sin, direction.x * sin + direction.y * cos };
	const sf::Vector2f newInitialVelocity = directionRotated * sqrt((gravitationalForce * initialBodyToOrbit->getMass()) / distanceFromBody);
	setInitialVelocity(newInitialVelocity);
}

void Rocket::setInitialOrbitHeight(const float newInitialOrbitHeight)
{
	const float oldInitialOrbitHeight = initialOrbitHeight;
	initialOrbitHeight = newInitialOrbitHeight;

	// Only update the initial orbit if the initial orbit height value was changed
	if (oldInitialOrbitHeight != initialOrbitHeight) {
		updateInitialOrbit();
	}
}

void Rocket::updateDrawablesPosition()
{
	sprite.setPosition(currentPosition);
}
