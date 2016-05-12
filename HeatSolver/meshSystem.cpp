#include "meshSystem.h"



meshSystem::meshSystem()
{
}

meshSystem::meshSystem(int winWidth, int winHeight)
{
	SIMULATION_PAUSED = true;
	USE_GRAY_SCALE_TEMP = true;
	SHOW_MATERIAL = false;

	TILE_SIZE = 2;
	WINDOW_WIDTH = winWidth;
	WINDOW_HEIGHT = winHeight;

	WIN_TILE_WIDTH = winWidth / TILE_SIZE;
	WIN_TILE_HEIGHT = winHeight / TILE_SIZE;


	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(WIN_TILE_WIDTH * WIN_TILE_HEIGHT * 4);



	//Init grid
	for (int x = 0; x < WIN_TILE_WIDTH; x++) {
		mesh.push_back(std::vector<Node>(WIN_TILE_HEIGHT));
		for (int y = 0; y < WIN_TILE_HEIGHT; y++) {
			if (x == 0 || x == WIN_TILE_WIDTH - 1) {
				mesh[x][y] = Node(100.0f, ALUMINUM, true);
			}
			else if ( y == 0 || y == WIN_TILE_HEIGHT - 1) {
				mesh[x][y] = Node(100.0f, ALUMINUM, true);
			}
			//else if (y <= 350/2) {
			//	mesh[x][y] = Node(1000.0f, PYRITE, false);
			//}
			else
				mesh[x][y] = Node(100.0f, PYRITE,false);
		}
	}

	initVerticies();

}


meshSystem::~meshSystem()
{
}

void meshSystem::update()
{
	if (!SIMULATION_PAUSED) {
		for (int x = 0; x < WIN_TILE_WIDTH; x++) {
			for (int y = 0; y < WIN_TILE_HEIGHT; y++) {

				float t0 = mesh[x][y].temperature;

				//Get surrounding node temperatures
				float t0_t, t0_b, t0_l, t0_r;
				if (y - 1 >= 0)
					t0_t = mesh[x][y - 1].temperature;
				else
					t0_t = 0;
				if (y + 1 < WIN_TILE_WIDTH)
					t0_b = mesh[x][y + 1].temperature;
				else
					t0_b = 0;
				if (x - 1 >= 0)
					t0_l = mesh[x - 1][y].temperature;
				else
					t0_l = 0;
				if (x + 1 < WIN_TILE_WIDTH)
					t0_r = mesh[x + 1][y].temperature;
				else
					t0_r = 0;

				//Get alphas
				float a0 = mesh[x][y].alpha;
				float a_t, a_b, a_l, a_r;
				if (y - 1 >= 0)
					a_t = mesh[x][y - 1].alpha;
				else
					a_t = 0;
				if (y + 1 < WIN_TILE_WIDTH)
					a_b = mesh[x][y + 1].alpha;
				else
					a_b = 0;
				if (x - 1 >= 0)
					a_l = mesh[x - 1][y].alpha;
				else
					a_l = 0;
				if (x + 1 < WIN_TILE_WIDTH)
					a_r = mesh[x + 1][y].alpha;
				else
					a_r = 0;

				//Need to fix boundary conditions

				//Component considering constant alpha
				 float constAlphaComponent= a0*(t0_t + t0_b + t0_l + t0_r - 4 * t0) ;

				//component taking into account non constant alpha
				 //Change this to use central distance
				float nonConstAlphaComponent = (t0_r - t0)*(a_r - a0) + (t0_b - t0) * (a_b - a0);
				 //float nonConstAlphaComponent = 0.25f*((t0_r - t0_l)*(a_r - a_l) + (t0_b - t0_t) * (a_b - a_t));

				mesh[x][y].nextTemperature = t0 + (dTime*timeMultiplier*(nonConstAlphaComponent + constAlphaComponent) / pow(h, 2.0f));
				updateTileColor(x, y);
			}
		}
	}

	//Update all temperatures to newly calculated values
	for (int x = 1; x < WIN_TILE_WIDTH - 1; x++) {
		for (int y = 1; y < WIN_TILE_HEIGHT - 1; y++) {

			mesh[x][y].temperature = mesh[x][y].nextTemperature;
		}
	}
	


}

