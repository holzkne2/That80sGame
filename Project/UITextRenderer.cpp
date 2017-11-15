#include "pch.h"
#include "UITextRenderer.h"
#include "RectTransform.h"
#include "GameObject.h"
#include "AssetHelper.h"

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
		XMConvertToRadians(rectTransform->GetRotation().EulerAngles().z),
		rectTransform->GetPivot(), rectTransform->GetScale());
}

void UITextRenderer::OnDeviceLost()
{
	m_font.reset();
}

void UITextRenderer::SetFont(ID3D11Device* device, const wchar_t* filename)
{
	m_fileName = wc_s(filename);

	m_font = std::make_unique<SpriteFont>(device, AssetHelper::GetFontPath(filename).c_str());
}

void UITextRenderer::Save(std::map<std::string, std::string>& data)
{
	Component::Save(data);

	data.insert(std::pair<std::string, std::string>("Font", m_fileName));
	data.insert(std::pair<std::string, std::string>("Text", wc_s(m_text.c_str())));
	data.insert(std::pair<std::string, std::string>("Color", to_string(m_color)));
}