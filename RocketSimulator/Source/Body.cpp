#include "..\Include\Body.hpp"

Body::Body(const sf::Vector2f& initialPosition, const sf::Vector2f& initialVelocity)
	: arrowShape(initialPosition, initialVelocity)
	, initialPosition(initialPosition)
	, initialVelocity(initialVelocity)
{
}

void Body::drawTrail(sf::RenderWindow& window)
{
	window.draw(trail);
}

void Body::reset()
{
	currentPosition = initialPosition;
	currentVelocity = initialVelocity;

	updateDrawablesPosition();
}

void Body::clearTrail()
{
	trail.clear();
}

void Body::setInitialPosition(const sf::Vector2f& newInitialPosition)
{
	initialPosition = newInitialPosition;
	currentPosition = initialPosition;

	arrowShape.setStartPos(currentPosition);
	updateDrawablesPosition();
}

void Body::setInitialVelocity(const sf::Vector2f& newInitialVelocity)
{
	initialVelocity = newInitialVelocity;
	currentVelocity = initialVelocity;

	arrowShape.setDirection(currentVelocity);
}
