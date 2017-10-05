#include "pch.h"
#include "UITextRenderer.h"
#include "RectTransform.h"
#include "GameObject.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;

UITextRenderer::UITextRenderer(GameObject* gameObject) : Component(gameObject)
{
	m_text = L"hello world";
	m_color = Colors::White;

	m_gameObject->MakeRectTransform();
}


UITextRenderer::~UITextRenderer()
{
}

void UITextRenderer::Render(SpriteBatch* spriteBatch, const RECT& screen)
{
	RectTransform* rectTransform = dynamic_cast<RectTransform*>(m_gameObject->GetTransform());

	m_font->DrawString(spriteBatch, m_text.c_str(),
		rectTransform->GetScreenPosition(screen), m_color,
		0.0 /*TODO: Rotation*/, rectTransform->GetPivot(), rectTransform->GetScale());
}

void UITextRenderer::OnDeviceLost()
{
	m_font.reset();
}

void UITextRenderer::SetFont(ID3D11Device* device, const wchar_t* filename)
{
	m_font = std::make_unique<SpriteFont>(device, filename);
}