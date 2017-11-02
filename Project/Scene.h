#pragma once

#include <vector>

#include "pch.h"
#include "GameObject.h"
#include "ModelRenderer.h"
#include "Camera.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();

	void LoadScene(int);

	void Update();
	void Render();
	void OnDeviceLost();

	void AddGameObject(std::unique_ptr<GameObject>&);
	void AddModelRenderer(ModelRenderer*);
	void AddCamera(Camera*);

private:
	void Init();

	void LoadScene0();
	void LoadScene1();

	void UnloadCurrentScene();

private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::CommonStates> m_states;

	std::vector<std::unique_ptr<GameObject>> m_gameObjects;

	std::vector<ModelRenderer*> m_modelRenderers;

	std::vector<Camera*> m_cameras;

	std::unique_ptr<DirectX::BasicEffect> m_debugEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_debugBatch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_debugInputLayout;
};

