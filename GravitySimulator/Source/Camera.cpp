#include "..\Include\Camera.hpp"
#include <SFML/Graphics.hpp>
#include "../Include/CelestialBody.hpp"
#include "../Include/Universe.hpp"
#include <iostream>

Camera::Camera(sf::RenderWindow& window, Universe& universe)
	: window(window)
	, universe(universe)
{
	lastSimulationState = universe.getSimulationState();
}

void Camera::update(float deltaTime)
{
	const SimulationState simulationState{ universe.getSimulationState() };
	// Camera is unlocked only when the simulation is either running or paused and central body is null
	bool unlocked{ !universe.getCentralBody() && (simulationState == SimulationState::Running || simulationState == SimulationState::Paused) };

	// If simulation state just changed to running from reset, reset the unlocked view to the default view
	if (lastSimulationState == SimulationState::Reset && simulationState == SimulationState::Running) {
		unlockedView = window.getDefaultView();
		//unlockedView.setSize(unlockedView.getSize() * zoom);
	}
	lastSimulationState = simulationState;

	// Allow camera movement only if unlocked
	if (unlocked) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			unlockedView.move({ -moveSpeed * deltaTime, 0.f });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			unlockedView.move({ moveSpeed * deltaTime, 0.f });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			unlockedView.move({ 0.f, -moveSpeed * deltaTime });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			unlockedView.move({ 0.f, moveSpeed * deltaTime });
		}
	}

	// Apply view to the window
	if (unlocked) {
		window.setView(unlockedView);
	}
	else {
		CelestialBody* centralBody{ universe.getCentralBody() };
		// Camera locked and central body is set - follow the central body
		if (centralBody) {
			sf::View view{ window.getView() };
			view.setCenter(centralBody->getCurrentPosition());
			window.setView(view);
		}
		// Camera locked and central body is null - default view
		else {
			window.setView(window.getDefaultView());
		}
	}

	// Apply zoom to the view
	sf::View zoomedView{ window.getView() };
	zoomedView.setSize(window.getDefaultView().getSize() * zoom);
	window.setView(zoomedView);
}

void Camera::zoomIn()
{
	zoom -= zoomSpeed;
	zoom = std::clamp(zoom, 0.f, maxZoom);
}

void Camera::zoomOut()
{
	zoom += zoomSpeed;
	zoom = std::clamp(zoom, 0.f, maxZoom);
}