//Initialize all verticies to their correct color and set up their positions
void meshSystem::initVerticies()
{
	for (int x = 0; x < WIN_TILE_WIDTH; x++) {
		for (int y = 0; y < WIN_TILE_HEIGHT; y++) {
			sf::Vertex* quad = getQuad(x, y);

			// define its 4 corners
			quad[0].position = sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
			quad[1].position = sf::Vector2f(x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE);
			quad[2].position = sf::Vector2f(x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE);
			quad[3].position = sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE + TILE_SIZE);

			//Set colors
			updateTileColor(x, y);


		}
	}
}

//Update the color of a tile given x,y coords in tile coordinates
void meshSystem::updateTileColor(int x, int y)
{
	sf::Color color;

	
	if (SHOW_MATERIAL) {
	
		if (mesh[x][y].alpha == 1.22e-3f)
			color = COLOR_MAP[PYRITE];
		else
			color = COLOR_MAP[ALUMINUM];
	}
	else {
		if (mesh[x][y].isBorder)
			color = tempToColor(-1.0f);
		else
			color = tempToColor(mesh[x][y].nextTemperature);
	}
	


	
	sf::Vertex* quad = getQuad(x, y);

	//Set colors
	// define its 4 corners
	quad[0].color = color;
	quad[1].color = color;
	quad[2].color = color;
	quad[3].color = color;
}


//Gets the temperature of a tile from mouse coords (pixels)
float meshSystem::getTempFromMouse(int x, int y)
{
	int xT = (x - (x % TILE_SIZE)) / TILE_SIZE;
	int yT = (y - (y % TILE_SIZE)) / TILE_SIZE;

	return mesh[xT][yT].temperature;

}

//Used for brush, changes the temperature and material of the nodes in the area of the brush
// x,y in mouse coords (pixels)
void meshSystem::setTemperature(int x, int y, float temperature, Material type, int brushSize)
{
	int xT = (x - (x % TILE_SIZE)) / TILE_SIZE;
	int yT = (y - (y % TILE_SIZE)) / TILE_SIZE;

	for (int i = 0; i < brushSize; i++) {
		for (int j = 0; j < brushSize; j++) {
			if (SIMULATION_PAUSED)
				mesh[xT + i - brushSize / 2][yT + j - brushSize / 2].nextTemperature = temperature;
			else
				mesh[xT + i - brushSize / 2][yT + j - brushSize / 2].temperature = temperature;
			mesh[xT + i - brushSize / 2][yT + j - brushSize / 2].alpha = ALPHA_MAP[type];
			updateTileColor(xT + i - brushSize / 2, yT + j - brushSize / 2);

		}

	}
}

//Updates the color of all the tiles on the screen
void meshSystem::refreshColors()
{
	for (int x = 0; x < WIN_TILE_WIDTH; x++) {
		for (int y = 0; y < WIN_TILE_HEIGHT; y++) {
			updateTileColor(x,y);
		}
	}

}

//Maps temperature within a given range to color gradient
sf::Color meshSystem::tempToColor(float temp)
{
	sf::Color color;

	float max = 1000.0f;
	float min = 0.0f;
	float mid = (max - min) / 2.0f;

	if (USE_GRAY_SCALE_TEMP) {

		int val = (int)((255.0f / (max - min))*(temp - min));

		color = sf::Color(val, val, val);
	}
	else {

		
		float r, g, b;

		if (temp <= mid) {
			r = 0.0f;
			g = (255.0f / (mid - min))*(temp - min);
			b = 255.0f - g;
		}
		else {
			r = (255.0f / (max - mid))*(temp - mid);
			g = 255.0f - r;
			b = 0.0f;
		}



		color = sf::Color((int)r, (int)g, (int)b);


	}
	if (temp == -1.0f)
		color = sf::Color::Magenta;
	return color;
}

//Gets the sub array of the verticies in a quad at a given x,y position
sf::Vertex * meshSystem::getQuad(int x, int y)
{

	return &m_vertices[(x + y * WIN_TILE_WIDTH) * 4];
	//return nullptr;
}
