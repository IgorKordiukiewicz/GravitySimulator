#include "../3rdParty/doctest.h"
#include "../Include/Utils.hpp"

TEST_CASE("Utils") {
	const sf::Vector2f vec1{ 0.f, 0.f };
	const sf::Vector2f vec2{ 10.f, 0.f };

	SUBCASE("Vector length") {
		CHECK(utils::getVectorLength(vec1) == 0.f);
		CHECK(utils::getVectorLength(vec2) == 10.f);
	}

	SUBCASE("Normalized vector") {
		CHECK(utils::getNormalizedVector(vec1) == sf::Vector2f{ 0.f, 0.f });
		CHECK(utils::getNormalizedVector(vec2) == sf::Vector2f{ 1.f, 0.f });
	}

	SUBCASE("Distance between points") {
		CHECK(utils::getDistanceBetweenPoints(vec1, vec2) == 10.f);
	}
}