#pragma once

#include "framework.h"

#include "CVertex.h"

class CCamera
{
public:

	XMFLOAT3 m_look;
	XMFLOAT3 m_position;
	XMFLOAT3 m_right;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_up;
	
	XMFLOAT4X4 m_xmfproj;
	XMFLOAT4X4 m_xmfview;
	XMFLOAT4X4 m_xmfworld;
	
	XMMATRIX m_twvpMat;
	XMMATRIX m_wvpMat;
	XMMATRIX m_xmproj;
	XMMATRIX m_xmview;
	XMMATRIX m_xmworld;
	
	float m_farPlane;
	float m_fov;
	float m_height;
	float m_nearPlane;
	float m_pitch;
	float m_roll;
	float m_width;
	float m_yaw;

	CCamera();
	CCamera(float width, float height, CVertex* position, float fov, float nearPlane, float farPlane);
	~CCamera();

	void Copy(CCamera* camera);
	void SetPosition(CVertex* position);
	void UpdateRotation(float x, float y, float z);
	void UpdateView();

private:

	XMFLOAT3 m_xmflook;
	XMFLOAT3 m_xmfup;
	
	XMMATRIX m_xmMatrix;
	
	XMVECTOR m_xmvlook;
	XMVECTOR m_xmvposition;
	XMVECTOR m_xmvup;
};