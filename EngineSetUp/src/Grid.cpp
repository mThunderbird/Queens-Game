#include "Grid.h"

#include "World.h"

extern World world;

Grid::Grid()
{
	m_hoverGrid = nullptr;
	m_onTurn = 1;
}

Grid::Grid(const Grid* model)
{
	m_dimensions = model->m_dimensions;
	m_borderThickness = model->m_borderThickness;

	m_gridBase.rect = model->m_gridBase.rect;

	m_gridBorder.rect = model->m_gridBorder.rect;
	m_gridBorder.texture = model->m_gridBorder.texture;

	m_gridSquares = model->m_gridSquares;
}

Grid::~Grid()
{
}

void Grid::load(int opponent)
{
	m_opponent = opponent;

	int2 coordinates;

	string temp , player1OnTurn, player2OnTurn, tutorailImg;
	fstream stream;

	stream.open(CONFIG_FOLDER + GAME_FOLDER + "grid.txt");

	stream >> temp >> m_borderThickness;

	stream >> temp >> m_player1OnTurn.rect.x >> m_player1OnTurn.rect.y >> m_player1OnTurn.rect.w >> m_player1OnTurn.rect.h;
	stream >> temp >> player1OnTurn >> player2OnTurn;
	stream >> temp >> m_tutorial.rect.x >> m_tutorial.rect.y >> m_tutorial.rect.w >> m_tutorial.rect.h;
	stream >> temp >> tutorailImg;
	
	stream.close();

	m_player2OnTurn.rect = m_player1OnTurn.rect;

	m_player1OnTurn.texture = loadTexture(GAME_FOLDER + player1OnTurn);
	m_player2OnTurn.texture = loadTexture(GAME_FOLDER + player2OnTurn);
	m_tutorial.texture = loadTexture(GAME_FOLDER + tutorailImg);
	
	m_drawTutorial = true;
	
	m_gridBorder.texture = loadTexture(GAME_FOLDER + "gridBorderTexture.bmp");
	m_unavailableMove.texture = loadTexture(GAME_FOLDER + "unavailableTile.bmp");
	m_hover.texture = loadTexture(GAME_FOLDER + "gridPossMove.bmp");

	m_oddSquareTexture = loadTexture(GAME_FOLDER + "gridOddSquareTexture.bmp");
	m_evenSquareTexture = loadTexture(GAME_FOLDER + "gridEvenSquareTexture.bmp");

	
	int maxDim = max(m_dimensions.x, m_dimensions.y);

	m_squareDimension = (1080 - m_borderThickness / 2)  / maxDim;

	D(m_squareDimension);

	coordinates.x = (1920 - m_dimensions.y * m_squareDimension) / 2;
	coordinates.y = (1080 - m_dimensions.x * m_squareDimension) / 2;

	m_gridBase.rect = { coordinates.x , coordinates.y, m_dimensions.y * m_squareDimension, m_dimensions.x * m_squareDimension };

	m_gridBorder.rect = 
	{	
		m_gridBase.rect.x - m_borderThickness, 
		m_gridBase.rect.y - m_borderThickness, 
		m_gridBase.rect.w + 2 * m_borderThickness, 
		m_gridBase.rect.h + 2 * m_borderThickness 
	};

	m_gridSquares.resize(m_dimensions.x);
	
		
	for (int r = 0; r < m_gridSquares.size(); r++)
	{
		m_gridSquares[r].resize(m_dimensions.y);
		
		for (int c = 0; c < m_gridSquares[r].size(); c++)
		{
			m_gridSquares[r][c].isFree = true;
			m_gridSquares[r][c].rect = { c * m_squareDimension + m_gridBase.rect.x, r * m_squareDimension + m_gridBase.rect.y, m_squareDimension, m_squareDimension };
			
			if (r % 2 == 0)
			{
				if (c % 2 == 0)
				{
					m_gridSquares[r][c].texture = m_evenSquareTexture;
				}
				else
				{
					m_gridSquares[r][c].texture = m_oddSquareTexture;
				}
			}
			else
			{
				if (c % 2 == 0)
				{
					m_gridSquares[r][c].texture = m_oddSquareTexture;
				}
				else
				{
					m_gridSquares[r][c].texture = m_evenSquareTexture;
				}
			}
		}
	}
}

