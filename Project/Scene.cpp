#include "pch.h"
#include "Scene.h"
#include "DeviceResources.h"
#include "Game.h"
#include "UIImageRenderer.h"
#include "UITextRenderer.h"
#include "ModelRenderer.h"
#include "ShipController.h"
#include "CameraFollow.h"
#include "TrackManager.h"
#include "BoxCollider.h"
#include "DebugDraw.h"

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

void Scene::Init()
{
	DX::DeviceResources* deviceResources = Game::Get()->GetDeviceResources();

	m_states = std::make_unique<CommonStates>(deviceResources->GetD3DDevice());
	m_spriteBatch = std::make_unique<SpriteBatch>(deviceResources->GetD3DDeviceContext());

	m_debugEffect = std::make_unique<BasicEffect>(deviceResources->GetD3DDevice());
	m_debugEffect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_debugEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	DX::ThrowIfFailed(
		deviceResources->GetD3DDevice()->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_debugInputLayout.ReleaseAndGetAddressOf()));

	m_debugBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(deviceResources->GetD3DDeviceContext());
}

void Scene::LoadScene0()
{
	DX::DeviceResources* deviceResources = Game::Get()->GetDeviceResources();

	Init();

	///
	/// Title
	///
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>("Menu Title");
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
	gameObject = std::make_unique<GameObject>("Select Arrow");
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
	gameObject = std::make_unique<GameObject>("Play Button");
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
	gameObject = std::make_unique<GameObject>("Terrain");
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"MenuTerrain.cmo");

	AddGameObject(gameObject);

	///
	/// Sun
	///
	gameObject = std::make_unique<GameObject>("Sun");
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Sun.cmo", true);
	gameObject->GetTransform()->SetPosition(Vector3(0, 0, 32.4));

	AddGameObject(gameObject);

	///
	/// Ship
	///
	gameObject = std::make_unique<GameObject>("Ship");
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Ship01.cmo");
	gameObject->GetTransform()->SetPosition(Vector3(0, 1, 4));
	gameObject->AddComponent<BoxCollider>()->Init(Vector3(0.5, 0.5, 0.5), 1, false, collisiontypes::COL_NOTHING, collisiontypes::COL_NOTHING);

	AddGameObject(gameObject);

	///
	/// Camera
	///
	gameObject = std::make_unique<GameObject>("Camera");
	gameObject->AddComponent<Camera>();
	gameObject->GetTransform()->SetPosition(Vector3(-3.3, 1.57, 0));
	gameObject->GetTransform()->SetRotation(
		Quaternion::Euler(Vector3(0, 191.4, 0)));

	AddGameObject(gameObject);
}

