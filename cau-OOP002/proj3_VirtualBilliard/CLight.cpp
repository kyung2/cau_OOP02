#include "CLight.h"

CLight::CLight(){
	static DWORD i = 0;
	m_index = i++;
	D3DXMatrixIdentity(&m_mLocal);
	::ZeroMemory(&m_lit, sizeof(m_lit));
	m_pMesh = NULL;
	m_bound._center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bound._radius = 0.0f;
}
CLight::~CLight(){}

bool CLight::create(IDirect3DDevice9* pDevice){
	if (NULL == pDevice)
		return false;
	if (FAILED(D3DXCreateSphere(pDevice, 0.1f , 10, 10, &m_pMesh, NULL)))
		return false;
	D3DLIGHT9 lit;
	::ZeroMemory(&lit, sizeof(lit));

	m_bound._center = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	m_bound._radius = 0.1f;
	m_lit.Type = D3DLIGHT_POINT;
	//m_lit.Diffuse = d3d::WHITE;
	//m_lit.Specular = d3d::WHITE;
	m_lit.Ambient = d3d::WHITE;
	m_lit.Position = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	m_lit.Direction = lit.Direction;
	m_lit.Range = 100.0f;
	m_lit.Falloff = lit.Falloff;
	m_lit.Attenuation0 = 0.0f;
	m_lit.Attenuation1 = 0.9f;
	m_lit.Attenuation2 = 0.0f;
	m_lit.Theta = lit.Theta;
	m_lit.Phi = lit.Phi;
	return true;
}
void CLight::destroy(){
	if (m_pMesh != NULL) {
		m_pMesh->Release();
		m_pMesh = NULL;
	}
}
bool CLight::setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (NULL == pDevice)
		return false;

	D3DXVECTOR3 pos(m_bound._center);
	D3DXVec3TransformCoord(&pos, &pos, &m_mLocal);
	D3DXVec3TransformCoord(&pos, &pos, &mWorld);
	m_lit.Position = pos;

	pDevice->SetLight(m_index, &m_lit);
	pDevice->LightEnable(m_index, TRUE);
	return true;
}
void CLight::draw(IDirect3DDevice9* pDevice)
{
	if (NULL == pDevice)
		return;
	D3DXMATRIX m;
	D3DXMatrixTranslation(&m, m_lit.Position.x, m_lit.Position.y, m_lit.Position.z);
	pDevice->SetTransform(D3DTS_WORLD, &m);
	pDevice->SetMaterial(&d3d::WHITE_MTRL);
	m_pMesh->DrawSubset(0);
}