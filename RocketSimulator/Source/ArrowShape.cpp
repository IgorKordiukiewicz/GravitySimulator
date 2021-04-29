#include "../Include/ArrowShape.hpp"
#include <SFML/Graphics.hpp>s

ArrowShape::ArrowShape(const sf::Vector2f& startPos, const sf::Vector2f& endPos)
	: startPos(startPos)
	, direction(endPos)
{
	lineVertices.setPrimitiveType(sf::PrimitiveType::Lines);
	headVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
	
	calculateVertices();
}

void ArrowShape::setStartPos(const sf::Vector2f& newStartPos)
{
	startPos = newStartPos;
	calculateVertices();
}

void ArrowShape::setDirection(const sf::Vector2f& newEndPos)
{
	direction = newEndPos;
	calculateVertices();
}

bool ArrowShape::contains(const sf::Vector2f& point) const
{
	return headVertices.getBounds().contains(point);
}

void ArrowShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(lineVertices);
	target.draw(headVertices);
}

void ArrowShape::calculateVertices()
{
	const sf::Vector2f endPos = startPos + direction;
	
	lineVertices.clear();
	lineVertices.append(startPos);
	lineVertices.append(endPos);

	const sf::Vector2f dirVec = endPos - startPos;
	const float dirVecLength = sqrt(dirVec.x * dirVec.x + dirVec.y * dirVec.y);
	const sf::Vector2f dirVecNormalized = -dirVec / dirVecLength;
	const sf::Vector2f headBaseCenter = endPos + dirVecNormalized * headHeight;
	const sf::Vector2f dirVecOrth1 = { -dirVecNormalized.y, dirVecNormalized.x };
	const sf::Vector2f dirVecOrth2 = { dirVecNormalized.y, -dirVecNormalized.x };
	const float halfHeadWidth = headWidth / 2.f;
	const sf::Vector2f headPoint1 = headBaseCenter + dirVecOrth1 * halfHeadWidth;
	const sf::Vector2f headPoint2 = headBaseCenter + dirVecOrth2 * halfHeadWidth;

	headVertices.clear();
	headVertices.append(headPoint1);
	headVertices.append(headPoint2);
	headVertices.append(endPos);
}
