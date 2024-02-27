#include "CCamera.h"

/*
*/
CCamera::CCamera()
{
	memset(this, 0x00, sizeof(CCamera));
}

/*
*/
CCamera::CCamera(float width, float height, CVertex* position, float fov, float nearPlane, float farPlane)
{
	memset(this, 0x00, sizeof(CCamera));

	m_width = width;
	m_height = height;

	CCamera::SetPosition(position);

	m_fov = fov;

	m_nearPlane = nearPlane;
	m_farPlane = farPlane;

	m_xmworld = XMMatrixIdentity();

	XMStoreFloat4x4(&m_xmfworld, m_xmworld);

	m_xmproj = XMMatrixPerspectiveFovLH(fov * _PiDiv180, width / height, nearPlane, farPlane);

	XMStoreFloat4x4(&m_xmfproj, m_xmproj);

	CCamera::UpdateView();
}

/*
*/
CCamera::~CCamera()
{
}

/*
*/
void CCamera::Copy(CCamera* camera)
{
	m_xmworld = camera->m_xmworld;

	m_xmproj = camera->m_xmproj;

	m_position = camera->m_position;

	m_rotation = camera->m_rotation;

	CCamera::UpdateView();
}

/*
*/
void CCamera::SetPosition(CVertex* position)
{
	m_position.x = position->p.x;
	m_position.y = position->p.y;
	m_position.z = position->p.z;
}

/*
*/
void CCamera::UpdateRotation(float x, float y, float z)
{
	m_rotation.x += x;

	if (m_rotation.x < 0.0f)
	{
		m_rotation.x = 359.9f;
	}
	else if (m_rotation.x > 359.9f)
	{
		m_rotation.x = 0.0f;
	}

	m_rotation.y += y;

	if (m_rotation.y < 0.0f)
	{
		m_rotation.y = 359.9f;
	}
	else if (m_rotation.y > 359.9f)
	{
		m_rotation.y = 0.0f;
	}

	m_rotation.z += z;

	if (m_rotation.z < 0.0f)
	{
		m_rotation.z = 359.9f;
	}
	else if (m_rotation.z > 359.9f)
	{
		m_rotation.z = 0.0f;
	}
}

/*
*/
void CCamera::UpdateView()
{
	// yaw pitch roll in radians
	m_yaw = m_rotation.y * _PiDiv180;
	m_pitch = m_rotation.x * _PiDiv180;
	m_roll = m_rotation.z * _PiDiv180;

	m_xmMatrix = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);


	m_xmflook.x = 0.0f;
	m_xmflook.y = 0.0f;
	m_xmflook.z = 1.0f;

	m_xmvlook = XMLoadFloat3(&m_xmflook);

	m_xmvlook = XMVector3TransformCoord(m_xmvlook, m_xmMatrix);

	m_xmflook = XMFLOAT3(XMVectorGetX(m_xmvlook), XMVectorGetY(m_xmvlook), XMVectorGetZ(m_xmvlook));


	m_xmflook.x = m_xmflook.x + m_position.x;
	m_xmflook.y = m_xmflook.y + m_position.y;
	m_xmflook.z = m_xmflook.z + m_position.z;

	m_xmvlook = XMLoadFloat3(&m_xmflook);


	m_xmfup.x = 0.0f;
	m_xmfup.y = 1.0f;
	m_xmfup.z = 0.0f;

	m_xmvup = XMLoadFloat3(&m_xmfup);

	m_xmvup = XMVector3TransformCoord(m_xmvup, m_xmMatrix);

	m_xmfup = XMFLOAT3(XMVectorGetX(m_xmvup), XMVectorGetY(m_xmvup), XMVectorGetZ(m_xmvup));


	m_xmvposition = XMLoadFloat3(&m_position);


	m_xmview = XMMatrixLookAtLH(m_xmvposition, m_xmvlook, m_xmvup);

	XMStoreFloat4x4(&m_xmfview, m_xmview);


	m_right = XMFLOAT3(m_xmfview.m[0][0], m_xmfview.m[1][0], m_xmfview.m[2][0]);
	m_up = XMFLOAT3(m_xmfview.m[0][1], m_xmfview.m[1][1], m_xmfview.m[2][1]);
	m_look = XMFLOAT3(m_xmfview.m[0][2], m_xmfview.m[1][2], m_xmfview.m[2][2]);


	// In order to transform the memory from the format defined on the CPU to the order expected in HLSL,
	// you need to transpose the CPU matrix before sending it to the GPU.
	// See row_major and column_major
	// Default is column_major

	//twvpMat = xmworld * xmview * xmproj;
	//wvpMat = XMMatrixTranspose(twvpMat);
}