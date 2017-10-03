#include "pch.h"
#include "UITextRenderer.h"
#include "RectTransform.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;

UITextRenderer::UITextRenderer(GameObject* gameObject) : Component(gameObject)
{
	m_text = L"hello world";
	m_color = Colors::White;
}


UITextRenderer::~UITextRenderer()
{
}

void UITextRenderer::Render(SpriteBatch* spriteBatch, const RECT& screen)
{
	m_font->DrawString(spriteBatch, m_text.c_str(),
		m_transform->GetScreenPosition(screen), m_color,
		0.0 /*TODO: Rotation*/, m_transform->GetPivot(), m_transform->GetScale());
}

void UITextRenderer::OnDeviceLost()
{
	m_font.reset();
}

void UITextRenderer::SetFont(ID3D11Device* device, const wchar_t* filename)
{
	m_font = std::make_unique<SpriteFont>(device, filename);
}