#include "../3rdParty/doctest.h"
#include "../Include/Color.hpp"

TEST_CASE("Color to SFML Color") {
	Color color;
	sf::Color sfmlColor = color.toSFMLColor();
	CHECK(sfmlColor.r == 255);
	CHECK(sfmlColor.g == 255);
	CHECK(sfmlColor.b == 255);

	color.r = 0.f;
	color.g = 0.f;
	color.b = 0.f;
	sfmlColor = color.toSFMLColor();
	CHECK(sfmlColor.r == 0);
	CHECK(sfmlColor.g == 0);
	CHECK(sfmlColor.b == 0);

	color.r = 0.5f;
	sfmlColor = color.toSFMLColor();
	CHECK(sfmlColor.r == 127);
}