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
	m_fxFactory = std::make_unique<EffectFactory>(device);
	m_model = Model::CreateFromCMO(device,
		AssetHelper::GetModelPath(fileName).c_str(), *m_fxFactory);

	if (isAlpha)
	{
		for (unsigned int m = 0; m < m_model->meshes.size(); ++m)
			for (unsigned int p = 0; p < m_model->meshes[m]->meshParts.size(); ++p)
				m_model->meshes[m]->meshParts[p]->isAlpha = true;
	}
}

void ModelRenderer::Render(ID3D11DeviceContext* context, CommonStates* states,
	Matrix view, Matrix projection)
{
	m_model->Draw(context, *states, m_gameObject->GetTransform()->GetWorldMatrix(),
		view, projection);
}

void ModelRenderer::OnDeviceLost()
{
	m_fxFactory.reset();
	m_model.reset();
}
