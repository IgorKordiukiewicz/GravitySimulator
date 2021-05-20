#pragma once

#include <vector>
#include "CelestialBody.hpp"

enum class SimulationState
{
	Running,
	Paused,
	Reset
};

class Universe
{
public:
	Universe();
	~Universe() = default;

	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

	// Adds a new body to the simulation
	void createNewBody();

	void setCentralBody(CelestialBody* newCentralBody);
	void setDrawTrails(bool newValue);

	void runSimulation();
	void pauseSimulation();
	void resetSimulation();

	inline std::vector<CelestialBody>& getCelestialBodies() { return celestialBodies; }
	inline SimulationState getSimulationState() const { return simulationState; }
	inline bool getDrawTrails() const { return drawTrails; }

private:
	std::vector<CelestialBody> celestialBodies;

	// Gravitational constant
	const float gravitationalForce = 100000.f;

	SimulationState simulationState{ SimulationState::Reset };

	// Central body is the body the view is centered around
	CelestialBody* centralBody{ nullptr };
	// Whether to display the paths the bodies make
	bool drawTrails{ true };
};
