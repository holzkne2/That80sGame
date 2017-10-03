#include "pch.h"
#include "UIImageRenderer.h"
#include "RectTransform.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;

UIImageRenderer::UIImageRenderer(GameObject* gameObject) : Component(gameObject)
{
	m_color = Colors::White;
}


UIImageRenderer::~UIImageRenderer()
{
}

void UIImageRenderer::Render(SpriteBatch* spriteBatch, const RECT& screen)
{
	spriteBatch->Draw(m_image.Get(), m_transform->GetScreenPosition(screen),
		nullptr, m_color, 0.0 /*TODO: Rotation*/,
		m_transform->GetPivot(), m_transform->GetScale());
}

void UIImageRenderer::OnDeviceLost()
{
	m_image.Reset();
}

void UIImageRenderer::SetImage(ID3D11Device* device, const wchar_t* filename)
{
	ComPtr<ID3D11Resource> resource;
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, filename,
		resource.GetAddressOf(), m_image.ReleaseAndGetAddressOf())
		);

	ComPtr<ID3D11Texture2D> image;
	DX::ThrowIfFailed(resource.As(&image));
	image->GetDesc(&m_imageDesc);
}