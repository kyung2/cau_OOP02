////////////////////////////////////////////////////////////////////////////////
//
// File: virtualLego.cpp
//
// Original Author: 박창현 Chang-hyeon Park, 
// Modified by Bong-Soo Sohn and Dong-Jun Kim
// 
// Originally programmed for Virtual LEGO. 
// Modified later to program for Virtual Billiard.
//        
////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include "CSphere.h"
#include "CWall.h"
#include "CLight.h"
#include "CPlayer.h"
#include "CText.h"
#include "ConstVariable.h"
#include "CCue.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <string>
using namespace std;
IDirect3DDevice9* Device = NULL;

// window size
const int Width  = 800;
const int Height = 600;

// There are four balls
// initialize the position (coordinate) of each ball (ball0 ~ ball3)
const float spherePos[4][2] = { {-2.7f,0} , {+2.4f,0} , {3.3f,0} , {-2.7f,-0.9f}}; 
//for debugging
//const float spherePos[4][2] = { { 2.4f, 0.3f }, { 2.4f, -0.3f }, { 2.7f, 0 }, { -2.7f, 0 } };

// initialize the color of each ball (ball0 ~ ball3)
const D3DXCOLOR sphereColor[4] = {d3d::RED, d3d::RED, d3d::YELLOW, d3d::WHITE};

string sphereName[4] = { "Red1", "Red2", "Yellow", "White" };

// -----------------------------------------------------------------------------
// Camera view
// -----------------------------------------------------------------------------
float leftright = 0, updown = 0, LRbuf = 0, UDbuf = 0;
float at_x = 0.0f, at_y = 5.0f, at_z = -8.0;
float point_x = 0.0f, point_y = 0.0f, point_z = 0.0f;
float up_x = 0.0f, up_y = 2.0f, up_z = 0.0f;
bool base_angle= false, white_angle = false, yellow_angle = false;

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;
D3DXMATRIX g_mBackup;

#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 0.9982

#define PLAYER_NUM 2

// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
CWall	g_legoPlane;
D3DXPLANE g_Plane(-4.0f, 0.f, -3.f, 0.f);
CWall	g_legowall[4];
CSphere	g_sphere[4];
CPlayer player[PLAYER_NUM];
/*CSphere g_redSphere[2];
CSphere g_whiteSphere;
CSphere g_yellowSphere;*/

LPDIRECT3DTEXTURE9 g_pShadowMap = nullptr;
LPDIRECT3DSURFACE9 g_pDSShadow = nullptr;

D3DXMATRIXA16	g_mShadowProj;

CSphere	g_target_blueball;
CLight	g_light;
//need to be deleted!
static int order = 0;

bool is_score_checked = true;

const string player1Str = "Player1";
const string player2Str = "Player2";
CText g_player1;
CText g_player2;
CText g_scoreIndicator;

CCue g_cue;


D3DXVECTOR4 g_camera_pos(0.0, 5.0, -8.0, 1.f);

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------


void destroyAllLegoBlock(void)
{
	for (int i = 0; i < 4l; i++){
		g_sphere[i].destroy();
	}
	g_player1.destroy();
	g_player2.destroy();
}

