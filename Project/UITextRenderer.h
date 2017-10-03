#pragma once

#include "pch.h"
#include "Component.h"

class UITextRenderer :
	public Component
{
public:
	UITextRenderer(GameObject*);
	virtual ~UITextRenderer();

	void Render(DirectX::SpriteBatch*, const RECT&);

	void OnDeviceLost();

	void SetFont(ID3D11Device*, const wchar_t*);
	DirectX::SpriteFont* GetFont() { return m_font.get(); }

	void SetText(std::wstring text) { m_text = text; }
	const std::wstring& GetText() { return m_text; }

	void SetColor(DirectX::XMVECTOR color) { m_color = color; }
	DirectX::XMVECTOR GetColor() { return m_color; }

private:
	std::unique_ptr<DirectX::SpriteFont> m_font;
	std::wstring m_text;

	DirectX::XMVECTOR m_color;
};

