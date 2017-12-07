#include "ModelRenderer.h"
#include "GameObject.h"
#include "Game.h"
#include "AssetHelper.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

ModelRenderer::ModelRenderer(GameObject* gameObject) : Component(gameObject)
{
	Game::Get()->GetScene()->AddModelRenderer(this);
}


ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::SetModel(ID3D11Device* device, const wchar_t* fileName, bool isAlpha)
{
	m_filename = fileName;
	m_model = AssetHelper::Get().GetModel(device, fileName, isAlpha);

	m_alpha = isAlpha;
}

void ModelRenderer::Render(ID3D11DeviceContext* context, CommonStates* states,
	Matrix view, Matrix projection)
{
	m_model->Draw(context, *states, m_gameObject->GetTransform()->GetWorldMatrix(),
		view, projection);
}

void ModelRenderer::OnDeviceLost()
{
}

void ModelRenderer::Save(std::map<std::string, std::string>& data)
{
	Component::Save(data);

	data.insert(std::pair<std::string, std::string>("file Name", wc_s(m_filename)));
	data.insert(std::pair<std::string, std::string>("Is Alpha", m_alpha ? "True" : "False"));
}