// initialization
bool Setup()
{
	int i;
	if (g_scoreIndicator.create(Device, Width, Height, SCORE) == false) return false;
	if (g_player1.create(Device, Width, Height, player1Str) == false) return false;
	if (g_player2.create(Device, Width, Height, player2Str) == false) return false;

	g_player1.setAnchor(DT_TOP | DT_LEFT);
	g_player2.setAnchor(DT_TOP | DT_RIGHT);
	g_scoreIndicator.setAnchor(DT_TOP | DT_CENTER);


    D3DXMatrixIdentity(&g_mWorld);
    D3DXMatrixIdentity(&g_mView);
    D3DXMatrixIdentity(&g_mProj);
	D3DXMatrixIdentity(&g_mBackup);
		
	// create plane and set the position
    if (false == g_legoPlane.create(Device, PLANE_WIDTH, PLANE_HEIGHT, PLANE_DEPTH, CWall::Plane)) return false;
    //g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);
	
	/*
	//shadow map texture
	Device->CreateTexture(
		512,
		512,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_R32F,
		D3DPOOL_DEFAULT,
		&g_pShadowMap, NULL);

	//create depth stencil buffer

	Device->CreateDepthStencilSurface(
		512,
		512,
		_D3DFORMAT::D3DFMT_D24X8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&g_pDSShadow,
		NULL);

	D3DXMatrixPerspectiveFovLH(&g_mShadowProj, 1.f, 1, 0.10f, 100.f);*/
	// create walls and set the position. note that there are four walls
	if (false == g_legowall[0].create(Device, EDGE_H_WIDTH, EDGE_HEIGHT, EDGE_H_DEPTH, CWall::Edge)) return false;
	g_legowall[0].setPosition(0.0f, 0.12f, 3.06f);
	if (false == g_legowall[1].create(Device, EDGE_H_WIDTH, EDGE_HEIGHT, EDGE_H_DEPTH, CWall::Edge)) return false;
	g_legowall[1].setPosition(0.0f, 0.12f, -3.06f);
	if (false == g_legowall[2].create(Device, EDGE_V_WIDTH, EDGE_HEIGHT, EDGE_V_DEPTH, CWall::Edge)) return false;
	g_legowall[2].setPosition(4.56f, 0.12f, 0.0f);
	if (false == g_legowall[3].create(Device, EDGE_V_WIDTH, EDGE_HEIGHT, EDGE_V_DEPTH, CWall::Edge)) return false;
	g_legowall[3].setPosition(-4.56f, 0.12f, 0.0f);

	// create four balls and set the position
	for (i=0;i<4;i++) {
		if (false == g_sphere[i].create(sphereName[i], Device, sphereColor[i])) return false;
		g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS , spherePos[i][1]);
		g_sphere[i].setPower(0,0);
	}
	
	//initialize player objects
	player[0].create(g_sphere[3]);		//white
	player[1].create(g_sphere[2]);		//yellow

	// create blue ball for set direction
    if (false == g_target_blueball.create("Blue", Device, d3d::BLUE)) return false;
	g_target_blueball.setCenter(.0f, (float)M_RADIUS , .0f);
	
	if (g_cue.create(Device) == false) return false;

	g_cue.setPosition(g_sphere[3 - CPlayer::turn].getCenter());
	g_cue.HitCallback = [=](){
		int curr_sphere = 3 - CPlayer::turn;
		g_cue.setVisible(false);
		D3DXVECTOR3 targetpos = g_target_blueball.getCenter();
		D3DXVECTOR3	spherePos = g_sphere[curr_sphere].getCenter();

		double theta = acos(sqrt(pow(targetpos.x - spherePos.x, 2)) / sqrt(pow(targetpos.x - spherePos.x, 2) +
			pow(targetpos.z - spherePos.z, 2)));		// 기본 1 사분면 및 x축 오른쪽

		if (targetpos.z - spherePos.z < 0 && targetpos.x - spherePos.x > 0) { theta = -theta; }	//4 사분면
		else if (targetpos.z - spherePos.z >= 0 && targetpos.x - spherePos.x < 0) { theta = PI - theta; } //2 사분면 및 x축 왼쪽
		else if (targetpos.z - spherePos.z < 0 && targetpos.x - spherePos.x < 0) { theta = PI + theta; } // 3 사분면
		else if (targetpos.z - spherePos.z < 0 && targetpos.x - spherePos.x == 0) { theta = PI*(3 / 2); } // y축 위쪽
		else if (targetpos.z - spherePos.z > 0 && targetpos.x - spherePos.x == 0) { theta = PI*(1 / 2); } // y축 아래쪽

		double distance = sqrt(pow(targetpos.x - spherePos.x, 2) + pow(targetpos.z - spherePos.z, 2));
		g_sphere[curr_sphere].setPower(distance * cos(theta), distance * sin(theta));
	};
	// light setting 
   
    if (false == g_light.create(Device))
        return false;
	
	// Position and aim the camera.
	D3DXVECTOR3 pos(0.0f, 5.0f, -8.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 2.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);
	
	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 4,
        (float)Width / (float)Height, 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
	
    // Set render states.
    Device->SetRenderState(D3DRS_LIGHTING, TRUE);
    Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
    Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	

	g_light.setLight(Device, g_mWorld);


	return true;
}

