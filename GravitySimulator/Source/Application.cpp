#include "../Include/Application.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <iostream>

Application::Application()
	: editor(window, universe, presetManager), camera(window, universe)
{
	// Initialize window
	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;
	window.create(sf::VideoMode(1600, 900), "Gravity Simulator", sf::Style::Default, contextSettings);
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	// Set window icon
	sf::Image icon;
	icon.loadFromFile("Resources/icon.png");
	window.setIcon(64, 64, icon.getPixelsPtr());

	// Add custom font
	ImGuiIO& io = ImGui::GetIO();
	auto* font = io.Fonts->AddFontFromFileTTF("Resources/bahnschrift.ttf", 16.f);
	ImGui::SFML::UpdateFontTexture();
	editor.setFont(font);
}

void Application::run()
{
	presetManager.loadPresets();

	sf::Clock clock;
	// Main loop
	while (window.isOpen()) {
		const float deltaTime{ clock.restart().asSeconds() };

		processEvents();

		// Update
		ImGui::SFML::Update(window, sf::seconds(deltaTime));
		universe.update(deltaTime);
		camera.update(deltaTime);

		window.clear();

		// Render
		universe.draw(window);

		editor.update();

		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();

	presetManager.savePresets();
}

void Application::processEvents()
{
	sf::Event event;
	while (window.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::MouseWheelScrolled) {
			if (event.mouseWheelScroll.delta > 0) {
				camera.zoomIn();
			}
			else if (event.mouseWheelScroll.delta < 0) {
				camera.zoomOut();
			}
		}
	}
}