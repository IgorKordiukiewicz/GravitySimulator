#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class ArrowShape : public sf::Drawable
{
public:
	ArrowShape(const sf::Vector2f& startPos, const sf::Vector2f& direction);
	virtual ~ArrowShape() = default;

	void setStartPos(const sf::Vector2f& newStartPos);
	void setDirection(const sf::Vector2f& newDirection);

	// Check whether a point is inside the shape
	bool contains(const sf::Vector2f& point) const;

	const sf::Vector2f& getStartPos() const { return startPos; }
	const sf::Vector2f& getDirection() const { return direction; }

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void calculateVertices();

private:
	sf::Vector2f startPos;
	// The direction the arrow is facing
	sf::Vector2f direction;
	float headHeight = 10.f;
	float headWidth = 11.5f;

	sf::VertexArray lineVertices;
	sf::VertexArray headVertices;
};