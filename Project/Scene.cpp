#include "pch.h"
#include "Scene.h"
#include "DeviceResources.h"
#include "Game.h"
#include "UIImageRenderer.h"
#include "UITextRenderer.h"
#include "ModelRenderer.h"
#include "ShipController.h"
#include "CameraFollow.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Scene::Scene()
{
	m_gameObjects = std::vector<std::unique_ptr<GameObject>>();
	m_modelRenderers = std::vector<ModelRenderer*>();
	m_cameras = std::vector<Camera*>();
}


Scene::~Scene()
{
}

void Scene::LoadScene(int sceneIndex)
{
	UnloadCurrentScene();

	if (sceneIndex == 0)
		LoadScene0();
	else if (sceneIndex == 1)
		LoadScene1();
}

void Scene::LoadScene0()
{
	DX::DeviceResources* deviceResources = Game::Get()->GetDeviceResources();

	m_states = std::make_unique<CommonStates>(deviceResources->GetD3DDevice());
	m_spriteBatch = std::make_unique<SpriteBatch>(deviceResources->GetD3DDeviceContext());

	///
	/// Title
	///
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>();
	UIImageRenderer* uiImageRenderer = gameObject->AddComponent<UIImageRenderer>();
	RectTransform* rectTransform = dynamic_cast<RectTransform*>(gameObject->GetTransform());
	uiImageRenderer->SetImage(deviceResources->GetD3DDevice(), L"GameTitle.png");

	rectTransform->SetPivot(Vector2(uiImageRenderer->GetWidth() / 2.0, uiImageRenderer->GetHeight() / 2.0));
	rectTransform->SetPosition(Vector3(0, 0, 0));
	rectTransform->SetScale(0.75);
	rectTransform->SetAnchors(Vector2(0.5, 0.25));

	AddGameObject(gameObject);

	///
	/// Select Arrow
	///
	gameObject = std::make_unique<GameObject>();
	uiImageRenderer = gameObject->AddComponent<UIImageRenderer>();
	rectTransform = dynamic_cast<RectTransform*>(gameObject->GetTransform());
	uiImageRenderer->SetImage(deviceResources->GetD3DDevice(), L"Arrow.png");
	uiImageRenderer->SetColor(Colors::Red);

	rectTransform->SetPivot(Vector2(uiImageRenderer->GetWidth() / 2.0, uiImageRenderer->GetHeight() / 2.0));
	rectTransform->SetPosition(Vector3(0, 0, 0));
	rectTransform->SetScale(1);
	rectTransform->SetAnchors(Vector2(0.45, 0.66));

	AddGameObject(gameObject);

	///
	/// Play Button
	///
	gameObject = std::make_unique<GameObject>();
	UITextRenderer* uiTextRenderer = gameObject->AddComponent<UITextRenderer>();
	rectTransform = dynamic_cast<RectTransform*>(gameObject->GetTransform());
	uiTextRenderer->SetFont(deviceResources->GetD3DDevice(), L"StillTime64.spritefont");
	uiTextRenderer->SetColor(Colors::Cyan);
	uiTextRenderer->SetText(L"Play");

	rectTransform->SetPivot(uiTextRenderer->GetFont()->MeasureString(uiTextRenderer->GetText().c_str()) / 2.0);
	rectTransform->SetPosition(Vector3(0, 0, 0));
	rectTransform->SetScale(1);
	rectTransform->SetAnchors(Vector2(0.5, 0.66));

	AddGameObject(gameObject);

	///
	/// Terrain
	///
	gameObject = std::make_unique<GameObject>();
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"MenuTerrain.cmo");

	AddGameObject(gameObject);

	///
	/// Sun
	///
	gameObject = std::make_unique<GameObject>();
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Sun.cmo", true);
	gameObject->GetTransform()->SetPosition(Vector3(0, 0, 32.4));

	AddGameObject(gameObject);

	///
	/// Ship
	///
	gameObject = std::make_unique<GameObject>();
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Ship01.cmo");
	gameObject->GetTransform()->SetPosition(Vector3(0, 1, 4));

	AddGameObject(gameObject);

	///
	/// Camera
	///
	gameObject = std::make_unique<GameObject>();
	gameObject->AddComponent<Camera>();
	gameObject->GetTransform()->SetPosition(Vector3(-3.3, 1.57, 0));
	gameObject->GetTransform()->SetRotation(
		Quaternion::Euler(Vector3(0, 191.4, 0)));

	AddGameObject(gameObject);
}

