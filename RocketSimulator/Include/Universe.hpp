#pragma once

#include <vector>
#include "CelestialBody.hpp"
#include <memory>
#include "Rocket.hpp"

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

	void createRocket();

	void setCentralBody(CelestialBody* newCentralBody);
	void setDrawTrails(bool newValue);

	void runSimulation();
	void pauseSimulation();
	void resetSimulation();

	std::vector<CelestialBody>& getCelestialBodies() { return celestialBodies; }
	SimulationState getSimulationState() const { return simulationState; }
	bool getDrawTrails() const { return drawTrails; }

private:
	std::vector<CelestialBody> celestialBodies;
	std::unique_ptr<Rocket> rocket{ nullptr };

	// Gravitational constant
	const float gravitationalForce = 100000.f;

	SimulationState simulationState{ SimulationState::Reset };

	// Central body is the body the view is centered around
	CelestialBody* centralBody{ nullptr };
	// Whether to display the paths the bodies make
	bool drawTrails{ true };
};
