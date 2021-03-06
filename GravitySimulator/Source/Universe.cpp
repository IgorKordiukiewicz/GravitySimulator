#include "../Include/Universe.hpp"
#include "../Include/CelestialBody.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Universe::Universe()
{
	CelestialBody body1{ 50.0f, 10.f, { 640.f, 420.f }, { 60.f, 10.f } };
	CelestialBody body2{ 10.0f, 2.f, { 690.f, 320.f }, { -80.f, -50.f } };
	celestialBodies.push_back(std::move(body1));
	celestialBodies.push_back(std::move(body2));
}

void Universe::update(float deltaTime)
{
	// When the application freezes for a bit or the window is moved by the user,
	// the delta time is very high and it causes the simulation to break
	if (deltaTime > 0.05f) {
		return;
	}

	// Check for potential bodies to delete
	for (auto i{ celestialBodies.size() }; i--;) {
		if (celestialBodies[i].getShouldBeDeleted()) {
			celestialBodies.erase(celestialBodies.begin() + i);
		}
	}

	// Only update the simulation if its state is set to running
	if (simulationState != SimulationState::Running) {
		return;
	}

	// Update velocities
	for (CelestialBody& celestialBody : celestialBodies) {
		celestialBody.updateVelocity(celestialBodies, gravitationalForce, deltaTime);
	}

	// Update positions
	for (CelestialBody& celestialBody : celestialBodies) {
		celestialBody.updatePosition(deltaTime);
	}

	// Check for collisions
	for (int i{ 0 }; i < celestialBodies.size(); ++i) {
		// Start at i to avoid checking for collision between 2 bodies twice
		for (int j{ i }; j < celestialBodies.size(); ++j) {
			if (i == j || celestialBodies[i].isDestroyed() || celestialBodies[j].isDestroyed()) {
				continue;
			}

			const sf::CircleShape& shape1{ celestialBodies[i].getBodyShape() };
			const sf::CircleShape& shape2{ celestialBodies[j].getBodyShape() };
			// Destroy the bodies if collision occured
			if (shape1.getGlobalBounds().intersects(shape2.getGlobalBounds())) {
				celestialBodies[i].destroy();
				celestialBodies[j].destroy();
			}
		}
	}
}

void Universe::draw(sf::RenderWindow& window) const
{
	// Draw the trails first, so that a trail is never drawn over a celestial body
	if (drawTrails) {
		for (const CelestialBody& celestialBody : celestialBodies) {
			celestialBody.drawTrail(window);
		}
	}

	// Draw the bodies
	for (const CelestialBody& celestialBody : celestialBodies) {
		celestialBody.draw(window, simulationState == SimulationState::Reset);
	}
}

void Universe::loadFromPreset(const Preset& preset)
{
	celestialBodies.clear();
	for (const auto& [mass, radius, initialPosition, initialVelocity] : preset.getCelestialBodiesProperties()) {
		CelestialBody celestialBody{ mass, radius, initialPosition, initialVelocity };
		celestialBodies.push_back(std::move(celestialBody));
	}
}

void Universe::createNewBody()
{
	// Create new body with some default values
	CelestialBody body{ 10.0f, 10.f, { 10.f, 10.f }, { 0.f, 0.f } };
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
	for (CelestialBody& celestialBody : celestialBodies) {
		celestialBody.clearTrail();
		celestialBody.reset();
	}
}