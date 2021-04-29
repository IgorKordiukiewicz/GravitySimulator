#pragma once

#include <vector>
#include "CelestialBody.hpp"

class Universe
{
public:
	Universe();
	~Universe() = default;

	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

private:
	std::vector<CelestialBody> celestialBodies;
};
