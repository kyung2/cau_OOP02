#pragma once
#include "CObject.h"
#include <d3dx9.h> 
#include <functional>
#include "ConstVariable.h"
/*
Refactoring
새 클래스를 만들 예정.
cue, light, wall, sphere들은 공통점으로 밑 멤버함수와 변수가 있다.
virtual:

destroy
->destroys ipd3dxmesh

get/set position(float x, y, z)

variables:
d3dxmatrix m_mLocal;
Ipd3dxmesh

wall, sphere는 object클래스를 상속하고 있으므로 object는 위 새로운 클래스를 상속/구현할 예정

light는 위 새 클래스를 바로 상속함.

코드 양이 좀 줄것 같다

object 클래스 이름은 primitive로 변경할 예정

object 클래스는 인터페이스로 create(sphere가 현재 parameter에 d3dxcolor가 붙어있지만 따로 setcolor로 나누는게 맞을듯)

인라인 함수로는 get/setLocalTransform, 

object 클래스에서 hitby, hasIntersected 메소드 뽑아내서 이것도 따로 인터페이스 클래스(ICollidable)를 만들 예정.
그런데 Collision 부분은 잘 모르겠어서 물어봐야겠다..
인터페이스로 뽑아내면 당구판에 홀같이 충돌하는 객체가 생겨도 유연하게 코드 확장이 가능할 것 같은데..

new object(primitive) class{
	
	destroy
	->calls base destroy and destroy m_effect, shadercode, texture

	pure virtual draw(device, world, view)

	get/set local transform

	pure virutal create(pdevice)

	private:

	load shader(device, fileName)
	load texture(device, fileName)
	struct vertex{};

	variable:
	shaderCode
	effect
	texture

}
new ICollidable class{
	pure virtual hitby(Icollidable)
	pure virtual HitProcess()<-자식 클래스에서 private으로 public인 hitby 내부 처리를 감싸는 메소드
	pure virtual hasIntersected(Icollidable)
	pure virtual intersectectedProcess()<-hitProcess와 동일

}


*/
class CSphere;
class CCue : public CObject{
public:
	CCue();
	~CCue();
	bool create(IDirect3DDevice9* pDevice);//will be moved to parent class
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld, const D3DXMATRIX& mView);
	const inline D3DXMATRIX& getLocalTransform() const{ return m_mLocal; }//will be moved to parent class
	const inline D3DXVECTOR3 getPosition()const{//will be moved to parent class
		return D3DXVECTOR3(m_x, m_y, m_z);
	}
	inline void setVisible(bool visible){ isVisible = visible; };
	void setRotationRelative(const D3DXVECTOR3& position);
	const inline bool isPlaying(){ return playing; };
	std::function<void()>HitCallback;
	void playHit();
private:
	bool playing;
	bool isVisible;
	void playAnimation();
	float moveDistance;
	bool pulled;
	float movementDelta = CUE_MOVEMENT_DELTA;
	LPD3DXMESH convertMesh(IDirect3DDevice9* pDevice, LPD3DXMESH& mesh) override;
	//to be overrided






};