#pragma once
#include <map>
#include <SFML\Graphics.hpp>

enum Material { PYRITE, ALUMINUM,};

//Contains default colors of all the elements
static std::map<Material, sf::Color> COLOR_MAP = {
	{ PYRITE, sf::Color::Cyan },
	{ ALUMINUM, sf::Color::Yellow }};


static std::map<Material, float> ALPHA_MAP = {
	{ PYRITE, 1.22e-3f },
	{ ALUMINUM, 8.418e-5f }};

