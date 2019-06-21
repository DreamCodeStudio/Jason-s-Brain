#pragma once
#include <iostream>

#include <irrlicht.h>

#include "World/World.h"

class Core
{
	public:

		Core();

		void Run();
		bool IsOpen();

	private:

		void Update();
		void Render();

		irr::IrrlichtDevice* _device;
		irr::video::IVideoDriver* _driver;
		irr::scene::ISceneManager* _manager;
		irr::scene::ICameraSceneNode* _camera;

		World _world;
};