#pragma once

#include <SFML/Graphics/Color.hpp>

struct Color
{
	// 0..1 range
	float r{ 1.f };
	float g{ 1.f };
	float b{ 1.f };

	sf::Color toSFMLColor() const
	{
		// Cast to 0..255 range
		const auto rInt{ static_cast<sf::Uint8>(r * 255.f) };
		const auto gInt{ static_cast<sf::Uint8>(g * 255.f) };
		const auto bInt{ static_cast<sf::Uint8>(b * 255.f) };
		return sf::Color{ rInt, gInt, bInt };
	}
};
