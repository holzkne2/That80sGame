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

	void SetModel(ID3D11Device* device, const wchar_t* fileName, bool isAlpha = false);

	virtual void Save(std::map<std::string, std::string>& data) override;

	bool GetAlpha() { return m_alpha; }

	std::vector<DirectX::SimpleMath::Vector3> GetVertices() {}

private:
	DirectX::Model* m_model;
	const wchar_t* m_filename;
	bool m_alpha;
};

