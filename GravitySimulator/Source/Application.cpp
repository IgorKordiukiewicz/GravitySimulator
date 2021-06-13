#include "../Include/Application.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <iostream>

Application::Application()
	: editor(window, universe), camera(window, universe)
{
	// Initialize window
	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;
	window.create(sf::VideoMode(1600, 900), "Gravity Simulator", sf::Style::Default, contextSettings);
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	// Add custom font
	ImGuiIO& io = ImGui::GetIO();
	//auto* font = io.Fonts->AddFontFromFileTTF("SourceSansPro-ExtraLight.ttf", 14.f);
	auto* font = io.Fonts->AddFontFromFileTTF("Resources/bahnschrift.ttf", 16.f);
	ImGui::SFML::UpdateFontTexture();
	editor.setFont(font);
}

void Application::run()
{
	sf::Text fpsText;
	sf::Font font;
	font.loadFromFile("Resources/SourceSansPro-ExtraLight.ttf");
	fpsText.setFont(font);
	fpsText.setCharacterSize(40);
	
	sf::Clock clock;
	// Main loop
	while (window.isOpen()) {
		const float deltaTime = clock.restart().asSeconds();

		processEvents();

		// Update
		ImGui::SFML::Update(window, sf::seconds(deltaTime));
		universe.update(deltaTime);
		camera.update(deltaTime);

		window.clear();

		editor.update();

		// Calculate fps
		const int fps = static_cast<int>(1.f / deltaTime);
		fpsText.setString(std::to_string(fps));

		// Render
		universe.draw(window);

		// Reset view to draw the text
		auto view = window.getView();
		window.setView(window.getDefaultView());
		window.draw(fpsText);
		window.setView(view);

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