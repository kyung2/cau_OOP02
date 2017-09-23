#pragma once
#include <d3dx9.h>
#include "d3dUtility.h"
#include "ConstVariable.h"
class CObject{
public:
	CObject();
	virtual ~CObject();

	void destroy();
	void inline setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
	void setPosition(float x, float y, float z);
	void setPosition(const D3DXVECTOR3& position);
	const inline D3DXMATRIX  getLocalTransform(void) const { return m_mLocal; }
	void setRotation(float x, float y, float z);
	const inline D3DXVECTOR3 getRotation(){ return D3DXVECTOR3(m_rotx, m_roty, m_rotz); }
	const D3DXMATRIX getRotationMat();
	void setRotation(const D3DXVECTOR3&);


protected:
	enum Shape{
		SPHERE,
		CYLINDER,
	};
	//vertex layout
#define FVF_VERTEX D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1
	struct VERTEX
	{
		D3DXVECTOR3 pos;     // vertex position
		D3DXVECTOR3 norm;    // vertex normal
		float tu;            // texture coordinates
		float tv;
	};

	//vertex description
	const static D3DVERTEXELEMENT9 decl[];


	bool create(IDirect3DDevice9* pDevice, CObject::Shape shape);
	bool createFloor(IDirect3DDevice9* pDevice);
	LPD3DXEFFECT LoadShader(IDirect3DDevice9* pDevice, const char* fileName);//will be moved to parent class
	LPDIRECT3DTEXTURE9 LoadTexture(IDirect3DDevice9* pDevice, const char* fileName);//will be moved to parent class
	virtual LPD3DXMESH convertMesh(IDirect3DDevice9* pDevice, LPD3DXMESH& input) = 0;


	float m_x;
	float m_y;
	float m_z;
	//m_effect와 m_shaderCode, m_texture로 대체될 예정, 

	float m_rotx;
	float m_roty;
	float m_rotz;

	LPD3DXEFFECT			m_effect;
	D3DXMATRIX              m_mLocal;
	LPD3DXMESH              m_pMesh;
	LPD3DXBUFFER			m_shaderCode;
	LPDIRECT3DTEXTURE9		m_texture;

	char* effectFile;
	char* textureFile;
};