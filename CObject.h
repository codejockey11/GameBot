#pragma once

#include "framework.h"

#include "CFrametime.h"
#include "CLinkList.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CTimer.h"
#include "CVertexBuffer.h"

class CObject
{
public:

	CString* m_name;
	CTimer* m_limboTimer;
	CVertexBuffer* m_vertexBuffers[CMaterial::E_MATERIAL_COUNT];
	
	XMFLOAT4 m_position;
	XMFLOAT4 m_environmentPosition;
	XMFLOAT4 m_rotation;
	XMFLOAT4 m_scale;
	
	CObject();
	CObject(CLinkList<CMesh>* meshs);
	~CObject();

	void Animation(CFrametime* frametime);
	void SetPosition(float x, float y, float z);
	void SetPosition(CVertex* position);
	void SetRotation(float x, float y, float z);
	void SetRotation(CVertex* position);
	void SetScale(float x, float y, float z);
	void UpdateServer();

private:

	CLinkList<CMesh>* m_meshs;
	
	float m_bob;
	float m_count;
	float m_spin;
};