void Grid::draw()
{
	drawObject(m_gridBorder);

	drawGridSquares();

	drawUnavailableMoves();

	drawEntities();

	drawHover();
	
	if (m_drawTutorial)
	{
		drawObject(m_tutorial);
	}
	
	if (m_onTurn == 1)
	{
		drawObject(m_player1OnTurn);
	}
	else if(m_onTurn == 2)
	{
		drawObject(m_player2OnTurn);
	}
}
/*
* used when we want to add an entity
* @param coor - where we want to add an entity (rows, colls)
* @param onTurn - the curr player
*/
void Grid::addEntity(int2 coor, int onTurn)
{
	if (possMove(coor))
	{
		Entity* temp;
		switch (onTurn)
		{
		case 1:
			temp = new Entity(ConfigManager::m_enityModelPlayer1, coor, onTurn);
			break;
		case 2:
			temp = new Entity(ConfigManager::m_enityModelPlayer2, coor, onTurn);
			break;
		case -1:
			temp = new Entity(ConfigManager::m_enityModelEnemy, coor, onTurn);
			break;
		case -2:
			temp = new Entity(ConfigManager::m_enityModelEnemy, coor, onTurn);
			break;
		default:
			break;
		}
		m_entities.push_back(temp);
		
		vector<int2> buff = giveUnavailableMoves(coor, m_dimensions.x, m_dimensions.y);

		for(int i = 0; i < buff.size(); i++)
		{
			m_gridSquares[buff[i].x][buff[i].y].isFree = false;
		}

		m_onTurn = (m_onTurn == m_opponent) ? 1 : m_opponent;

		world.m_soundManager.playSound(SOUND::PLACE_QUEEN);
		
		m_drawTutorial = false;
	}
	else
	{
		cout << "Invalid move" << endl;
	}
}

int Grid::getSquareDimension()
{
	return m_squareDimension;
}

int2 Grid::easyBot(vector<vector<gridSquare>> m_matrix)
{
	vector<int2> freeSquares;

	for (int i = 0; i < m_matrix.size(); i++)
	{
		for (int j = 0; j < m_matrix[i].size(); j++)
		{
			if (m_matrix[i][j].isFree)
			{
				freeSquares.push_back(int2{i,j});
			}
		}
	}
	
	int randomIndex = 0 + rand() % freeSquares.size();

	return freeSquares[randomIndex];
}

int2 Grid::mediumBot(vector<vector<gridSquare>> matrix)
{
	// decide how many moves into the future to check
	
	vector<int2> possibleMoves;

	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (matrix[i][j].isFree)
			{
				possibleMoves.push_back(int2{ i,j });
			}
		}
	}

	if (possibleMoves.size() > 100) return easyBot(matrix);

	AI_move bestMove;
	bestMove.efficiency = matrix.size() * matrix[0].size();
	bestMove.coordinates = possibleMoves[0];

	for (int i = 0; i < possibleMoves.size(); i++)
	{

		vector <vector< gridSquare >> newMatrix;
		newMatrix = matrix;
		vector<int2> unavailable = giveUnavailableMoves(possibleMoves[i], matrix.size(), matrix[0].size());
		int temp = matrix.size() * matrix[0].size();

		for (int j = 0; j < unavailable.size(); j++)
		{
			newMatrix[unavailable[j].x][unavailable[j].y].isFree = false;
		}

		int freeSpots = 0;
		for (int p = 0; p < matrix.size(); p++)
		{
			for (int k = 0; k < matrix[p].size(); k++)
			{
				if (matrix[p][k].isFree)
				{
					freeSpots++;
				}
			}
		}

		if (freeSpots == 0)
		{
			return possibleMoves[i];
		}

		if (checkForPossibleWin(newMatrix))
		{
			temp = matrix.size() * matrix[0].size();
		}
		else
		{
			temp = freeSpots;
		}
		
		if (temp < bestMove.efficiency)
		{
			bestMove.coordinates = possibleMoves[i];
			bestMove.efficiency = temp;
		}
	}
	
	return bestMove.coordinates;
}

bool Grid::checkForPossibleWin(vector<vector<gridSquare>> matrix)
{
	vector<int2> possibleMoves;

	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (matrix[i][j].isFree)
			{
				possibleMoves.push_back(int2{ i,j });
			}
		}
	}
		
	vector<vector<gridSquare>>backup;
	backup = matrix;

	for (int l = 0; l < possibleMoves.size(); l++)
	{
		matrix = backup;
		vector<int2> unavailable = giveUnavailableMoves(possibleMoves[l], matrix.size(), matrix[0].size());

		for (int i = 0; i < unavailable.size(); i++)
		{
			matrix[unavailable[i].x][unavailable[i].y].isFree = false;
		}

		int freeSpots = 0;

		for (int i = 0; i < matrix.size(); i++)
		{
			for (int j = 0; j < matrix[i].size(); j++)
			{
				if (matrix[i][j].isFree)
				{
					freeSpots++;
				}

			}
		}
		
		if (freeSpots == 0)
		{
			return true;
		}
	}
	return false;
}



