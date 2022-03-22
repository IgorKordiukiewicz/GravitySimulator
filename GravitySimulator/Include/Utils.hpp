#pragma once

#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace utils
{
	template<typename T>
	float getVectorLength(const sf::Vector2<T>& vec)
	{
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}

	template<typename T>
	sf::Vector2f getNormalizedVector(const sf::Vector2<T>& vec)
	{
		const float vecLength = getVectorLength(vec);
		return (vecLength != 0.f ? sf::Vector2f(static_cast<float>(vec.x) / vecLength, static_cast<float>(vec.y) / vecLength) : sf::Vector2f{ 0.f, 0.f });
	}

	template<typename T>
	float getDistanceBetweenPoints(const sf::Vector2<T>& p1, const sf::Vector2<T>& p2)
	{
		return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	}

	template<typename T>
	void printVector(const sf::Vector2<T>& vec)
	{
		std::cout << vec.x << " : " << vec.y << '\n';
	}
}