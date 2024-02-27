#pragma once

#include "framework.h"

class CVertex
{
public:

	enum
	{
		E_VT_VERTEX = 0,
		E_VT_VERTEX2D,
		E_VT_VERTEXLINE,
		E_VT_VERTEXNT,
		E_VT_VERTEXRGBA,
		E_VT_VERTEXT,
	};

	XMFLOAT3 p;

	CVertex();
	CVertex(float p1, float p2, float p3);
	~CVertex();

	void operator += (const CVertex& v);
	void operator -= (const CVertex& v);
	void operator *= (float v);
	void operator /= (float v);

	CVertex operator + (const CVertex& v);
	CVertex operator - (const CVertex& v);
	CVertex operator * (float v);
	CVertex operator / (float v);

	bool operator == (const CVertex v);
	bool operator != (const CVertex v);

	CVertex Centroid(CVertex* a, CVertex* b, CVertex* c);
	CVertex Cross(CVertex v);
	float Dot(CVertex v);
	float Length();
	CVertex Normal(CVertex* a, CVertex* b, CVertex* c);
	void Normalize();
	CVertex PointToDegree();
	CVertex PointToRadian();
	void RadiusNormalize();
	CVertex RotateAngleByAxis(const float angleInRad, const CVertex referenceAxis);
	void Set(CVertex* v);
};

class CVertexRGBA : public CVertex
{
public:

	XMFLOAT4 c;

	CVertexRGBA();
	~CVertexRGBA();
};

class CVertexT : public CVertex
{
public:

	XMFLOAT2 uv;

	CVertexT();
	~CVertexT();
};

class CVertexNT : public CVertex
{
public:

	XMFLOAT3 n;

	XMFLOAT2 uv;

	CVertexNT();
	~CVertexNT();
};

class CVertex2
{
public:

	XMFLOAT2 p;

	CVertex2();
	CVertex2(float p1, float p2);
	~CVertex2();

	void RadiusNormalize();
};

class CVertex2d
{
public:

	XMFLOAT2 p;

	XMFLOAT2 uv;

	CVertex2d();
	~CVertex2d();
};