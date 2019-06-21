#pragma once
#include <iostream>
#include <ctime>
#include <vector>
#include <Windows.h>

#include <irrlicht.h>

#define WORLD_SIZE 100
#define MAX_LIFE_VALUE 5

class World
{
	public:

		//Create method initalizes the world
		void Create(irr::scene::ISceneManager *manager);

		//Calculates the next frame
		void Update();


	private:

		void CreateWorldArray();
		void InitializeArray();
		int GetNeighbour(int x, int y, int z);
		void DeleteGraphics();

		irr::scene::ISceneManager* _manager;
		irr::scene::ILightSceneNode* _light;

		int*** _world;
		int*** _worldBuffer;
		std::vector<irr::scene::IAnimatedMeshSceneNode*> _graphics;
		irr::scene::IAnimatedMesh* _meshes[5];
		int _frames;

};