void Scene::LoadScene1()
{
	DX::DeviceResources* deviceResources = Game::Get()->GetDeviceResources();

	Init();

	///
	/// Track Manager
	///
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>("Track Manager");
	TrackManager* trackManager = gameObject->AddComponent<TrackManager>();

	AddGameObject(gameObject);

	GameObject* tempParent;

	///
	/// Grid
	///
	gameObject = std::make_unique<GameObject>("Grid");
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"TestGrid500.cmo");
	gameObject->GetTransform()->SetPosition(Vector3(0, 0, 250));

	tempParent = gameObject.get();
	trackManager->AddObject(gameObject.get());
	AddGameObject(gameObject);

	///
	/// Tower
	///
	GameObject* last;

	gameObject = std::make_unique<GameObject>("Tower");
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Tower01.cmo");
	gameObject->GetTransform()->SetParent(tempParent->GetTransform());
	gameObject->GetTransform()->SetPosition(Vector3(3, 0, 120));
	last = gameObject.get();

	AddGameObject(gameObject);

	gameObject = std::make_unique<GameObject>("Tower Collider");
	gameObject->GetTransform()->SetParent(last->GetTransform());
	gameObject->GetTransform()->SetLocalPosition(Vector3(0, 2.25, 0));
	gameObject->AddComponent<BoxCollider>()->Init(Vector3(2, 2.25, 2), 0, true, collisiontypes::COL_WALL, collisiontypes::COL_SHIP);

	AddGameObject(gameObject);

	gameObject = std::make_unique<GameObject>("Tower 1");
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Tower01.cmo");
	gameObject->GetTransform()->SetParent(tempParent->GetTransform());
	gameObject->GetTransform()->SetPosition(Vector3(-2.5, 0, 90));
	last = gameObject.get();

	AddGameObject(gameObject);

	gameObject = std::make_unique<GameObject>("Tower Collider");
	gameObject->GetTransform()->SetParent(last->GetTransform());
	gameObject->GetTransform()->SetLocalPosition(Vector3(0, 2.25, 0));
	gameObject->AddComponent<BoxCollider>()->Init(Vector3(2, 2.25, 2), 0, true, collisiontypes::COL_WALL, collisiontypes::COL_SHIP);

	AddGameObject(gameObject);

	gameObject = std::make_unique<GameObject>("Tower 2");
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Tower01.cmo");
	gameObject->GetTransform()->SetParent(tempParent->GetTransform());
	gameObject->GetTransform()->SetPosition(Vector3(0.1, -1.5, 175));
	last = gameObject.get();

	AddGameObject(gameObject);

	gameObject = std::make_unique<GameObject>("Tower Collider");
	gameObject->GetTransform()->SetParent(last->GetTransform());
	gameObject->GetTransform()->SetLocalPosition(Vector3(0, 2.25, 0));
	gameObject->AddComponent<BoxCollider>()->Init(Vector3(2, 2.25, 2), 0, true, collisiontypes::COL_WALL, collisiontypes::COL_SHIP);

	AddGameObject(gameObject);

	///
	/// Sun
	///
	gameObject = std::make_unique<GameObject>("Sun");
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Sun.cmo", true);
	gameObject->GetTransform()->SetPosition(Vector3(0, 0, 500));
	gameObject->GetTransform()->SetScale(5);

	AddGameObject(gameObject);

	///
	/// GameOver
	///
	gameObject = std::make_unique<GameObject>("Game Over");
	UIImageRenderer* uiImageRenderer = gameObject->AddComponent<UIImageRenderer>();
	uiImageRenderer->SetImage(deviceResources->GetD3DDevice(), L"GameOver.png");
	RectTransform* rectTransform = dynamic_cast<RectTransform*>(gameObject->GetTransform());
	rectTransform->SetPivot(Vector2(uiImageRenderer->GetWidth() / 2.0, uiImageRenderer->GetHeight() / 2.0));
	rectTransform->SetScale(0.33);
	rectTransform->SetAnchors(Vector2(0.5, 0.5));
	last = gameObject.get();
	gameObject->SetActive(false);

	AddGameObject(gameObject);

	///
	/// Ship
	///
	gameObject = std::make_unique<GameObject>("Ship");
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Ship01.cmo");
	gameObject->GetTransform()->SetPosition(Vector3(0, 2, 0));
	gameObject->AddComponent<ShipController>();
	gameObject->AddComponent<BoxCollider>()->Init(Vector3(0.5, 0.25, 0.5), 1, false, collisiontypes::COL_SHIP, collisiontypes::COL_WALL | collisiontypes::COL_CONTRAINT);
	gameObject->GetComponent<ShipController>()->SetGameOverUI(last);

	AddGameObject(gameObject);
	GameObject* player = m_gameObjects[m_gameObjects.size() - 1].get();

	///
	/// Constraint Box
	///
	gameObject = std::make_unique<GameObject>("Top");
	gameObject->GetTransform()->SetPosition(Vector3(0, 5, 0));
	gameObject->AddComponent<BoxCollider>()->Init(Vector3(3, 0.25, 0.5), 0, true, collisiontypes::COL_CONTRAINT, collisiontypes::COL_SHIP);

	AddGameObject(gameObject);

	gameObject = std::make_unique<GameObject>("Bottom");
	gameObject->GetTransform()->SetPosition(Vector3(0, 1.25, 0));
	gameObject->AddComponent<BoxCollider>()->Init(Vector3(3, 0.25, 0.5), 0, true, collisiontypes::COL_CONTRAINT, collisiontypes::COL_SHIP);

	AddGameObject(gameObject);

	gameObject = std::make_unique<GameObject>("Left");
	gameObject->GetTransform()->SetPosition(Vector3(3, 2.5, 0));
	gameObject->AddComponent<BoxCollider>()->Init(Vector3(0.25, 3, 0.5), 0, true, collisiontypes::COL_CONTRAINT, collisiontypes::COL_SHIP);

	AddGameObject(gameObject);

	gameObject = std::make_unique<GameObject>("Right");
	gameObject->GetTransform()->SetPosition(Vector3(-3, 2.5, 0));
	gameObject->AddComponent<BoxCollider>()->Init(Vector3(0.25, 3, 0.5), 0, true, collisiontypes::COL_CONTRAINT, collisiontypes::COL_SHIP);

	AddGameObject(gameObject);

	///
	/// Camera
	///
	gameObject = std::make_unique<GameObject>("Camera");
	gameObject->AddComponent<Camera>();
	gameObject->GetTransform()->SetPosition(Vector3(0, 3.33, -3));
	gameObject->GetTransform()->SetRotation(
		Quaternion::Euler(Vector3(0, 180, 0)));
	gameObject->AddComponent<CameraFollow>()->SetTarget(player->GetTransform());

	AddGameObject(gameObject);
}

