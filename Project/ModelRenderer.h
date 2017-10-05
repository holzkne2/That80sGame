#pragma once

#include "pch.h"
#include "Component.h"

class ModelRenderer :
	public Component
{
public:
	ModelRenderer(GameObject*);
	virtual ~ModelRenderer();

	void Render(ID3D11DeviceContext*, DirectX::CommonStates*,
		DirectX::SimpleMath::Matrix, DirectX::SimpleMath::Matrix);

	void OnDeviceLost();

	void SetModel(ID3D11Device*, const wchar_t*);

private:
	std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
	std::unique_ptr<DirectX::Model> m_model;
};

