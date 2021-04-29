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

	inline std::vector<CelestialBody>& getCelestialBodies() { return celestialBodies; }

private:
	std::vector<CelestialBody> celestialBodies;

	const float gravitationalForce = 0.01f;
};
