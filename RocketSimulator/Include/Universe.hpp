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

	const float gravitationalForce = 100000.f;

	SimulationState simulationState{ SimulationState::Reset };

	CelestialBody* centralBody{ nullptr };
	bool drawTrails{ true };
};
