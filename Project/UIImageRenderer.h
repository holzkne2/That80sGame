#pragma once

#include "pch.h"
#include "Component.h"
#include "RectTransform.h"

class UIImageRenderer :
	public Component
{
public:
	UIImageRenderer(GameObject*);
	virtual ~UIImageRenderer();

	void Render(DirectX::SpriteBatch*, const RECT&);

	void OnDeviceLost();

	void SetImage(ID3D11Device*, const wchar_t*);
	ID3D11ShaderResourceView* GetImage() { return m_image.Get(); }

	void SetColor(DirectX::SimpleMath::Color color) { m_color = color; }
	DirectX::SimpleMath::Color GetColor() { return m_color; }

	UINT GetWidth() { return m_imageDesc.Width; }
	UINT GetHeight() { return m_imageDesc.Height; }

	virtual void Save(std::map<std::string, std::string>& data) override;

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_image;
	CD3D11_TEXTURE2D_DESC m_imageDesc;

	std::string m_fileName;
	DirectX::SimpleMath::Color m_color;
};

