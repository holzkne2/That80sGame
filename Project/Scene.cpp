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
#include "DebugDraw.h"
#include "PrefabLoader.h"
#include "PhysicsComponent.h"
#include "TimerUI.h"

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
	uiImageRenderer->SetColor(Color(Colors::Red));

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
	uiTextRenderer->SetColor(Color(Colors::Cyan));
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
	gameObject = std::make_unique<GameObject>("Tree");
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Tree.cmo");
	gameObject->GetTransform()->SetPosition(Vector3(0, -1.75, 6));

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

	PrefabLoader::SavePrefab(gameObject.get());
	AddGameObject(gameObject);

	GameObject* tempParent;

	std::vector<Vector3> collisionPointsTower;
	collisionPointsTower.push_back(Vector3(2, 0, 2));
	collisionPointsTower.push_back(Vector3(2, 0, -2));
	collisionPointsTower.push_back(Vector3(-2, 0, -2));
	collisionPointsTower.push_back(Vector3(-2, 0, 2));
	collisionPointsTower.push_back(Vector3(1.249, 4.693, 1.249));
	collisionPointsTower.push_back(Vector3(1.249, 4.693, -1.249));
	collisionPointsTower.push_back(Vector3(-1.249, 4.693, -1.249));
	collisionPointsTower.push_back(Vector3(-1.249, 4.693, 1.249));

	// Technically not needed
	std::vector<Vector3> collisionPointsTreeBase;
	collisionPointsTreeBase.push_back(Vector3(3.906, 0, 0.056));
	collisionPointsTreeBase.push_back(Vector3(1.152, 0, -1.378));
	collisionPointsTreeBase.push_back(Vector3(-2.834, 0, -1.74));
	collisionPointsTreeBase.push_back(Vector3(-3.906, 0, -1.5));
	collisionPointsTreeBase.push_back(Vector3(-3.906, 0, 1.5));
	collisionPointsTreeBase.push_back(Vector3(-2.92, 0, 1.635));
	collisionPointsTreeBase.push_back(Vector3(1.084, 0, 0.733));
	collisionPointsTreeBase.push_back(Vector3(0.289, 0.75, -0.483));
	collisionPointsTreeBase.push_back(Vector3(-2.006, 0.997, -0.198));
	collisionPointsTreeBase.push_back(Vector3(-2.484, 1.036, -0.128));
	collisionPointsTreeBase.push_back(Vector3(-2.308, 1.033, 0.287));
	collisionPointsTreeBase.push_back(Vector3(-1.906, 0.997, 0.201));

	std::vector<Vector3> collisionPointsTreeTrunk1;
	collisionPointsTreeTrunk1.push_back(Vector3(-2.006, 0.997, -0.198));
	collisionPointsTreeTrunk1.push_back(Vector3(-2.484, 1.036, -0.128));
	collisionPointsTreeTrunk1.push_back(Vector3(-2.308, 1.033, 0.287));
	collisionPointsTreeTrunk1.push_back(Vector3(-1.906, 0.997, 0.201));
	collisionPointsTreeTrunk1.push_back(Vector3(-1.684, 2.956, -0.11));
	collisionPointsTreeTrunk1.push_back(Vector3(-1.925, 3.098, -0.102));
	collisionPointsTreeTrunk1.push_back(Vector3(-1.86, 3.097, 0.206));
	collisionPointsTreeTrunk1.push_back(Vector3(-1.648, 2.965, 0.156));

	std::vector<Vector3> collisionPointsTreeTrunk2;
	collisionPointsTreeTrunk2.push_back(Vector3(-1.684, 2.956, -0.11));
	collisionPointsTreeTrunk2.push_back(Vector3(-1.925, 3.098, -0.102));
	collisionPointsTreeTrunk2.push_back(Vector3(-1.86, 3.097, 0.206));
	collisionPointsTreeTrunk2.push_back(Vector3(-1.648, 2.965, 0.156));
	collisionPointsTreeTrunk2.push_back(Vector3(-0.537, 4.706, -0.049));
	collisionPointsTreeTrunk2.push_back(Vector3(-0.619, 4.822, -0.167));
	collisionPointsTreeTrunk2.push_back(Vector3(-0.649, 4.865, 0.05));
	collisionPointsTreeTrunk2.push_back(Vector3(-0.57, 4.745, 0.119));
	
	std::vector<Vector3> collisionPointsTreeLeaves;
	collisionPointsTreeLeaves.push_back(Vector3(0.201, 4.309, -1.705));
	collisionPointsTreeLeaves.push_back(Vector3(-1.183, 4.742, -1.541));
	collisionPointsTreeLeaves.push_back(Vector3(-1.101, 4.447, -0.818));
	collisionPointsTreeLeaves.push_back(Vector3(-1.749, 5.069, 0.082));
	collisionPointsTreeLeaves.push_back(Vector3(-0.89, 4.359, 1.549));
	collisionPointsTreeLeaves.push_back(Vector3(0.864, 4.6, 1.993));
	collisionPointsTreeLeaves.push_back(Vector3(2.447, 4.66, 1.112));
	collisionPointsTreeLeaves.push_back(Vector3(2.677, 5.562, -0.174));
	collisionPointsTreeLeaves.push_back(Vector3(2.017, 5.013, -1.829));

	if (false)
	{
		///
		/// Grid
		///
		gameObject = std::make_unique<GameObject>("Part 1");
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
		gameObject->GetTransform()->SetPosition(Vector3(-1.5, 0, 30));
		last = gameObject.get();

		AddGameObject(gameObject);

		gameObject = std::make_unique<GameObject>("Tower Collider");
		gameObject->GetTransform()->SetParent(last->GetTransform());
		gameObject->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
		gameObject->SetTag("Obsticle");
		PhysicsComponent* physics = gameObject->AddComponent<PhysicsComponent>();
		physics->AddMeshCollider(collisionPointsTower);
		physics->SetMass(0);
		physics->SetKinematic(true);
		physics->SetGroup(collisiontypes::COL_OBSTICLE);
		physics->SetMask(collisiontypes::COL_SHIP);
		physics->init();

		AddGameObject(gameObject);

		gameObject = std::make_unique<GameObject>("Tower 1");
		gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Tower01.cmo");
		gameObject->GetTransform()->SetParent(tempParent->GetTransform());
		gameObject->GetTransform()->SetPosition(Vector3(1.5, 0, 45));
		last = gameObject.get();

		AddGameObject(gameObject);

		gameObject = std::make_unique<GameObject>("Tower Collider");
		gameObject->GetTransform()->SetParent(last->GetTransform());
		gameObject->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
		gameObject->SetTag("Obsticle");
		physics = gameObject->AddComponent<PhysicsComponent>();
		physics->AddMeshCollider(collisionPointsTower);
		physics->SetMass(0);
		physics->SetKinematic(true);
		physics->SetGroup(collisiontypes::COL_OBSTICLE);
		physics->SetMask(collisiontypes::COL_SHIP);
		physics->init();

		AddGameObject(gameObject);

		gameObject = std::make_unique<GameObject>("Tree");
		gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Tree.cmo");
		gameObject->GetTransform()->SetParent(tempParent->GetTransform());
		gameObject->GetTransform()->SetPosition(Vector3(0.1, 0, 60));
		last = gameObject.get();

		AddGameObject(gameObject);

		gameObject = std::make_unique<GameObject>("Tree Collider");
		gameObject->GetTransform()->SetParent(last->GetTransform());
		gameObject->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
		gameObject->SetTag("Obsticle");
		physics = gameObject->AddComponent<PhysicsComponent>();
		physics->AddMeshCollider(collisionPointsTreeBase);
		physics->AddMeshCollider(collisionPointsTreeTrunk1);
		physics->AddMeshCollider(collisionPointsTreeTrunk2);
		physics->AddMeshCollider(collisionPointsTreeLeaves);
		physics->SetMass(0);
		physics->SetKinematic(true);
		physics->SetGroup(collisiontypes::COL_OBSTICLE);
		physics->SetMask(collisiontypes::COL_SHIP);
		physics->init();
		AddGameObject(gameObject);

		PrefabLoader::SavePrefab(tempParent);
	}
	else
	{
		GameObject* part = PrefabLoader::LoadPrefab("Part 1");
		trackManager->AddObject(part);
	}

	if (true)
	{
		GameObject* part = PrefabLoader::LoadPrefab("Part 1");
		part->SetName("Part 2");
		trackManager->AddObject(part);

		part = PrefabLoader::LoadPrefab("Part 1");
		part->SetName("Part 4");
		trackManager->AddObject(part);
	}

	///
	/// Sun
	///
	gameObject = std::make_unique<GameObject>("Sun");
	gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Sun.cmo", true);
	gameObject->GetTransform()->SetPosition(Vector3(0, 0, 500));
	gameObject->GetTransform()->SetScale(5);

	PrefabLoader::SavePrefab(gameObject.get());
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
	GameObject* last = gameObject.get();
	gameObject->SetActive(false);

	PrefabLoader::SavePrefab(gameObject.get());
	AddGameObject(gameObject);

	PhysicsComponent* physics;

	///
	/// Ship
	///
	GameObject* player;
	if (false)
	{
		gameObject = std::make_unique<GameObject>("Ship");
		gameObject->AddComponent<ModelRenderer>()->SetModel(deviceResources->GetD3DDevice(), L"Ship01.cmo");
		gameObject->GetTransform()->SetPosition(Vector3(0, 2, 0));
		gameObject->AddComponent<ShipController>();

		std::vector<Vector3> collisionPointsShip;
		collisionPointsShip.push_back(Vector3(0, 0, 2.135));
		collisionPointsShip.push_back(Vector3(0, 0, -0.135));
		collisionPointsShip.push_back(Vector3(0, 0.225, -0.135));
		collisionPointsShip.push_back(Vector3(0, 0.425, 0.029));
		collisionPointsShip.push_back(Vector3(0.5, 0, -0.071));
		collisionPointsShip.push_back(Vector3(-0.5, 0, -0.071));
		
		physics = gameObject->AddComponent<PhysicsComponent>();
		physics->AddMeshCollider(collisionPointsShip);
		
		collisionPointsShip.clear();
		collisionPointsShip.push_back(Vector3(1.326, -0.089, -0.095));
		collisionPointsShip.push_back(Vector3(-1.326, -0.089, -0.095));
		collisionPointsShip.push_back(Vector3(0, 0.081, 0.842));
		collisionPointsShip.push_back(Vector3(0, 0.142, 0.842));
		physics->AddMeshCollider(collisionPointsShip);
		
		physics->SetMass(1);
		physics->SetGroup(collisiontypes::COL_SHIP);
		physics->SetMask(collisiontypes::COL_OBSTICLE | collisiontypes::COL_CONTRAINT);
		physics->init();

		gameObject->GetComponent<ShipController>()->SetGameOverUI(last);

		player = gameObject.get();
		PrefabLoader::SavePrefab(gameObject.get());
		AddGameObject(gameObject);
	}
	else
	{
		player = PrefabLoader::LoadPrefab("Ship");
		player->GetComponent<ShipController>()->SetGameOverUI(last);
	}

	///
	/// Constraint Box
	///
	gameObject = std::make_unique<GameObject>("Top");
	gameObject->GetTransform()->SetPosition(Vector3(0, 5.75, 0));
	physics = gameObject->AddComponent<PhysicsComponent>();
	physics->AddBoxCollider(Vector3(3, 0.25, 0.5));
	physics->SetMass(0);
	physics->SetKinematic(true);
	physics->SetGroup(collisiontypes::COL_CONTRAINT);
	physics->SetMask(collisiontypes::COL_SHIP);
	physics->init();

	PrefabLoader::SavePrefab(gameObject.get());
	AddGameObject(gameObject);

	gameObject = std::make_unique<GameObject>("Bottom");
	gameObject->GetTransform()->SetPosition(Vector3(0, 1, 0));
	physics = gameObject->AddComponent<PhysicsComponent>();
	physics->AddBoxCollider(Vector3(3, 0.25, 0.5));
	physics->SetMass(0);
	physics->SetKinematic(true);
	physics->SetGroup(collisiontypes::COL_CONTRAINT);
	physics->SetMask(collisiontypes::COL_SHIP);
	physics->init();

	PrefabLoader::SavePrefab(gameObject.get());
	AddGameObject(gameObject);

	gameObject = std::make_unique<GameObject>("Left");
	gameObject->GetTransform()->SetPosition(Vector3(3.5, 2.5, 0));
	physics = gameObject->AddComponent<PhysicsComponent>();
	physics->AddBoxCollider(Vector3(0.25, 3, 0.5));
	physics->SetMass(0);
	physics->SetKinematic(true);
	physics->SetGroup(collisiontypes::COL_CONTRAINT);
	physics->SetMask(collisiontypes::COL_SHIP);
	physics->init();

	PrefabLoader::SavePrefab(gameObject.get());
	AddGameObject(gameObject);

	gameObject = std::make_unique<GameObject>("Right");
	gameObject->GetTransform()->SetPosition(Vector3(-3.5, 2.5, 0));
	physics = gameObject->AddComponent<PhysicsComponent>();
	physics->AddBoxCollider(Vector3(0.25, 3, 0.5));
	physics->SetMass(0);
	physics->SetKinematic(true);
	physics->SetGroup(collisiontypes::COL_CONTRAINT);
	physics->SetMask(collisiontypes::COL_SHIP);
	physics->init();

	PrefabLoader::SavePrefab(gameObject.get());
	AddGameObject(gameObject);

	///
	/// Camera
	///
	gameObject = std::make_unique<GameObject>("Camera");
	gameObject->AddComponent<Camera>();
	gameObject->GetTransform()->SetPosition(Vector3(0, 5, -3.5));
	gameObject->GetTransform()->SetRotation(
		Quaternion::Euler(Vector3(-20, 180, 0)));
	gameObject->AddComponent<CameraFollow>()->SetTarget(player->GetTransform());

	PrefabLoader::SavePrefab(gameObject.get());
	AddGameObject(gameObject);

	///
	/// Timer
	///
	gameObject = std::make_unique<GameObject>("Timer");
	UITextRenderer* uiTextRenderer = gameObject->AddComponent<UITextRenderer>();
	rectTransform = dynamic_cast<RectTransform*>(gameObject->GetTransform());
	uiTextRenderer->SetFont(deviceResources->GetD3DDevice(), L"StillTime64.spritefont");
	uiTextRenderer->SetColor(Color(Colors::White));
	uiTextRenderer->SetText(L"00:00");

	gameObject->AddComponent<TimerUI>()->SetUI(uiTextRenderer);

	rectTransform->SetPosition(Vector3(0, 0, 0));
	rectTransform->SetScale(1);
	rectTransform->SetAnchors(Vector2(0, 0));

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
	GamePad::ButtonStateTracker* buttons = Game::Get()->GetButtons();

	if (state.IsConnected())
	{
		// TODO: Move to GAME
		buttons->Update(state);

		if (buttons->start == GamePad::ButtonStateTracker::PRESSED)
		{
			LoadScene(1);
		}
		if (buttons->view == GamePad::ButtonStateTracker::PRESSED)
		{
			LoadScene(0);
		}

		if (buttons->dpadDown == GamePad::ButtonStateTracker::PRESSED)
		{
			Game::Get()->GetPhysicsManager()->GetDebugDraw()->SetActive(
				!Game::Get()->GetPhysicsManager()->GetDebugDraw()->IsActive());
		}
	}
}