void Cleanup(void)
{
    g_legoPlane.destroy();
	for(int i = 0 ; i < 4; i++) {
		g_sphere[i].destroy();
		g_legowall[i].destroy();
	}
	g_cue.destroy();
    destroyAllLegoBlock();
    g_light.destroy();
}

void campSetting()
{
	if (base_angle)
	{
		g_mWorld = g_mBackup;
		at_x = 0.0f, at_y = 5.0f, at_z = -8.0;
		point_x = 0.0f, point_y = 0.0f, point_z = 0.0f;
		up_x = 0.0f, up_y = 2.0f, up_z = 0.0f; LRbuf = 0, UDbuf = 0;
		base_angle = false, yellow_angle = false, white_angle = false;
	}
	if (white_angle)
	{
		at_x = g_sphere[3].getCenter().x, at_y = g_sphere[3].getCenter().y + 1.5, at_z = g_sphere[3].getCenter().z - 1.5;
		point_x = g_sphere[3].getCenter().x, point_y = g_sphere[3].getCenter().y, point_z = g_sphere[3].getCenter().z;
		up_x = 0.0f, up_y = 2.0f, up_z = 0.0f;
	}
	if (yellow_angle)
	{
		at_x = g_sphere[2].getCenter().x, at_y = g_sphere[2].getCenter().y + 1.5, at_z = g_sphere[2].getCenter().z - 1.5;
		point_x = g_sphere[2].getCenter().x, point_y = g_sphere[2].getCenter().y, point_z = g_sphere[2].getCenter().z;
		up_x = 0.0f, up_y = 2.0f, up_z = 0.0f;
	}

	D3DXVECTOR3 pos(at_x, at_y, at_z);
	D3DXVECTOR3 target(point_x, point_y, point_z);
	D3DXVECTOR3 up(up_x, up_y, up_z);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);
}

// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"
bool Display(float timeDelta)
{
	int i=0;
	int j = 0;


	if( Device )
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00afafaf, 1.0f, 0);
		
		Device->BeginScene();

		campSetting();

		CPlayer& current_player = player[CPlayer::turn];
		g_player1.setStr("Player1: " + to_string(player[0].getScore()));
		g_player2.setStr("Player2: " + to_string(player[1].getScore()));
		g_player1.draw();
		g_player2.draw();
		if (CSphere::IsAllStop(g_sphere[0], g_sphere[1], g_sphere[2], g_sphere[3]) && !g_cue.isPlaying()){
			//JisuLee__update score for current player
			if (!is_score_checked){
				current_player.decideScore();

				if (!current_player.isOneMoreTurn()) { CPlayer::switchTurn(); }

				//initialize flags
				is_score_checked = true;
				current_player.resetHit();
			}

			int curr_sphere = 3 - CPlayer::turn;
			if (curr_sphere == 2){
				OutputDebugString("2\n");
			}
			else{
				OutputDebugString("3\n");
			}

			if (CPlayer::turn == 0){
				g_player1.setColor(d3d::RED);
				g_player2.setColor(d3d::WHITE);
			}
			else{
				g_player1.setColor(d3d::WHITE);
				g_player2.setColor(d3d::RED);
			}
			g_cue.setPosition(g_sphere[curr_sphere].getCenter());
			g_cue.setVisible(true);
			g_cue.setRotationRelative(g_target_blueball.getCenter());

		}


		g_scoreIndicator.draw();

		//draw blue ball when all balls stop
		if (CSphere::IsAllStop(g_sphere[0], g_sphere[1], g_sphere[2], g_sphere[3]))
			g_target_blueball.tempdraw(Device, g_mWorld, g_mView, g_light.getPosition4(),g_camera_pos );

		// check whether any two balls hit together and update the direction of balls
		for(i = 0 ;i < 4; i++){
			for(j = 0 ; j < 4; j++) {
				if(i >= j) {continue;}
				if (g_sphere[i].hitBy(g_sphere[j])){
					OutputDebugString("hit\n");
					if (current_player.isMyBall(g_sphere[i])){
						OutputDebugString("myBall : g_sphere[i]\n");
						current_player.setHitBall(g_sphere[j]);
					}
					else if (current_player.isMyBall(g_sphere[j])){
						OutputDebugString("myBall : g_sphere[j]\n");
						current_player.setHitBall(g_sphere[i]);
					}
				}
			}
		}

		// update the position of each ball. during update, check whether each ball hit by walls.
		for( i = 0; i < 4; i++) {
			for(j = 0; j < 4; j++){ g_legowall[i].hitBy(g_sphere[j]); }
			g_sphere[i].ballUpdate(timeDelta);
		}

		//TODO : shadow mapping
		/*
		Device->SetRenderState(D3DRS_STENCILENABLE, true);
		Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		Device->SetRenderState(D3DRS_STENCILREF, 0x0);
		Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
		Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
		Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

		D3DXVECTOR4 lightDir(
			g_light.getPosition().x,
			g_light.getPosition().y,
			g_light.getPosition().z,
			0.f);
		for (i = 0; i < 4; i++){
			D3DXMATRIX S, T;

			D3DXMatrixShadow(&S, &lightDir, &g_Plane);
			D3DXMatrixTranslation(
				&T,
				g_legowall[i].getPosition().x,
				g_legowall[i].getPosition().y,
				g_legowall[i].getPosition().z);
			Device->SetTransform(D3DTS_WORLD, &(T * S));
		}
		for (i = 0; i < 4; i++){
			D3DXMATRIX S, T;

			D3DXMatrixShadow(&S, &lightDir, &g_Plane);
			D3DXMatrixTranslation(
				&T,
				g_sphere[i].getCenter().x,
				g_sphere[i].getCenter().y,
				g_sphere[i].getCenter().z);
			Device->SetTransform(D3DTS_WORLD, &(T * S));
		}
		D3DXMATRIX S, T;

		D3DXMatrixShadow(&S, &lightDir, &g_Plane);
		D3DXMatrixTranslation(
			&T,
			g_legoPlane.getPosition().x,
			g_legoPlane.getPosition().y,
			g_legoPlane.getPosition().z);
		Device->SetTransform(D3DTS_WORLD, &(T * S));

		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		D3DMATERIAL9 mtrl = d3d::InitMtrl(d3d::BLACK, d3d::BLACK, d3d::BLACK, d3d::BLACK, 0.0f);
		mtrl.Diffuse.a = 0.5f;
		Device->SetRenderState(D3DRS_ZENABLE, false);
		Device->SetMaterial(&mtrl);
		Device->SetTexture(0, 0);
		*/


		/*if (g_cue.IsAnimationEnded()){
			D3DXVECTOR3 targetpos = g_target_blueball.getCenter();
			D3DXVECTOR3	whitepos = g_sphere[3].getCenter();
			double theta = acos(sqrt(pow(targetpos.x - whitepos.x, 2)) / sqrt(pow(targetpos.x - whitepos.x, 2) +
				pow(targetpos.z - whitepos.z, 2)));		// 기본 1 사분면
			if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x >= 0) { theta = -theta; }	//4 사분면
			if (targetpos.z - whitepos.z >= 0 && targetpos.x - whitepos.x <= 0) { theta = PI - theta; } //2 사분면
			if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x <= 0){ theta = PI + theta; } // 3 사분면
			double distance = sqrt(pow(targetpos.x - whitepos.x, 2) + pow(targetpos.z - whitepos.z, 2));
			g_sphere[3].setPower(distance * cos(theta), distance * sin(theta));
		}*/

		g_cue.draw(Device, g_mWorld, g_mView);

		// draw plane, walls, and spheres
		g_legoPlane.draw(Device, g_mWorld, g_mView);
		for (i=0;i<4;i++) 	{
			g_legowall[i].draw(Device, g_mWorld, g_mView);
			g_sphere[i].tempdraw(Device, g_mWorld, g_mView, g_light.getPosition4(), g_camera_pos);//  draw(Device, g_mWorld, g_mView);
		}

        g_light.draw(Device);
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture( 0, NULL );

		//Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		//Device->SetRenderState(D3DRS_STENCILENABLE, false);
	}
	return true;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool wire = false;
	static bool isReset = true;
	static float zoomlvl = D3DX_PI / 4;
	static int old_x = 0;
	static float rot_x = 0;
	static float rot_y = 0;
	static int old_y = 0;
	static enum { WORLD_MOVE, LIGHT_MOVE, BLOCK_MOVE } move = WORLD_MOVE;


	switch (msg) {
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case 0x42: // B Key
					base_angle = true;
					white_angle = false;
					yellow_angle = false;
					break;

				case 0x4e: // N key
					base_angle = false;
					white_angle = true;
					yellow_angle = false;
					break;

				case 0x4d: // M key
					base_angle = false;
					white_angle = false;
					yellow_angle = true;
					break;

				case VK_F1:
					g_target_blueball.setCenter(.0f, (float)M_RADIUS, .0f);
					break;

				case VK_ESCAPE:
					::DestroyWindow(hwnd);
					break;

				case VK_RETURN:
					if (NULL != Device)
					{
						wire = !wire;
						Device->SetRenderState(D3DRS_FILLMODE,
						(wire ? D3DFILL_WIREFRAME : D3DFILL_SOLID));
					}
					break;

				case VK_SPACE:

					g_cue.playHit();
					is_score_checked = false;

					break;
				default:
					D3DXMATRIX kX;
					D3DXMATRIX kY;
					leftright = 0, updown = 0;
					if (wParam == VK_RIGHT)
					{
						if (LRbuf >= 1.6)
							leftright = 0;
						else{
							LRbuf += 0.1;
							leftright = 0.1;
						}
					}
					if (wParam == VK_LEFT)
					{
						if (LRbuf <= -1.6)
							leftright = 0;
						else{
							LRbuf -= 0.1;
							leftright = -0.1;
						}
					}
					if (wParam == VK_UP)
					{
						if (UDbuf <= -1.0)
							updown = 0;
						else{
							UDbuf -= 0.1;
							updown = -0.1;
						}
					}
					if (wParam == VK_DOWN)
					{
						if (UDbuf >= 0.5)
							updown = 0;
						else{
							UDbuf += 0.1;
							updown = 0.1;
						}
					}
					D3DXMatrixRotationY(&kY, leftright);
					D3DXMatrixRotationX(&kX, updown);
					g_mWorld = g_mWorld * kX * kY;
					break;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			int new_x = LOWORD(lParam);
			int new_y = HIWORD(lParam);
			float dx;
			float dy;

			if (LOWORD(wParam) & MK_LBUTTON)
			{
				if (g_cue.isPlaying())
					break;
				if (isReset)
				{
					isReset = false;
				}
				else 
				{
					D3DXVECTOR3 vDist;
					D3DXVECTOR3 vTrans;
					D3DXMATRIX mTrans;
					D3DXMATRIX mX;
					D3DXMATRIX mY;

					switch (move) 
					{
						case WORLD_MOVE:
							dx = (old_x - new_x) * 0.01f;
							dy = (old_y - new_y) * 0.01f;
							D3DXMatrixRotationY(&mX, dx);
							D3DXMatrixRotationX(&mY, dy);
							g_mWorld = g_mWorld * mX * mY;

						break;
					}
				}
				old_x = new_x;
				old_y = new_y;

			}
			else 
			{
				isReset = true;

				if (LOWORD(wParam) & MK_RBUTTON) 
				{
					if (LOWORD(wParam) <= 0)
						new_x = 0;
					if (new_y <= 0)
						new_y = 0;
					if (g_cue.isPlaying())
						break;
					dx = (old_x - new_x);// * 0.01f;
					dy = (old_y - new_y);// * 0.01f;

					D3DXVECTOR3 coord3d = g_target_blueball.getCenter();
					g_target_blueball.setCenter(coord3d.x + dx*(-0.007f), coord3d.y, coord3d.z + dy*0.007f);
				}
				int new_x = LOWORD(lParam);
				int new_y = HIWORD(lParam);
				float dx;
				float dy;

			if (LOWORD(wParam) & MK_LBUTTON) 
			{

				if (isReset) {
					isReset = false;
				}
				else {
					D3DXVECTOR3 vDist;
					D3DXVECTOR3 vTrans;
					D3DXMATRIX mTrans;
					D3DXMATRIX mX;
					D3DXMATRIX mY;

					switch (move) {
					case WORLD_MOVE:
						dx = (old_x - new_x) * 0.01f;
						dy = (old_y - new_y) * 0.01f;

						//rotation restriction
						/*
						rot_x += dx;
						rot_y += dy;
						std::string debug = "X :" + std::to_string(rot_x) + " Y: " + std::to_string(rot_y) + "\n";
						OutputDebugString(debug.c_str());
						if (abs(rot_y) > 1.0 || abs(rot_x) > 1.7f)
						break;
						*/
						D3DXMatrixRotationY(&mX, dx);
						D3DXMatrixRotationX(&mY, dy);
						g_mWorld = g_mWorld * mX * mY;

						break;
					}
				}

				old_x = new_x;
				old_y = new_y;

			}
			else 
			{
				isReset = true;

				if (LOWORD(wParam) & MK_RBUTTON)
				{
					dx = (old_x - new_x);// * 0.01f;
					dy = (old_y - new_y);// * 0.01f;

					D3DXVECTOR3 coord3d = g_target_blueball.getCenter();
					g_target_blueball.setCenter(coord3d.x + dx*(-0.007f), coord3d.y, coord3d.z + dy*0.007f);
				}
				old_x = new_x;
				old_y = new_y;

				move = WORLD_MOVE;
			}
			break;
		}
	}
	case WM_MOUSEWHEEL:
	{
		short wheelAmout = GET_WHEEL_DELTA_WPARAM(wParam);
		if (wheelAmout != 0){
			D3DXMATRIX mProj;
			if (wheelAmout > 0){
				zoomlvl += 0.1f;
				if (zoomlvl >= 1.f)
					zoomlvl = 1.f;
			}
			else{
				zoomlvl -= 0.1f;
				if (zoomlvl <= 0.2f)
					zoomlvl = 0.2f;
			}

			D3DXMatrixPerspectiveFovLH(&mProj, zoomlvl, (float)Width / (float)Height, 1.f, 100.0f);
			Device->SetTransform(D3DTS_PROJECTION, &mProj);
		}
		break;
	}
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
    srand(static_cast<unsigned int>(time(NULL)));
	
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
	
	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}
	
	d3d::EnterMsgLoop( Display );
	
	Cleanup();
	
	Device->Release();
	
	return 0;
}