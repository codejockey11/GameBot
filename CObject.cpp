#include "CObject.h"

/*
*/
CObject::CObject()
{
	memset(this, 0x00, sizeof(CObject));
}

/*
*/
CObject::CObject(CLinkList<CMesh>* meshs)
{
	memset(this, 0x00, sizeof(CObject));

	m_limboTimer = new CTimer(5000);

	m_meshs = meshs;

	CLinkListNode<CMesh>* mesh = m_meshs->m_list;

	short materialCount = 0;

	while (mesh->m_object)
	{
		m_vertexBuffers[materialCount] = new CVertexBuffer(mesh->m_object->m_vertexCount, (void*)mesh->m_object->m_vertices);

		mesh = mesh->m_next;
	}
}

/*
*/
CObject::~CObject()
{
	for (short materialCount = 0; materialCount < m_meshs->m_count; materialCount++)
	{
		delete m_vertexBuffers[materialCount];
	}

	delete m_name;
	delete m_limboTimer;
}

/*
*/
void CObject::Animation(CFrametime* frametime)
{
	// bob = non-updating environment position + amp * sin(freq * time);
	m_bob = m_environmentPosition.y + (0.1250f * (float)sin((float)m_count));
	
	m_spin += 90.0f * frametime->m_frametime;
	if (m_spin > 359.99f)
	{
		m_spin -= 359.99f;
	}
	
	float freq = 4.0f;

	m_count += freq * frametime->m_frametime;

	m_position.y = m_bob;
	
	m_rotation.y = m_spin;
}

/*
*/
void CObject::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	m_environmentPosition = m_position;
}

/*
*/
void CObject::SetPosition(CVertex* position)
{
	m_position.x = position->p.x;
	m_position.y = position->p.y;
	m_position.z = position->p.z;

	m_environmentPosition = m_position;
}

/*
*/
void CObject::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

/*
*/
void CObject::SetRotation(CVertex* rotation)
{
	m_rotation.x = rotation->p.x;
	m_rotation.y = rotation->p.y;
	m_rotation.z = rotation->p.z;
}

/*
*/
void CObject::SetScale(float x, float y, float z)
{
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
}

/*
*/
void CObject::UpdateServer()
{
	CLinkListNode<CMesh>* mn = m_meshs->m_list;

	short materialCount = 0;

	while (mn->m_object)
	{
		m_vertexBuffers[materialCount]->m_scale.x = m_scale.x;
		m_vertexBuffers[materialCount]->m_scale.y = m_scale.y;
		m_vertexBuffers[materialCount]->m_scale.z = m_scale.z;

		m_vertexBuffers[materialCount]->m_position.x = m_position.x;
		m_vertexBuffers[materialCount]->m_position.y = m_position.y;
		m_vertexBuffers[materialCount]->m_position.z = m_position.z;

		m_vertexBuffers[materialCount]->m_rotation.x = m_rotation.x;
		m_vertexBuffers[materialCount]->m_rotation.y = m_rotation.y;
		m_vertexBuffers[materialCount]->m_rotation.z = m_rotation.z;

		m_vertexBuffers[materialCount]->UpdateRotation();

		m_vertexBuffers[materialCount]->UpdateServer(mn->m_object->m_vertices);

		materialCount++;

		mn = mn->m_next;
	}
}