void Scene::Render()
{
	DX::DeviceResources* deviceResources = Game::Get()->GetDeviceResources();

	deviceResources->PIXBeginEvent(L"Render");
	auto context = deviceResources->GetD3DDeviceContext();
	// Add your rendering code here.
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
	if (Game::Get()->GetPhysicsManager()->GetDebugDraw()->IsActive())
	{
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
	}

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

void Scene::AddGameObject(GameObject* gameObject)
{
	m_gameObjects.push_back(std::unique_ptr<GameObject>(gameObject));
}

void Scene::AddModelRenderer(ModelRenderer* modelRenderer)
{
	m_modelRenderers.push_back(modelRenderer);
}

void Scene::AddCamera(Camera* camera)
{
	m_cameras.push_back(camera);
}

GameObject* Scene::GetGameObject(std::string name)
{
	for (unsigned int i = 0; i < m_gameObjects.size(); ++i) {
		if (m_gameObjects[i]->GetName() == name)
			return m_gameObjects[i].get();
	}
}

std::vector<GameObject*> Scene::GetGameObjects(std::string name)
{
	std::vector<GameObject*> output;
	for (unsigned int i = 0; i < m_gameObjects.size(); ++i) {
		if (m_gameObjects[i]->GetName() == name)
			output.push_back(m_gameObjects[i].get());
	}
	return output;
}

GameObject* Scene::GetGameObjectWithTag(std::string tag)
{
	for (unsigned int i = 0; i < m_gameObjects.size(); ++i) {
		if (m_gameObjects[i]->GetTag() == tag)
			return m_gameObjects[i].get();
	}
}

std::vector<GameObject*> Scene::GetGameObjectsWithTag(std::string tag)
{
	std::vector<GameObject*> output;
	for (unsigned int i = 0; i < m_gameObjects.size(); ++i) {
		if (m_gameObjects[i]->GetTag() == tag)
			output.push_back(m_gameObjects[i].get());
	}
	return output;
}