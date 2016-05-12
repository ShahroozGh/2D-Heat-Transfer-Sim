#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "meshSystem.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int windowStyle)
{
	sf::RenderWindow window(sf::VideoMode(700, 700), "Heat Solver");

	//Create new mesh sys
	meshSystem tempSystem(700,700);


	//Parameters for input
	float mouseTemperature = 500.0f;
	int brushSize = 5;
	Material brushMaterial = PYRITE;
	

	//Calculate stability const
	float stabilityValue = pow(tempSystem.h, 2.0f) / (4.0f*ALPHA_MAP[PYRITE]*tempSystem.timeMultiplier);
	bool isStable;
	if (tempSystem.dTime <= stabilityValue)
		isStable = true;
	else
		isStable = false;

	//Text stuff
	sf::Clock clock;

	//Text
	//Load font
	sf::Font font;
	if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
	{
		// error...
		return 0;
	}

	//Create text to draw
	sf::Text elapsedTime;
	// set the string to display
	elapsedTime.setString("Time: ");
	// set the character size
	elapsedTime.setCharacterSize(12); // in pixels, not points!
									  // set the color
	elapsedTime.setColor(sf::Color::White);

	elapsedTime.setFont(font);
	//----------------------------------------------------
	sf::Text fpsLabel;
	fpsLabel.setPosition(sf::Vector2f(600, 20));
	// set the string to display
	fpsLabel.setString("FPS: ");
	// set the character size
	fpsLabel.setCharacterSize(12); // in pixels, not points!
								   // set the color
	fpsLabel.setColor(sf::Color::White);

	fpsLabel.setFont(font);
	//----------------------------------------------------

	sf::Text temperatureLabel;
	temperatureLabel.setPosition(sf::Vector2f(550, 40));
	// set the string to display
	temperatureLabel.setString("Temperature: ");
	// set the character size
	temperatureLabel.setCharacterSize(12); // in pixels, not points!
										   // set the color
	temperatureLabel.setColor(sf::Color::White);

	temperatureLabel.setFont(font);

	//----------------------------------------------------

	sf::Text mouseTemperatureLabel;
	mouseTemperatureLabel.setPosition(sf::Vector2f(540, 60));
	// set the string to display
	mouseTemperatureLabel.setString("Mouse Temperature: ");
	// set the character size
	mouseTemperatureLabel.setCharacterSize(12); // in pixels, not points!
										   // set the color
	mouseTemperatureLabel.setColor(sf::Color::White);

	mouseTemperatureLabel.setFont(font);

	//----------------------------------------------------


	sf::Text brushLabel;
	brushLabel.setPosition(sf::Vector2f(550, 80));
	// set the string to display
	brushLabel.setString("Brush Size: ");
	// set the character size
	brushLabel.setCharacterSize(12); // in pixels, not points!
												// set the color
	brushLabel.setColor(sf::Color::White);

	brushLabel.setFont(font);

	//----------------------------------------------------


	sf::Text brushTypeLabel;
	brushTypeLabel.setPosition(sf::Vector2f(550, 100));
	// set the string to display
	brushTypeLabel.setString("Brush Material: PYRITE");
	// set the character size
	brushTypeLabel.setCharacterSize(12); // in pixels, not points!
									 // set the color
	brushTypeLabel.setColor(sf::Color::White);

	brushTypeLabel.setFont(font);

	//----------------------------------------------------


	sf::Text deltaTime;
	deltaTime.setPosition(sf::Vector2f(0, 20));
	// set the string to display
	deltaTime.setString("dTime (s): " + std::to_string(tempSystem.dTime));
	// set the character size
	deltaTime.setCharacterSize(12); // in pixels, not points!
										   // set the color
	deltaTime.setColor(sf::Color::White);

	deltaTime.setFont(font);

	//----------------------------------------------------


	sf::Text stabilityConst;
	stabilityConst.setPosition(sf::Vector2f(0, 40));
	// set the string to display
	stabilityConst.setString("(h^2)/4a: " + std::to_string(stabilityValue));
	// set the character size
	stabilityConst.setCharacterSize(12); // in pixels, not points!
									// set the color
	stabilityConst.setColor(sf::Color::White);

	stabilityConst.setFont(font);

	//----------------------------------------------------


	sf::Text pausedLabel;
	pausedLabel.setPosition(sf::Vector2f(0, 80));
	// set the string to display
	pausedLabel.setString("RUNNING");
	// set the character size
	pausedLabel.setCharacterSize(12); // in pixels, not points!
										 // set the color
	pausedLabel.setColor(sf::Color::White);

	pausedLabel.setFont(font);

	//----------------------------------------------------


	sf::Text isStableText;
	isStableText.setPosition(sf::Vector2f(0, 60));
	// set the string to display
	if (isStable)
		isStableText.setString("STABLE");
	else
		isStableText.setString("UNSTABLE");
	// set the character size
	isStableText.setCharacterSize(12); // in pixels, not points!
										 // set the color
	isStableText.setColor(sf::Color::White);

	isStableText.setFont(font);

	//----------------------------------------------------


	//Update Loop

	while (window.isOpen())
	{
		
		//Poll for events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i localPosition = sf::Mouse::getPosition(window);
					
				}
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					//sf::Vector2i localPosition = sf::Mouse::getPosition(window);

					
				}
			}


			if (event.type == sf::Event::KeyPressed)
			{

				switch (event.key.code) {
				case sf::Keyboard::Up:
						brushSize += 5;
						if (brushSize > 100)
							brushSize = 100;
						brushLabel.setString("Brush Size: " + std::to_string(brushSize));
					break;
				case sf::Keyboard::Down:
						brushSize -= 5;
						if (brushSize < 5)
							brushSize = 5;
						brushLabel.setString("Brush Size: " + std::to_string(brushSize));
					break;
				case sf::Keyboard::Space:
					tempSystem.SIMULATION_PAUSED = !tempSystem.SIMULATION_PAUSED;
					if (tempSystem.SIMULATION_PAUSED)
						pausedLabel.setString("PAUSED");
					else
						pausedLabel.setString("RUNNING");
					break;
				case sf::Keyboard::C:
						tempSystem.USE_GRAY_SCALE_TEMP = !tempSystem.USE_GRAY_SCALE_TEMP;
						tempSystem.refreshColors();
					break;
				case sf::Keyboard::M:
						tempSystem.SHOW_MATERIAL = !tempSystem.SHOW_MATERIAL;
						tempSystem.refreshColors();
					break;
				case sf::Keyboard::Num1:
						brushMaterial = PYRITE;
						brushTypeLabel.setString("Brush Material: PYRITE");
						tempSystem.refreshColors();
					break;
				case sf::Keyboard::Num2:
						brushMaterial = ALUMINUM;
						brushTypeLabel.setString("Brush Material: ALUMINUM");
						tempSystem.refreshColors();
					break;
				}
			}

			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {

					
					mouseTemperature += event.mouseWheelScroll.delta;
					if (mouseTemperature > 1000.0f)
						mouseTemperature = 1000.0f;
					if (mouseTemperature < 0.0f)
						mouseTemperature = 0.0f;

					mouseTemperatureLabel.setString("Mouse Temperature: " + std::to_string(mouseTemperature));
					

				}
				
			}

		}

		//Draw on screen using mouse
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i localPosition = sf::Mouse::getPosition(window); // window is a sf::Window
																		 // left mouse button is pressed:
			
			if (localPosition.x < 700 && localPosition.x > 0 && localPosition.y < 700 && localPosition.y > 0)
				tempSystem.setTemperature(localPosition.x, localPosition.y, mouseTemperature, brushMaterial, brushSize);
		}


		//update temperature text field
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (mousePos.x < 700 && mousePos.x > 0 && mousePos.y < 700 && mousePos.y > 0)
				temperatureLabel.setString("Temperature: " + std::to_string(tempSystem.getTempFromMouse(mousePos.x, mousePos.y)));


		//Update temperature map
		tempSystem.update();
		
		window.clear();
		
		//Draw temperature map
		window.draw(tempSystem);

		//Draw text
		window.draw(elapsedTime);
		window.draw(fpsLabel);
		window.draw(temperatureLabel);
		window.draw(deltaTime);
		window.draw(stabilityConst);
		window.draw(isStableText);
		window.draw(mouseTemperatureLabel);
		window.draw(brushLabel);
		window.draw(pausedLabel);
		window.draw(brushTypeLabel);
		
		window.display();

		sf::Time elapsed = clock.restart();
		elapsedTime.setString("Time:" + std::to_string(elapsed.asMilliseconds()));
		fpsLabel.setString("FPS:" + std::to_string(1000.0f / elapsed.asMilliseconds()));

		window.setFramerateLimit(60);
	}

	return 0;
}