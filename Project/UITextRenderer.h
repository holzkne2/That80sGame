#pragma once

#include "pch.h"
#include "Component.h"
#include "RectTransform.h"

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

	void SetColor(DirectX::SimpleMath::Color color) { m_color = color; }
	DirectX::SimpleMath::Color GetColor() { return m_color; }

	virtual void Save(std::map<std::string, std::string>& data) override;

private:
	std::unique_ptr<DirectX::SpriteFont> m_font;
	std::wstring m_text;

	std::string m_fileName;
	DirectX::SimpleMath::Color m_color;
};

