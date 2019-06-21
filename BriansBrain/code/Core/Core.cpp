#include "Core.h"

Core::Core()
{
	_device = irr::createDevice(irr::video::EDT_DIRECT3D9, irr::core::dimension2d<irr::u32>(1920, 1080), 16U, false, true, true);
	_driver = _device->getVideoDriver();
	_manager = _device->getSceneManager();

	_camera = _manager->addCameraSceneNodeMaya(0, -1500, 200, 1500, -1, 150, true);
	_camera->setTarget(irr::core::vector3df(50, 50, 50));

	irr::scene::ISceneNodeAnimator* animator = _manager->createFlyCircleAnimator(irr::core::vector3df(50, 50, 50), 140, 0.0001);
	//_camera->addAnimator(animator);

	_world.Create(_manager);
}

void Core::Run()
{
	this->Update();
	this->Render();
}

bool Core::IsOpen()
{
	return _device->run();
}

void Core::Update()
{
	_world.Update();
}

void Core::Render()
{
	_driver->beginScene(true, true, irr::video::SColor(100, 100, 100, 100));

	_manager->drawAll();

	_driver->endScene();
}