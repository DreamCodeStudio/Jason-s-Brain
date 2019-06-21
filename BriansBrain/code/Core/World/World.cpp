#include "World.h"

void World::Create(irr::scene::ISceneManager *manager)
{
	_manager = manager;

	//Create the array and initialize the world
	this->CreateWorldArray();
	this->InitializeArray();

	//Clear the graphics vector at the beginning
	_graphics.clear();

	//Load meshes
	_meshes[4] = _manager->getMesh("Assets\\State5\\State5.obj");
	_meshes[3] = _manager->getMesh("Assets\\State4\\State4.obj");
	_meshes[2] = _manager->getMesh("Assets\\State3\\State3.obj");
	_meshes[1] = _manager->getMesh("Assets\\State2\\State2.obj");
	_meshes[0] = _manager->getMesh("Assets\\State1\\State1.obj");

	//Add a light scene node to the world. The light will follow the player
	_light = _manager->addLightSceneNode(0, irr::core::vector3df(0, 0, 0), irr::video::SColor(255, 255, 255, 255), 250, -1);

	_frames = 0;
}

void World::Update()
{
	_frames++; //Count frames

	//Run through the whole world and calculate the development of every block, depending on its neighbours
	for (register unsigned int x = 2; x < WORLD_SIZE - 2; x++)
	{
		for (register unsigned int y = 2; y < WORLD_SIZE - 2; y++)
		{
			for (register unsigned int z = 2; z < WORLD_SIZE - 2; z++)
			{
				//Get the amount of neighbours for the specific cell
				int neighbours = this->GetNeighbour(x, y, z);

				//If a dead cell has 4 or 8 neighbours it gets born here
				if (_world[x][y][z] == 0 && (neighbours == 4 || neighbours == 8))
				{
					_worldBuffer[x][y][z] = MAX_LIFE_VALUE;
				}
				//If a alive cell has not 4 neighbours its LIFE_VALUE gets lowerd by one
				else if (_world[x][y][z] > 0 && neighbours != 4)
				{
					_worldBuffer[x][y][z] = _world[x][y][z] - 1;
				}
				//If a alive cell has 4 neighbours nothing happens to it
				else if (_world[x][y][z] > 0 && neighbours == 4)
				{
					_worldBuffer[x][y][z] = _world[x][y][z];
				}
				//If a dead cell has not 4 or 8 neighbours it remains dead
				else if (_world[x][y][z] == 0 && neighbours != 4 && neighbours != 8)
				{
					_worldBuffer[x][y][z] = 0;
				}
			}
		}
	}

	//Once every frame clear the all the graphics
	this->DeleteGraphics();

	//Copy the WorldBuffer to the world array and create the graphics
	for (register unsigned int x = 0; x < WORLD_SIZE; x++)
	{
		for (register unsigned int y = 0; y < WORLD_SIZE; y++)
		{
			for (register unsigned int z = 0; z < WORLD_SIZE; z++)
			{
				//Copy the buffer 
				_world[x][y][z] = _worldBuffer[x][y][z];
				_worldBuffer[x][y][z] = 0;

				//Create graphics here. Depending on the different cell states, a different mesh is used to display the blocks
				if (_world[x][y][z] > 0)
				{
					_graphics.push_back(_manager->addAnimatedMeshSceneNode(_meshes[_world[x][y][z] - 1], 0, -1, irr::core::vector3df(x, y, z)));
				}
			}
		}
	}

	//If the user presses the space key or once every 5 frames, we will spawn some random blocks on a random location
	//to keep the whole world alive
	if (GetAsyncKeyState(VK_SPACE) || _frames == 5)
	{
		this->InitializeArray();
		_frames = 0;
	}

	//Follow the player with the light
	_light->setPosition(_manager->getActiveCamera()->getAbsolutePosition());
}

void World::CreateWorldArray()
{
	srand(time(NULL));

	//Create 2 World arrays here. The first one holds the current data, while the second one is used
	//to calculate the next frame. The arrays are swapped after the calculation
	_world = new int** [WORLD_SIZE];
	for (unsigned int i = 0; i < WORLD_SIZE; i++)
	{
		_world[i] = new int* [WORLD_SIZE];
	}
	for (unsigned int x = 0; x < WORLD_SIZE; x++)
	{
		for (unsigned int y = 0; y < WORLD_SIZE; y++)
		{
			_world[x][y] = new int[WORLD_SIZE];
		}
	}
	for (unsigned int x = 0; x < WORLD_SIZE; x++)
	{
		for (unsigned int y = 0; y < WORLD_SIZE; y++)
		{
			for (unsigned int z = 0; z < WORLD_SIZE; z++)
			{
				_world[x][y][z] = 0;
			}
		}
	}

	//Create the 2nd Array here
	_worldBuffer = new int** [WORLD_SIZE];
	for (unsigned int i = 0; i < WORLD_SIZE; i++)
	{
		_worldBuffer[i] = new int* [WORLD_SIZE];
	}
	for (unsigned int x = 0; x < WORLD_SIZE; x++)
	{
		for (unsigned int y = 0; y < WORLD_SIZE; y++)
		{
			_worldBuffer[x][y] = new int[WORLD_SIZE];
		}
	}
	for (unsigned int x = 0; x < WORLD_SIZE; x++)
	{
		for (unsigned int y = 0; y < WORLD_SIZE; y++)
		{
			for (unsigned int z = 0; z < WORLD_SIZE; z++)
			{
				_worldBuffer[x][y][z] = 0;
			}
		}
	}
}

void World::InitializeArray()
{
	//First get a random spawn area in our WORLD_SIZE Cube
	int randX = rand() % (WORLD_SIZE - 8) + 4;
	int randY = rand() % (WORLD_SIZE - 8) + 4;
	int randZ = rand() % (WORLD_SIZE - 8) + 4;

	//Run through the whole spawn area. 
	for (register unsigned int x = randX - 4; x < randX + 4; x++)
	{
		for (register unsigned int y = randY - 4; y < randY + 4; y++)
		{
			for (register unsigned int z = randZ - 4; z < randZ + 4; z++)
			{
				//Every block in the area has a 30% chance to develop life
				int randomValue = rand() % 100;
				if (randomValue < 30)
				{
					_world[x][y][z] = MAX_LIFE_VALUE;
				}
			}
		}
	}
}

int World::GetNeighbour(int x, int y, int z)
{
	int counter = 0;

	//Run through the neighbours of the given cell
	for (register unsigned int xr = x - 1; xr <= x + 1; xr++)
	{
		for (register unsigned int yr = y - 1; yr <= y + 1; yr++)
		{
			for (register unsigned int zr = z - 1; zr <= z + 1; zr++)
			{
				//We only count cells, which have MAX_LIFE 
				if (_world[xr][yr][zr] == MAX_LIFE_VALUE)
				{
					//Add the cell to the neighbours
					counter++;
				}
			}
		}
	}

	//If the cell itself is alive, we counted it to the vector. So we need to sustract, because the cell can not be its own neighbour
	if (_world[x][y][z] > 0)
	{
		counter--;
	}

	//Return the number of neighbours
	return counter;
}

void World::DeleteGraphics()
{
	//Remove every block from the scene manager
	for (register unsigned int i = 0; i < _graphics.size(); i++)
	{
		_graphics[i]->remove();
	}

	//Clear the graphics vector
	_graphics.clear();
}