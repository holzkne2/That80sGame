//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "UIImageRenderer.h"
#include "UITextRenderer.h"
#include "ModelRenderer.h"

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game* Game::s_instance;

Game::Game()
{
	Game::s_instance = this;

    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);

	m_gamePad = std::make_unique<GamePad>();
	m_scene = std::make_unique<Scene>();
	m_physicsManager = std::make_unique<PhysicsManager>();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

	m_physicsManager->Initialize();

	m_scene->LoadScene(0);

    // Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:    
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());
    // Add your game logic here.
    elapsedTime;

	m_physicsManager->Tick(elapsedTime);

	m_scene->Update();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

	m_scene->Render();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::Black);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // Game is becoming active window.
	m_gamePad->Resume();
	m_buttons.Reset();
}

void Game::OnDeactivated()
{
    // Game is becoming background window.
	m_gamePad->Suspend();
}

void Game::OnSuspending()
{
    // Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // Game is being power-resumed (or returning from minimize).
	m_buttons.Reset();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // Change to desired default window size (note minimum size is 320x200).
    width = 1080;
    height = 720;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

    // Initialize device dependent objects here (independent of window size).
    device;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // Add Direct3D resource cleanup here.
	m_scene->OnDeviceLost();
}

void Game::OnDeviceRestored()
{
	// Rebuild Scene

    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