void Grid::winCondition()
{
	int res = 1;

	for (int r = 0; r < m_gridSquares.size(); r++)
	{
		for (int c = 0; c < m_gridSquares[r].size(); c++)
		{
			if (m_gridSquares[r][c].isFree)
			{
				res = 0;
			}
		}
	}

	if (res != 0)
	{
		res = (m_onTurn == m_opponent) ? 1 : m_opponent;
		m_winner = res;
		SDL_Delay(3000);
		world.m_stateManager.changeGameState(GAME_STATE::WIN_SCREEN);
		destroy();
	}
}

void Grid::destroy()
{
	m_gridSquares.clear();

	m_opponent = 0;
	for (int i = 0; i < m_entities.size(); i++)
	{
		delete m_entities[i];
	}
	m_entities.clear();
}

void Grid::checkForClick()
{
	if (world.m_inputManager.m_mouseOnClick)
	{
		for (int r = 0; r < m_gridSquares.size(); r++)
		{
			for (int c = 0; c < m_gridSquares[r].size(); c++)
			{
				if (MouseIsInRect(world.m_inputManager.m_mouseCoor, m_gridSquares[r][c].rect))
				{
					int2 coor;
					coor.x = r;
					coor.y = c;

					addEntity(coor, m_onTurn);
				}
			}
		}
		for (int i = 0; i < m_entities.size(); i++)
		{
			if (MouseIsInRect(world.m_inputManager.m_mouseCoor, m_entities[i]->getRect()))
			{
				m_currentEntity = m_entities[i];
				m_lastEntityCoordinates = centerOfRect(m_entities[i]->getRect());
			}
		}
	}
}

void Grid::drawGridSquares()
{
	for (int r = 0; r < m_gridSquares.size(); r++)
	{
		for (int c = 0; c < m_gridSquares[r].size(); c++)
		{
			drawObject(m_gridSquares[r][c]);
		}
	}

}

void Grid::drawEntities()
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		m_entities[i]->draw();
	}
}

void Grid::onHover()
{
	m_hoverGrid = nullptr;
	for (int r = 0; r < m_gridSquares.size(); r++)
	{
		for (int c = 0; c < m_gridSquares[r].size(); c++)
		{
			if (MouseIsInRect(world.m_inputManager.m_mouseCoor, m_gridSquares[r][c].rect))
			{
				m_hoverGrid = &m_gridSquares[r][c];
			}
		}
	}
}

void Grid::drawHover()
{
	if (m_hoverGrid != nullptr)
	{
		m_hover.rect = m_hoverGrid->rect;
		drawObject(m_hover);
	}
}

void Grid::drawUnavailableMoves()
{
	for(gridSquare* gs : m_unavailableMoves)
	{
		m_unavailableMove.rect = gs->rect;
		drawObject(m_unavailableMove);
	}
}

void Grid::calcUnavailableMoves()
{
	m_unavailableMoves.clear();
	for (int r = 0; r < m_gridSquares.size(); r++)
	{
		for (int c = 0; c < m_gridSquares[r].size(); c++)
		{
			if (m_gridSquares[r][c].isFree == false)
			{
				m_unavailableMoves.push_back(&m_gridSquares[r][c]);
			}
		}
	}
}

/*
* check for valid move when we add entity
* @param coor - the coordinates of the square we are checking
* @return true if the square is free, false if it is not
*/
bool Grid::possMove(int2 coor)
{
	if (inGrid(coor, m_dimensions.x, m_dimensions.y))
	{
		if (m_gridSquares[coor.x][coor.y].isFree)
		{
			return true;
		}
	}
	return false;
}

void Grid::update()
{
	onHover();

	if (m_onTurn == -1)
	{
		SDL_Delay(2000);
		addEntity(easyBot(m_gridSquares), m_onTurn);
	}
	else if (m_onTurn == -2)
	{
		SDL_Delay(1000);
		addEntity(mediumBot(m_gridSquares), m_onTurn);
	}
	else
	{
		checkForClick();
	}
	calcUnavailableMoves();

	winCondition();
}
