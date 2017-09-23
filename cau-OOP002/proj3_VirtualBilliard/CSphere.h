#pragma once
#include "CObject.h"
#include "d3dUtility.h"
#include "ICollidable.h"
#include "ConstVariable.h"
class CSphere : public CObject, public ICollidable{
public:
	CSphere();
	~CSphere();

	bool create(std::string name, IDirect3DDevice9* pDevice, D3DCOLOR);

	void tempdraw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld,
		const D3DXMATRIX& mView,
		const D3DXVECTOR4& mLightPos,
		const D3DXVECTOR4& camPos);
	bool hasIntersected(CSphere& ball) override;
	bool hitBy(CSphere& ball) override;

	void ballUpdate(float timeDiff);

	//caution : getters and setters for pre_velocity_x, pre_velocity_z, isChanged are added
	D3DXVECTOR3 inline getVelocity() const { return this->velocity; }
	float inline getRadius(void)  const { return (float)(M_RADIUS); }
	const inline D3DXVECTOR3 getCenter(void) const
	{
		D3DXVECTOR3 org(m_x, m_y, m_z);
		return org;
	}

	void setCenter(float x, float y, float z);
	void inline setPower(double vx, double vz)
	{
		this->velocity.x = vx;
		this->velocity.z = vz;
	}

	void inline setPower(D3DXVECTOR3 velocity){
		this->velocity = velocity;
	}

	float inline getPower(){
		return velocity.x * velocity.x + velocity.z * velocity.z;
	}

	bool inline isStop(){
		return (velocity.x == 0 && velocity.z == 0);
		//x축 속도와 z축 속도가 모두 0일 경우 true 반환
	}
	static inline bool IsAllStop(CSphere& r1, CSphere& r2, CSphere& w, CSphere& y){
		return r1.isStop() && r2.isStop() && w.isStop() && y.isStop();
		//4개의 공이 모두 정지해 있을 경우 true 반환
	}
	void inline setName(std::string name){ this->name = name; }
	std::string inline getName(){ return this->name; }

private:

	D3DXVECTOR3				velocity;
	float                   m_radius;
	LPD3DXMESH convertMesh(IDirect3DDevice9* pDevice, LPD3DXMESH& mesh) override;
	void moveCenter(D3DXVECTOR3 velocity);

	std::string name;

};
