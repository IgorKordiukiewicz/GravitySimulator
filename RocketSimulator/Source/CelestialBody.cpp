#include "../Include/CelestialBody.hpp"
#include <SFML/Graphics.hpp>

CelestialBody::CelestialBody(const float mass, const float radius, const sf::Vector2f& initialPosition, const sf::Vector2f& initialVelocity)
	: mass(mass)
	, radius(radius)
	, initialPosition(initialPosition)
	, initialVelocity(initialVelocity)
{
	shape.setRadius(radius);
	shape.setOrigin({ radius / 2.f, radius / 2.f });
	shape.setPosition(initialPosition);
	shape.setFillColor(sf::Color::White);

	currentPosition = initialPosition;
	currentVelocity = initialVelocity;
}

void CelestialBody::draw(sf::RenderWindow& window)
{
	window.draw(shape);
}
