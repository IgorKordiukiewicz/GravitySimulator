#include "../Include/Universe.hpp"
#include "../Include/CelestialBody.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Universe::Universe()
{
	CelestialBody body1{ 10.0f, 10.f, {500.f, 300.f},{50.f, 20.f} };
	CelestialBody body2{ 5.0f, 5.f, {890.f, 20.f},{-20.f, 10.f} };
	celestialBodies.push_back(std::move(body1));
	celestialBodies.push_back(std::move(body2));
}

void Universe::update(float deltaTime)
{
	// Check for potential bodies to destroy
	for (auto i = celestialBodies.size(); i--;) {
		if (celestialBodies[i].getShouldBeDeleted()) {
			celestialBodies.erase(celestialBodies.begin() + i);
		}
	}
	
	if (!simulationRunning) {
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
	for (auto& celestialBody : celestialBodies) {
		celestialBody.draw(window);
	}
}

void Universe::createNewBody()
{
	CelestialBody body{ 10.0f, 10.f, {10.f, 10.f},{0.f, 0.f} };
	celestialBodies.push_back(std::move(body));
}
