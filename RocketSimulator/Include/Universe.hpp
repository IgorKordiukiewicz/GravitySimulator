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

	inline void runSimulation() { simulationRunning = true; }
	inline void pauseSimulation() { simulationRunning = false; }

	inline std::vector<CelestialBody>& getCelestialBodies() { return celestialBodies; }
	inline bool isSimulationRunning() const { return simulationRunning; }

private:
	std::vector<CelestialBody> celestialBodies;

	const float gravitationalForce = 100000.f;

	bool simulationRunning{ false };
};