void Scene::LoadScene1()
{
	DX::DeviceResources* deviceResources = Game::Get()->GetDeviceResources();

	m_states = std::make_unique<CommonStates>(deviceResources->GetD3DDevice());
	m_spriteBatch = std::make_unique<SpriteBatch>(deviceResources->GetD3DDeviceContext());

	///
	/// Grid
	///
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>();
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"TestGrid500.cmo");
	gameObject->GetTransform()->SetPosition(Vector3(0, 0, 250));

	AddGameObject(gameObject);

	///
	/// Sun
	///
	gameObject = std::make_unique<GameObject>();
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Sun.cmo", true);
	gameObject->GetTransform()->SetPosition(Vector3(0, 0, 500));
	gameObject->GetTransform()->SetScale(3);

	AddGameObject(gameObject);

	///
	/// Ship
	///
	gameObject = std::make_unique<GameObject>();
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Ship01.cmo");
	gameObject->GetTransform()->SetPosition(Vector3(0, 2, 0));
	gameObject->AddComponent<ShipController>();

	AddGameObject(gameObject);
	GameObject* player = m_gameObjects[m_gameObjects.size() - 1].get();

	///
	/// Camera
	///
	gameObject = std::make_unique<GameObject>();
	gameObject->AddComponent<Camera>();
	gameObject->GetTransform()->SetPosition(Vector3(0, 3.33, -3));
	gameObject->GetTransform()->SetRotation(
		Quaternion::Euler(Vector3(0, 180, 0)));
	gameObject->AddComponent<CameraFollow>()->SetTarget(player->GetTransform());

	AddGameObject(gameObject);
}

void Scene::UnloadCurrentScene()
{
	m_gameObjects.clear();
	m_modelRenderers.clear();
	m_cameras.clear();
}

void Scene::Update()
{
	DX::DeviceResources* deviceResources = Game::Get()->GetDeviceResources();

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->UpdateComponents();
	}

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->LateUpdateComponents();
	}

	auto state = GamePad::Get().GetState(0, GamePad::DEAD_ZONE_CIRCULAR);

	if (state.IsConnected())
	{
		if (state.IsStartPressed())
		{
			LoadScene(1);
		}
		if (state.IsBackPressed())
		{
			LoadScene(0);
		}
	}

	//m_view = Matrix::CreateLookAt(Vector3(-3.3, 1.57, 0), Vector3(-2.31, 1.57, 4.9), Vector3::UnitY);
}

void Scene::Render()
{
	DX::DeviceResources* deviceResources = Game::Get()->GetDeviceResources();

	deviceResources->PIXBeginEvent(L"Render");
	auto context = deviceResources->GetD3DDeviceContext();
	// TODO: Add your rendering code here.
	context;

	GameObject* gameObject;
	ModelRenderer* modelRenderer;
	Camera* camera;
	for (unsigned int c = 0; c < m_cameras.size(); c++)
	{
		camera = m_cameras[c];
		if (camera == nullptr)
			continue;
		Matrix view = camera->GetViewMatrix();
		Matrix projection = camera->GetProjectionMatrix(deviceResources->GetOutputSize());
		for (unsigned int i = 0; i < m_modelRenderers.size(); i++)
		{
			modelRenderer = m_modelRenderers[i];
			if (modelRenderer != nullptr)
				modelRenderer->Render(context, m_states.get(), view, projection);
		}
	}
	//for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	//{
	//	gameObject = m_gameObjects[i].get();
	//	if (gameObject->GetComponent<ModelRenderer>() != nullptr)
	//	{
	//		gameObject->GetComponent<ModelRenderer>()->Render(context, m_states.get(), m_view, m_proj);
	//	}
	//}

	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		gameObject = m_gameObjects[i].get();
		if (gameObject->GetComponent<UIImageRenderer>() != nullptr)
		{
			gameObject->GetComponent<UIImageRenderer>()->Render(m_spriteBatch.get(), deviceResources->GetOutputSize());
		}
		if (gameObject->GetComponent<UITextRenderer>() != nullptr)
		{
			gameObject->GetComponent<UITextRenderer>()->Render(m_spriteBatch.get(), deviceResources->GetOutputSize());
		}
	}

	m_spriteBatch->End();

	deviceResources->PIXEndEvent();
}

void Scene::OnDeviceLost()
{
	GameObject* gameObject;
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		gameObject = m_gameObjects[i].get();
		if (gameObject->GetComponent<UIImageRenderer>() != nullptr)
			gameObject->GetComponent<UIImageRenderer>()->OnDeviceLost();
		if (gameObject->GetComponent<UITextRenderer>() != nullptr)
			gameObject->GetComponent<UITextRenderer>()->OnDeviceLost();
		if (gameObject->GetComponent<ModelRenderer>() != nullptr)
			gameObject->GetComponent<ModelRenderer>()->OnDeviceLost();
	}
	m_spriteBatch.reset();
	m_states.reset();
}

void Scene::AddGameObject(std::unique_ptr<GameObject>& gameObject)
{
	m_gameObjects.push_back(std::move(gameObject));
}

void Scene::AddModelRenderer(ModelRenderer* modelRenderer)
{
	m_modelRenderers.push_back(modelRenderer);
}

void Scene::AddCamera(Camera* camera)
{
	m_cameras.push_back(camera);
}