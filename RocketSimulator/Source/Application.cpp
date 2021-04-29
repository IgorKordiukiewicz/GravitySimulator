#include "../Include/Application.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

Application::Application()
	: editor(window, universe.getCelestialBodies())
{
	// Initialize window
	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;
	window.create(sf::VideoMode(1600, 900), "Rocket Simulator", sf::Style::Default, contextSettings);
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);
}

void Application::run()
{
	sf::Clock clock;
	while (window.isOpen()) {
		const float deltaTime = clock.restart().asSeconds();

		processEvents();

		ImGui::SFML::Update(window, sf::seconds(deltaTime));

		universe.update(deltaTime);

		window.clear();

		editor.update();

		// Render
		universe.draw(window);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}

void Application::processEvents()
{
	sf::Event event;
	while (window.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed) {
			window.close();
		}
	}
}