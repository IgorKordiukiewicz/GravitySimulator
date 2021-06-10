#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "ArrowShape.hpp"

class Body
{
public:
	Body(const sf::Vector2f& initialPosition, const sf::Vector2f& initialVelocity);
	virtual ~Body() = default;

	// Resets the body's position and velocity to the initial values
	void reset();

	void setInitialPosition(const sf::Vector2f& newInitialPosition);
	void setInitialVelocity(const sf::Vector2f& newInitialVelocity);

	const sf::Vector2f& getInitialPosition() const { return initialPosition; }
	const sf::Vector2f& getInitialVelocity() const { return initialVelocity; }
	const sf::Vector2f& getCurrentPosition() const { return currentPosition; }
	const sf::Vector2f& getCurrentVelocity() const { return currentVelocity; }
	const ArrowShape& getVelocityArrowShape() const { return arrowShape; }

protected:
	// TODO: change name
	virtual void updateDrawablesPosition() {}

protected:
	sf::Vector2f initialPosition;
	sf::Vector2f initialVelocity;
	sf::Vector2f currentPosition;
	sf::Vector2f currentVelocity;

	// Arrow displaying the body's initial velocity
	ArrowShape arrowShape;
};
