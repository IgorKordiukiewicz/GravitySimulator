#include "..\Include\Body.hpp"

Body::Body(const sf::Vector2f& initialPosition, const sf::Vector2f& initialVelocity)
	: arrowShape(initialPosition, initialVelocity)
	, initialPosition(initialPosition)
	, initialVelocity(initialVelocity)
{
}

void Body::reset()
{
	currentPosition = initialPosition;
	currentVelocity = initialVelocity;
}

void Body::setInitialPosition(const sf::Vector2f& newInitialPosition)
{
	initialPosition = newInitialPosition;
	currentPosition = initialPosition;

	arrowShape.setStartPos(currentPosition);
	onInitialPositionUpdated();
}

void Body::setInitialVelocity(const sf::Vector2f& newInitialVelocity)
{
	initialVelocity = newInitialVelocity;
	currentVelocity = initialVelocity;

	arrowShape.setDirection(currentVelocity);
}
