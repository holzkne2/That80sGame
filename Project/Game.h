//
// Game.h
//

#pragma once

#include <vector>

#include "DeviceResources.h"
#include "StepTimer.h"

#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"
#include "PhysicsManager.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify
{
public:

	Game();

	// Initialization and management
	void Initialize(HWND window, int width, int height);

	// Basic game loop
	void Tick();

	// IDeviceNotify
	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	// Properties
	void GetDefaultSize(int& width, int& height) const;

	DX::DeviceResources* GetDeviceResources() { return m_deviceResources.get(); }
	Scene* GetScene() { return m_scene.get(); }

	DX::StepTimer* GetTimer() { return &m_timer; }

private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	// Device resources.
	std::unique_ptr<DX::DeviceResources>    m_deviceResources;

	// Rendering loop timer.
	DX::StepTimer                           m_timer;

	std::unique_ptr<DirectX::GamePad> m_gamePad;

	std::unique_ptr<Scene> m_scene;

	std::unique_ptr<PhysicsManager> m_physicsManager;

	// Singleton
public:
	static Game* Get()
	{
		return s_instance;
	}
private:
	static Game* s_instance;
};