void Scene::UnloadCurrentScene()
{
	Game::Get()->GetPhysicsManager()->Clear();
	m_gameObjects.clear();
	m_modelRenderers.clear();
	m_cameras.clear();
}

void Scene::Update()
{
	DX::DeviceResources* deviceResources = Game::Get()->GetDeviceResources();

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i]->IsActive())
			m_gameObjects[i]->UpdateComponents();
	}

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i]->IsActive())
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
		if (camera == nullptr || !camera->IsActive())
			continue;
		Matrix view = camera->GetViewMatrix();
		Matrix projection = camera->GetProjectionMatrix(deviceResources->GetOutputSize());
		for (unsigned int i = 0; i < m_modelRenderers.size(); i++)
		{
			modelRenderer = m_modelRenderers[i];
			if (modelRenderer != nullptr && modelRenderer->IsActive())
				modelRenderer->Render(context, m_states.get(), view, projection);
		}
	}

	// TODO: Cache
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		gameObject = m_gameObjects[i].get();

		if (gameObject->GetComponent<UIImageRenderer>() != nullptr && gameObject->GetComponent<UIImageRenderer>()->IsActive())
		{
			gameObject->GetComponent<UIImageRenderer>()->Render(m_spriteBatch.get(), deviceResources->GetOutputSize());
		}
		if (gameObject->GetComponent<UITextRenderer>() != nullptr && gameObject->GetComponent<UITextRenderer>()->IsActive())
		{
			gameObject->GetComponent<UITextRenderer>()->Render(m_spriteBatch.get(), deviceResources->GetOutputSize());
		}
	}

	m_spriteBatch->End();

	// Debug Lines
	m_debugEffect->SetWorld(Matrix::Identity);
	m_debugEffect->Apply(context);
	context->IASetInputLayout(m_debugInputLayout.Get());
	m_debugBatch->Begin();

	std::vector<DebugDraw::Line>* lines = Game::Get()->GetPhysicsManager()->GetDebugDraw()->GetLines();
	VertexPositionColor from;
	VertexPositionColor to;
	for (unsigned int c = 0; c < m_cameras.size(); c++)
	{
		camera = m_cameras[c];
		if (camera == nullptr || !camera->IsActive())
			continue;
		Matrix view = camera->GetViewMatrix();
		Matrix projection = camera->GetProjectionMatrix(deviceResources->GetOutputSize());
		m_debugEffect->SetView(view);
		m_debugEffect->SetProjection(projection);
		
		for (unsigned int l = 0; l < lines->size(); l++)
		{
			lines->at(l).GetAsVertexPositionColors(from, to);
			m_debugBatch->DrawLine(from, to);
		}
	}
	m_debugBatch->End();

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
	m_debugEffect.reset();
	m_debugBatch.reset();
	m_debugInputLayout.Reset();
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