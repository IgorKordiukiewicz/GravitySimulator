#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "ArrowShape.hpp"
#include "Color.hpp"
#include "Body.hpp"

class CelestialBody : public Body
{
public:
	CelestialBody(const float mass, const float radius, const sf::Vector2f& initialPosition, const sf::Vector2f& initialVelocity);
	~CelestialBody() = default;

	void updateVelocity(const std::vector<CelestialBody>& otherBodies, const float gravitationalForce, float deltaTime);
	void updatePosition(float deltaTime);

	void draw(sf::RenderWindow& window, bool drawArrowShape);

	void setMass(const float newMass);
	void setRadius(const float newRadius);
	void setColor(const Color& newColor);
	void markToDelete();

	int getId() const { return id; }
	bool getShouldBeDeleted() const { return shouldBeDeleted; }
	float getMass() const { return mass; }
	float getRadius() const { return radius; }
	const Color& getColor() const { return color; }
	const sf::CircleShape& getBodyShape() const { return bodyShape; }

private:
	virtual void updateDrawablesPosition() override;

private:
	float mass{ 1.0 };
	float radius{ 1.0 };

	Color color;
	sf::CircleShape bodyShape;

	bool shouldBeDeleted{ false };
	// The id is assigned on creation, and nextBodyId is incremented
	int id{ 0 };
	inline static int nextBodyId = 1;
};
