#pragma once
#include "d3dUtility.h"
class CLight{
public:
	CLight();
	~CLight();
	bool create(IDirect3DDevice9* pDevice);
	void destroy();
	bool setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);
	void draw(IDirect3DDevice9* pDevice);
	const inline D3DXVECTOR3 getPosition()const{ return D3DXVECTOR3(m_lit.Position); }
	const D3DXVECTOR4 getPosition4()const{ return D3DXVECTOR4(m_lit.Position, 1); }
private:
	DWORD               m_index;
	D3DLIGHT9           m_lit;
	d3d::BoundingSphere m_bound;
	D3DXMATRIX              m_mLocal;
	ID3DXMesh*              m_pMesh;

};