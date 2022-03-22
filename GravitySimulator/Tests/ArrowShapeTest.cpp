#include "../3rdParty/doctest.h"
#include "../Include/ArrowShape.hpp"

TEST_CASE("ArrowShape") {
	sf::Vector2f startPos{ 0.f, 0.f };
	sf::Vector2f direction{ 0.f, 0.f };
	ArrowShape arrowShape{ startPos, direction };

	SUBCASE("contains") {
		direction = { 100.f, 100.f };
		arrowShape.setDirection(direction);
		const sf::Vector2f endPos{ startPos + direction };

		// Contains should return true only for points that are on the arrow head
		CHECK_FALSE(arrowShape.contains(startPos));
		CHECK_FALSE(arrowShape.contains({ -10.f, -10.f }));
		CHECK(arrowShape.contains(endPos - sf::Vector2f{0.1f, 0.1f}));
	}
}