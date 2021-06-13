#include "../Include/Universe.hpp"
#include "../Include/CelestialBody.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Universe::Universe()
{
	CelestialBody body1{ 50.0f, 10.f, {640.f, 420.f},{60.f, 10.f} };
	CelestialBody body2{ 10.0f, 2.f, {690.f, 320.f},{-80.f, -50.f} };
	celestialBodies.push_back(std::move(body1));
	celestialBodies.push_back(std::move(body2));
}

void Universe::update(float deltaTime)
{
	// Check for potential bodies to delete
	for (auto i = celestialBodies.size(); i--;) {
		if (celestialBodies[i].getShouldBeDeleted()) {
			celestialBodies.erase(celestialBodies.begin() + i);
		}
	}
	
	// Only update the simulation if its state is set to running
	if (simulationState != SimulationState::Running) {
		return;
	}

	// Update velocities
	for (auto& celestialBody : celestialBodies) {
		celestialBody.updateVelocity(celestialBodies, gravitationalForce, deltaTime);
	}

	// Update positions
	for (auto& celestialBody : celestialBodies) {
		celestialBody.updatePosition(deltaTime);
	}
}

void Universe::draw(sf::RenderWindow& window)
{
	// If central body is set, then center the view around it
	if (centralBody) {
		sf::View view = window.getView();
		view.setCenter(centralBody->getCurrentPosition());
		window.setView(view);
	}
	else {
		window.setView(window.getDefaultView());
	}
	
	// Draw the trails first, so that a trail is never drawn over a celestial body
	if (drawTrails) {
		for (auto& celestialBody : celestialBodies) {
			celestialBody.drawTrail(window);
		}
	}
	
	// Draw the bodies
	for (auto& celestialBody : celestialBodies) {
		celestialBody.draw(window, simulationState == SimulationState::Reset);
	}
}

void Universe::createNewBody()
{
	// Create new body with some default values	
	CelestialBody body{ 10.0f, 10.f, {10.f, 10.f},{0.f, 0.f} };
	celestialBodies.push_back(std::move(body));
}

void Universe::setCentralBody(CelestialBody* newCentralBody)
{
	centralBody = newCentralBody;
}

void Universe::setDrawTrails(bool newValue)
{
	drawTrails = newValue;
}

void Universe::runSimulation()
{
	simulationState = SimulationState::Running;
}

void Universe::pauseSimulation()
{
	simulationState = SimulationState::Paused;
}

void Universe::resetSimulation()
{
	simulationState = SimulationState::Reset;

	// Reset bodies and clear their trails
	for (auto& celestialBody : celestialBodies) {
		celestialBody.clearTrail();
		celestialBody.reset();
	}
}