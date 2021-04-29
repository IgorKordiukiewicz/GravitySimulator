#include "../Include/CelestialBody.hpp"
#include <SFML/Graphics.hpp>

CelestialBody::CelestialBody(const float mass, const float radius, const sf::Vector2f& initialPosition, const sf::Vector2f& initialVelocity)
	: mass(mass)
	, radius(radius)
	, initialPosition(initialPosition)
	, initialVelocity(initialVelocity)
{
	bodyShape.setRadius(radius);
	bodyShape.setOrigin({ radius / 2.f, radius / 2.f });
	bodyShape.setPosition(initialPosition);
	bodyShape.setFillColor(sf::Color::White);

	currentPosition = initialPosition;
	currentVelocity = initialVelocity;
}

void CelestialBody::draw(sf::RenderWindow& window)
{
	window.draw(bodyShape);
}

void CelestialBody::setInitialPosition(const sf::Vector2f& newInitialPosition)
{
	initialPosition = newInitialPosition;
	currentPosition = initialPosition;
	bodyShape.setPosition(currentPosition);
}

void CelestialBody::setInitialVelocity(const sf::Vector2f& newInitialVelocity)
{
	initialVelocity = newInitialVelocity;
	currentVelocity = initialVelocity;
}
