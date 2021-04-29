#include "../Include/Application.hpp"

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

Application::Application()
	: window(sf::VideoMode(1600, 900), "Rocket Simulator")
{
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);
}

void Application::run()
{
	sf::Clock clock;
	while (window.isOpen()) {
		const float deltaTime = clock.restart().asSeconds();

		processEvents();

		universe.update(deltaTime);

		ImGui::SFML::Update(window, sf::seconds(deltaTime));

		window.clear();

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