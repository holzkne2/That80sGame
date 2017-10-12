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
	void LoadScene0();

private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::CommonStates> m_states;

	std::vector<std::unique_ptr<GameObject>> m_gameObjects;

	std::vector<ModelRenderer*> m_modelRenderers;

	std::vector<Camera*> m_cameras;

	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
};

