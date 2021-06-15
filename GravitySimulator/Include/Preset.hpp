#pragma once

#include <string>
#include <vector>
#include "CelestialBody.hpp"

struct CelestialBodyProperties
{
	float mass;
	float radius;
	sf::Vector2f initialPosition;
	sf::Vector2f initialVelocity;
};

class Preset
{
public:
	Preset(const std::string& name);
	Preset(const std::string& name, const std::vector<CelestialBody>& celestialBodies);

	void loadFromFile();
	void saveToFile() const;

	const std::string& getName() const { return name; }
	const std::vector<CelestialBodyProperties>& getCelestialBodiesProperties() const { return celestialBodiesProperties; }

private:
	const std::string name{ "Preset" };

	std::vector<CelestialBodyProperties> celestialBodiesProperties;
};
