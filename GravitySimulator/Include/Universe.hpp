#pragma once

#include <vector>
#include "CelestialBody.hpp"
#include <memory>
#include "Preset.hpp"

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

	void update(float deltaTime);
	void draw(sf::RenderWindow& window) const;

	void loadFromPreset(const Preset& preset);

	// Adds a new body to the simulation
	void createNewBody();

	void setCentralBody(CelestialBody* newCentralBody);
	void setDrawTrails(bool newValue);

	void runSimulation();
	void pauseSimulation();
	void resetSimulation();

	std::vector<CelestialBody>& getCelestialBodies() { return celestialBodies; }
	SimulationState getSimulationState() const { return simulationState; }
	CelestialBody* getCentralBody() const { return centralBody; }
	bool getDrawTrails() const { return drawTrails; }

private:
	std::vector<CelestialBody> celestialBodies;

	// Gravitational constant
	const float gravitationalForce{ 100000.f };

	SimulationState simulationState{ SimulationState::Reset };

	// Central body is the body the view is centered around
	CelestialBody* centralBody{ nullptr };

	// Whether to display the paths the bodies make
	bool drawTrails{ true };
};
