#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "ArrowShape.hpp"

class CelestialBody
{
public:
	CelestialBody(const float mass, const float radius, const sf::Vector2f& initialPosition, const sf::Vector2f& initialVelocity);
	~CelestialBody() = default;

	void updateVelocity(const std::vector<CelestialBody>& otherBodies, const float gravitationalForce, float deltaTime);
	void updatePosition(float deltaTime);

	void drawTrail(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);

	void setMass(const float newMass);
	void setRadius(const float newRadius);
	void setInitialPosition(const sf::Vector2f& newInitialPosition);
	void setInitialVelocity(const sf::Vector2f& newInitialVelocity);
	void setColor(const sf::Color& newColor);
	void clearTrail();
	void markToDelete();

	inline int getId() const { return id; }
	inline bool getShouldBeDeleted() const { return shouldBeDeleted; }
	inline float getMass() const { return mass; }
	inline float getRadius() const { return radius; }
	inline const sf::Vector2f& getInitialPosition() const { return initialPosition; }
	inline const sf::Vector2f& getInitialVelocity() const { return initialVelocity; }
	inline const sf::Vector2f& getCurrentPosition() const { return currentPosition; }
	inline const sf::Vector2f& getCurrentVelocity() const { return currentVelocity; }
	inline const sf::CircleShape& getBodyShape() const { return bodyShape; }
	inline const ArrowShape& getVelocityArrowShape() const { return arrowShape; }

private:
	float mass{ 1.0 };
	float radius{ 1.0 };
	sf::Vector2f initialPosition;
	sf::Vector2f initialVelocity;
	sf::Vector2f currentPosition;
	sf::Vector2f currentVelocity;

	sf::CircleShape bodyShape;
	ArrowShape arrowShape;

	sf::VertexArray trail;

	bool shouldBeDeleted{ false };
	int id{ 0 };
	inline static int nextBodyId = 1;
};
