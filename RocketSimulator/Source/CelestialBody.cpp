#include "../Include/CelestialBody.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Include/Utils.hpp"

CelestialBody::CelestialBody(const float mass, const float radius, const sf::Vector2f& initialPosition, const sf::Vector2f& initialVelocity)
	: Body(initialPosition, initialVelocity)
	, mass(mass)
	, radius(radius)
	, id(nextBodyId++)
{	
	currentPosition = initialPosition;
	currentVelocity = initialVelocity;
	
	// Setup the body shape
	setRadius(radius);
	bodyShape.setPosition(initialPosition);
	bodyShape.setFillColor(color.toSFMLColor());

	trail.setPrimitiveType(sf::LineStrip);
}

void CelestialBody::updateVelocity(const std::vector<CelestialBody>& otherBodies, const float gravitationalForce, float deltaTime)
{
	for (const auto& otherBody : otherBodies) {
		if (otherBody.getId() == id) {
			continue;
		}

		// Calculate velocity using Newton's law of universal gravitation
		const float dist = utils::getDistanceBetweenPoints(otherBody.getCurrentPosition(), currentPosition);
		const float distSquared = dist * dist;
		const sf::Vector2f forceDirection = utils::getNormalizedVector(sf::Vector2f(otherBody.getCurrentPosition() - currentPosition));
		const sf::Vector2f force = forceDirection * gravitationalForce * mass * otherBody.getMass() /  distSquared;
		const sf::Vector2f acceleration = force / mass;
		currentVelocity += acceleration * deltaTime;
	}
}

void CelestialBody::updatePosition(float deltaTime)
{	
	currentPosition += currentVelocity * deltaTime;
	bodyShape.setPosition(currentPosition);

	// Update trail
	if (trail.getVertexCount() >= 1) {
		// Compare body and last trail point positions casted to int vector to avoid unnecessary trail points
		const sf::Vector2i posInt(currentPosition);
		const sf::Vector2i lastTrailPosInt(trail[trail.getVertexCount() - 1].position);
		if (posInt != lastTrailPosInt) {
			trail.append({ currentPosition, bodyShape.getFillColor() });
		}
	}
	else {
		trail.append({ currentPosition, bodyShape.getFillColor() });
	}
}

void CelestialBody::drawTrail(sf::RenderWindow& window)
{
	window.draw(trail);
}

void CelestialBody::draw(sf::RenderWindow& window, bool drawArrowShape)
{
	if (drawArrowShape) {
		window.draw(arrowShape);
	}
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

void CelestialBody::setColor(const Color& newColor)
{
	color = newColor;
	bodyShape.setFillColor(color.toSFMLColor());
}

void CelestialBody::clearTrail()
{
	trail.clear();
}

void CelestialBody::markToDelete()
{
	shouldBeDeleted = true;
}

void CelestialBody::updateDrawablesPosition()
{
	bodyShape.setPosition(currentPosition);
}
