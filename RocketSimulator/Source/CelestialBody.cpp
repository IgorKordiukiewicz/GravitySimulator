#include "../Include/CelestialBody.hpp"
#include <SFML/Graphics.hpp>

CelestialBody::CelestialBody(const float mass, const float radius, const sf::Vector2f& initialPosition, const sf::Vector2f& initialVelocity)
	: mass(mass)
	, radius(radius)
	, initialPosition(initialPosition)
	, initialVelocity(initialVelocity)
	, arrowShape(initialPosition, initialVelocity)
{
	currentPosition = initialPosition;
	currentVelocity = initialVelocity;
	
	setRadius(radius);
	bodyShape.setPosition(initialPosition);
	bodyShape.setFillColor(sf::Color::White);
}

void CelestialBody::draw(sf::RenderWindow& window)
{
	window.draw(arrowShape);
	window.draw(bodyShape);
}

void CelestialBody::setMass(const float newMass)
{
	mass = newMass;
}

void CelestialBody::setRadius(const float newRadius)
{
	radius = newRadius;
	// Update body shape
	bodyShape.setOrigin({ radius, radius });
	bodyShape.setRadius(radius);
}

void CelestialBody::setInitialPosition(const sf::Vector2f& newInitialPosition)
{
	initialPosition = newInitialPosition;
	currentPosition = initialPosition;
	
	bodyShape.setPosition(currentPosition);
	arrowShape.setStartPos(currentPosition);
}

void CelestialBody::setInitialVelocity(const sf::Vector2f& newInitialVelocity)
{
	initialVelocity = newInitialVelocity;
	currentVelocity = initialVelocity;

	arrowShape.setDirection(currentVelocity);
}