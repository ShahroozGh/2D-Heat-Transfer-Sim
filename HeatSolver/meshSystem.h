#pragma once
#include <vector>
#include <SFML\Graphics.hpp>
#include "Constants.h"

class meshSystem : public sf::Drawable, public sf::Transformable
{
public:
	meshSystem();
	meshSystem(int winWidth, int winHeight);
	~meshSystem();

	struct Node {
		float temperature, nextTemperature;
		float alpha;
		bool isBorder;

		Node() {
			temperature = 1000;
			nextTemperature = 1000;
			alpha = ALPHA_MAP[PYRITE];
			
			isBorder = false;

		}
		Node(float temp, Material type, bool border) {
			temperature = temp;
			nextTemperature = temp;
			alpha = ALPHA_MAP[type];
			isBorder = border;
		}
	};

	std::vector<std::vector<Node> > mesh;

	sf::VertexArray m_vertices;


	float dTime = 1.0f / 60.0f;
	float timeMultiplier = 4.0f; //To speed up/ slow down sim, 1.0f is realtime
	float h = 8.0f / 320.0f; //Window is 10mx10m

	//Note this will only be stable when dtime <= h^2 / 4*alpha

	//States
	bool USE_GRAY_SCALE_TEMP;
	bool SIMULATION_PAUSED;
	bool SHOW_MATERIAL;


	void update();
	float getTempFromMouse(int x, int y);
	void setTemperature(int x, int y, float temperature, Material type, int brushSize);
	void refreshColors();

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// our particles don't use a texture
		states.texture = NULL;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

	int WINDOW_WIDTH, WINDOW_HEIGHT;
	int WIN_TILE_WIDTH, WIN_TILE_HEIGHT;
	int TILE_SIZE;

	void initVerticies();
	void updateTileColor(int x, int y);
	sf::Color tempToColor(float temp);

	//Gets ptr to quad sub-array at x,y
	sf::Vertex* getQuad(int x, int y);
};

