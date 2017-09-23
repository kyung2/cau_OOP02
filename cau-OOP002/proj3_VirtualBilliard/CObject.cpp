#include "CObject.h"

CObject::CObject(){}
CObject::~CObject(){}
const D3DVERTEXELEMENT9 CObject::decl[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, sizeof(D3DXVECTOR3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, sizeof(D3DXVECTOR3) * 2, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
};
bool CObject::create(IDirect3DDevice9* pDevice, CObject::Shape shape){//Only create mesh 
	HRESULT result;
	switch (shape){
	case SPHERE:
		result = D3DXCreateSphere(pDevice, M_RADIUS, SPHERE_SLICE, SPHERE_STACK, &m_pMesh, NULL);
		break;
	case CYLINDER:
		result = D3DXCreateCylinder(pDevice, CYLINDER_NEGZRAD, CYLINDER_POSZRAD, CYLINDER_LENGTH, CYLINDER_SLICE, CYLINDER_STACK, &m_pMesh, NULL);
		break;
	}
	if (FAILED(result)){
		return false;
	}

	return true;
}
void CObject::setRotation(float x, float y, float z){
	D3DXMATRIX m;
	m_rotx = x;
	m_roty = y;
	m_rotz = z;
	D3DXMatrixRotationYawPitchRoll(&m, D3DXToRadian(y), D3DXToRadian(x), D3DXToRadian(z));
	D3DXMatrixMultiply(&m_mLocal, &m, &m_mLocal);
}
void CObject::setRotation(const D3DXVECTOR3& rot){
	setRotation(rot.x, rot.y, rot.z);
}
const D3DXMATRIX CObject::getRotationMat(){
	D3DXMATRIX m;
	D3DXMatrixRotationYawPitchRoll(&m, D3DXToRadian(m_roty), D3DXToRadian(m_rotx), D3DXToRadian(m_rotz));
	return m;
}
void CObject::setPosition(float x, float y, float z){

	D3DXMATRIX m;
	this->m_x = x;
	this->m_y = y;
	this->m_z = z;

	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}
void CObject::setPosition(const D3DXVECTOR3& position){
	D3DXMATRIX m;
	this->m_x = position.x;
	this->m_y = position.y;
	this->m_z = position.z;
	D3DXMatrixTranslation(&m, position.x, position.y, position.z);
	setLocalTransform(m);
}
void CObject::destroy(){

	if (m_pMesh != NULL) {
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_shaderCode != NULL){
		m_shaderCode->Release();
		m_shaderCode = NULL;
	}
	if (m_texture != NULL) {
		m_texture->Release();
		m_texture = NULL;
	}
	if (m_effect != NULL){
		m_effect->Release();
		m_effect = NULL;
	}
}
LPD3DXEFFECT CObject::LoadShader(IDirect3DDevice9* pDevice, const char* fileName){
	LPD3DXBUFFER pError = nullptr;
	LPD3DXEFFECT ret = nullptr;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;

#endif
	D3DXCreateEffectFromFile(pDevice, fileName, 0, 0, dwShaderFlags, 0, &ret, &pError);
	if (!ret && pError){
		int size = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();
		if (ack) {
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
			//d3d::Release<LPD3DXBUFFER>(pError);
		}
	}
	return ret;
}
LPDIRECT3DTEXTURE9 CObject::LoadTexture(IDirect3DDevice9* pDevice, const char* fileName) {
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(pDevice, fileName, &ret))) {
		OutputDebugString("Failed to load texture: ");
		OutputDebugString(fileName);
		OutputDebugString("\n");
	}
	return ret;
}