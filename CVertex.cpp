#include "CVertex.h"

/*
*/
CVertex::CVertex()
{
	memset(this, 0x00, sizeof(CVertex));
}

/*
*/
CVertex::CVertex(float p1, float p2, float p3)
{
	memset(this, 0x00, sizeof(CVertex));

	p.x = p1;
	p.y = p2;
	p.z = p3;
}

/*
*/
CVertex::~CVertex()
{
}

/*
*/
void CVertex::operator += (const CVertex& v)
{
	p.x += v.p.x;
	p.y += v.p.y;
	p.z += v.p.z;
};

/*
*/
void CVertex::operator -= (const CVertex& v)
{
	p.x -= v.p.x;
	p.y -= v.p.y;
	p.z -= v.p.z;
};

/*
*/
void CVertex::operator *= (float v)
{
	p.x *= v;
	p.y *= v;
	p.z *= v;
};

/*
*/
void CVertex::operator /= (float v)
{
	p.x /= v;
	p.y /= v;
	p.z /= v;
};

/*
*/
CVertex CVertex::operator + (const CVertex& v)
{
	CVertex t;

	t.p.x = p.x + v.p.x;
	t.p.y = p.y + v.p.y;
	t.p.z = p.z + v.p.z;

	return t;
};

/*
*/
CVertex CVertex::operator - (const CVertex& v)
{
	CVertex t;

	t.p.x = p.x - v.p.x;
	t.p.y = p.y - v.p.y;
	t.p.z = p.z - v.p.z;

	return t;
};

/*
*/
CVertex CVertex::operator * (float v)
{
	CVertex t;

	t.p.x = p.x * v;
	t.p.y = p.y * v;
	t.p.z = p.z * v;

	return t;
};

/*
*/
CVertex CVertex::operator / (float v)
{
	CVertex t;

	t.p.x = p.x / v;
	t.p.y = p.y / v;
	t.p.z = p.z / v;

	return t;
};

/*
*/
bool CVertex::operator == (const CVertex v)
{
	return (p.x == v.p.x) && (p.y == v.p.y) && (p.z == v.p.z);
};

/*
*/
bool CVertex::operator != (const CVertex v)
{
	return !((p.x == v.p.x) && (p.y == v.p.y) && (p.z == v.p.z));
};

/*
*/
CVertex CVertex::Centroid(CVertex* a, CVertex* b, CVertex* c)
{
	CVertex t;

	t.p.x = (a->p.x + b->p.x + c->p.x) / 3;
	t.p.y = (a->p.y + b->p.y + c->p.y) / 3;
	t.p.z = (a->p.z + b->p.z + c->p.z) / 3;

	return t;
}

/*
*/
CVertex CVertex::Cross(CVertex v)
{
	CVertex t;

	t.p.x = (p.y * v.p.z) - (p.z * v.p.y);
	t.p.y = (p.z * v.p.x) - (p.x * v.p.z);
	t.p.z = (p.x * v.p.y) - (p.y * v.p.x);

	return t;
}

/*
*/
float CVertex::Dot(CVertex v)
{
	return (p.x * v.p.x) + (p.y * v.p.y) + (p.z * v.p.z);
};

/*
*/
float CVertex::Length()
{
	return (float)sqrtf((p.x * p.x) + (p.y * p.y) + (p.z * p.z));
}

/*
*/
CVertex CVertex::Normal(CVertex* a, CVertex* b, CVertex* c)
{
	CVertex edge1 = *c - *b;
	CVertex edge2 = *c - *a;

	edge1.Normalize();
	edge2.Normalize();

	CVertex N1 = edge1.Cross(edge2);

	N1.Normalize();

	return N1;
}

/*
*/
void CVertex::Normalize()
{
	float d = CVertex::Length();

	p.x = p.x / d;
	p.y = p.y / d;
	p.z = p.z / d;
}

/*
*/
CVertex CVertex::PointToDegree()
{
	CVertex r;

	CVertex ptr = CVertex::PointToRadian();

	// yaw angle
	r.p.y = ptr.p.x * _180DivPi;
	if (r.p.y < 0.0f) { r.p.y += 360.0f; }

	// pitch angle
	r.p.x = ptr.p.y * _180DivPi;
	if (r.p.x < 0.0f) { r.p.x += 360.0f; }

	// roll angle
	r.p.z = ptr.p.z * _180DivPi;
	if (r.p.z < 0.0f) { r.p.z += 360.0f; }

	return r;
}

/*
*/
CVertex CVertex::PointToRadian()
{
	CVertex r;

	// yaw angle
	r.p.x = atan2f(p.x, p.z);

	// pitch angle
	r.p.y = atan2f(p.y, p.z);

	// roll angle from camera's right normal
	r.p.z = atan2f(p.y, p.x);

	return r;
}

/*
*/
void CVertex::RadiusNormalize()
{
	float d = (float)fabs((double)p.x) + (float)fabs((double)p.y) + (float)fabs((double)p.z);

	p.x = p.x / d;
	p.y = p.y / d;
	p.z = p.z / d;
}

/*
*  Rotate 3D vector pvToRotate by angle AngleRad (in radian), along axis pvAxis,
*  using right handed space (positive rotation is counter-clockwize)
*  This is using Rodrigues' rotation formula which is more efficient than
*  converting the axis and angle into a rotation matrix, and using the rotation
*  matrix to compute the rotated vector.
*
* Rotate vector V by Angle about direction vector W:
*      Vrot = V*Cos(Angle) + (WxV)*Sin(Angle) + w*(w.v)*(1-Cos(Angle))
*/
CVertex CVertex::RotateAngleByAxis(const float angleInRad, const CVertex referenceAxis)
{
	CVertex out, axis;

	axis = referenceAxis;
	
	axis.Normalize();
	
	out = (*this) * ((float)(cos(angleInRad))) +
		axis.Cross(*this) * ((float)(sin(angleInRad))) +
		axis * (axis.Dot(*this)) * (1 - ((float)(cos(angleInRad))));

	return out;
}

void CVertex::Set(CVertex* v)
{
	p.x = v->p.x;
	p.y = v->p.y;
	p.z = v->p.z;
}

/*
*/
CVertexRGBA::CVertexRGBA()
{
	memset(this, 0x00, sizeof(CVertexRGBA));
}

/*
*/
CVertexRGBA::~CVertexRGBA()
{
}

/*
*/
CVertexT::CVertexT()
{
	memset(this, 0x00, sizeof(CVertexT));
}

/*
*/
CVertexT::~CVertexT()
{
}

/*
*/
CVertexNT::CVertexNT()
{
	memset(this, 0x00, sizeof(CVertexNT));
}

/*
*/
CVertexNT::~CVertexNT()
{
}

/*
*/
CVertex2::CVertex2()
{
	memset(this, 0x00, sizeof(CVertex2));
}

/*
*/
CVertex2::CVertex2(float p1, float p2)
{
	p.x = p1;
	p.y = p2;
}

/*
*/
CVertex2::~CVertex2()
{
}

/*
*/
void CVertex2::RadiusNormalize()
{
	float d = (float)fabs((double)p.x) + (float)fabs((double)p.y);

	p.x = p.x / d;
	p.y = p.y / d;
}

/*
*/
CVertex2d::CVertex2d()
{
	memset(this, 0x00, sizeof(CVertex2d));
}

/*
*/
CVertex2d::~CVertex2d()
{
}