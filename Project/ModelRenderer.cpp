#include "ModelRenderer.h"
#include "GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

ModelRenderer::ModelRenderer(GameObject* gameObject) : Component(gameObject)
{
}


ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::SetModel(ID3D11Device* device, const wchar_t* fileName)
{
	m_fxFactory = std::make_unique<EffectFactory>(device);
	m_model = Model::CreateFromCMO(device, fileName, *m_fxFactory);
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
