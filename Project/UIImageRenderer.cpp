#include "pch.h"
#include "UIImageRenderer.h"
#include "RectTransform.h"
#include "GameObject.h"
#include "AssetHelper.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

UIImageRenderer::UIImageRenderer(GameObject* gameObject) : Component(gameObject)
{
	m_color = Colors::White;

	m_gameObject->MakeRectTransform();
}


UIImageRenderer::~UIImageRenderer()
{
}

void UIImageRenderer::Render(SpriteBatch* spriteBatch, const RECT& screen)
{
	RectTransform* rectTransform = dynamic_cast<RectTransform*>(m_gameObject->GetTransform());

	spriteBatch->Draw(m_image.Get(), rectTransform->GetScreenPosition(screen),
		nullptr, m_color, XMConvertToRadians(rectTransform->GetRotation().EulerAngles().z),
		rectTransform->GetPivot(), rectTransform->GetScale());
}

void UIImageRenderer::OnDeviceLost()
{
	m_image.Reset();
}

void UIImageRenderer::SetImage(ID3D11Device* device, const wchar_t* filename)
{
	m_fileName = wc_s(filename);

	ComPtr<ID3D11Resource> resource;
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, AssetHelper::GetSpritePath(filename).c_str(),
		resource.GetAddressOf(), m_image.ReleaseAndGetAddressOf())
		);

	ComPtr<ID3D11Texture2D> image;
	DX::ThrowIfFailed(resource.As(&image));
	image->GetDesc(&m_imageDesc);
}

void UIImageRenderer::Save(std::map<std::string, std::string>& data)
{
	Component::Save(data);

	data.insert(std::pair<std::string, std::string>("File Name", m_fileName));
	data.insert(std::pair<std::string, std::string>("Color", to_string(m_color)));
}