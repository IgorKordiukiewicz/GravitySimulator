#include "../Include/Preset.hpp"
#include <fstream>
#include <optional>
#include <sstream>
#include <filesystem>

Preset::Preset(const std::string& name)
	: name(name)
{
}

Preset::Preset(const std::string& name, const std::vector<CelestialBody>& celestialBodies)
	: name(name)
{
	for (const CelestialBody& celestialBody : celestialBodies) {
		CelestialBodyProperties celestialBodyProperties;
		celestialBodyProperties.mass = celestialBody.getMass();
		celestialBodyProperties.radius = celestialBody.getRadius();
		celestialBodyProperties.initialPosition = celestialBody.getInitialPosition();
		celestialBodyProperties.initialVelocity = celestialBody.getInitialVelocity();

		celestialBodiesProperties.push_back(std::move(celestialBodyProperties));
	}
}

void Preset::loadFromFile()
{
	// Create Presets directory if it doesn't exist
	auto presetsPath{ std::filesystem::current_path() / "Presets" };
	if (!std::filesystem::is_directory(presetsPath)) {
		std::filesystem::create_directory(presetsPath);
	}

	std::ifstream file{ "Presets/" + name + ".preset" };

	std::string line;
	int propertyCounter{ 0 };
	std::vector<float> masses;
	std::vector<float> radiuses;
	std::vector<sf::Vector2f> initialPositions;
	std::vector<sf::Vector2f> initialVelocities;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		// mass
		if (propertyCounter == 0) {
			float mass;
			iss >> mass;
			masses.push_back(mass);
		}
		// radius
		else if (propertyCounter == 1) {
			float radius;
			iss >> radius;
			radiuses.push_back(radius);
		}
		// initial position
		else if (propertyCounter == 2) {
			float x, y;
			iss >> x >> y;
			initialPositions.push_back(sf::Vector2f(x, y));
		}
		// initial velocity
		else if (propertyCounter == 3) {
			float x, y;
			iss >> x >> y;
			initialVelocities.push_back(sf::Vector2f(x, y));
		}

		++propertyCounter;
		if (propertyCounter > 3) {
			propertyCounter = 0;
		}
	}

	celestialBodiesProperties.clear();
	// Create celestial bodies properties from the file data
	for (int i{ 0 }; i < masses.size(); ++i) {
		CelestialBodyProperties celestialBodyProperties;
		celestialBodyProperties.mass = masses[i];
		celestialBodyProperties.radius = radiuses[i];
		celestialBodyProperties.initialPosition = initialPositions[i];
		celestialBodyProperties.initialVelocity = initialVelocities[i];

		celestialBodiesProperties.push_back(std::move(celestialBodyProperties));
	}

	file.close();
}

void Preset::saveToFile() const
{
	std::ofstream file{ "Presets/" + name + ".preset" };

	for (const CelestialBodyProperties& celestialBodyProperties : celestialBodiesProperties) {
		file << celestialBodyProperties.mass << '\n';
		file << celestialBodyProperties.radius << '\n';
		file << celestialBodyProperties.initialPosition.x << " " << celestialBodyProperties.initialPosition.y << '\n';
		file << celestialBodyProperties.initialVelocity.x << " " << celestialBodyProperties.initialVelocity.y << '\n';
	}

	file.close();
}
