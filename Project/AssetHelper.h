#pragma once
#include "pch.h"
#include <map>

struct ModelWrapper {
	std::unique_ptr<DirectX::Model> m_model;
	std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
};

class AssetHelper
{
public:
	virtual ~AssetHelper();

	DirectX::Model* GetModel(ID3D11Device*, const wchar_t*, const bool&);

	void Reset() { m_models.clear(); }

	static std::wstring GetSpritePath(const wchar_t*);

	static std::wstring GetFontPath(const wchar_t*);

	static bool FileExists(const std::string&);

private:
	static std::wstring GetModelPath(const wchar_t*);

private:
	std::map<std::wstring, ModelWrapper> m_models;

public:
	static AssetHelper& Get()
	{
		static AssetHelper instance;
		return instance;
	}
private:
	AssetHelper();
};

