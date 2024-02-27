#include "CModel.h"

/*
*/
CModel::CModel()
{
	memset(this, 0x00, sizeof(CModel));
}

/*
*/
CModel::CModel(const char* filename)
{
	memset(this, 0x00, sizeof(CModel));

	m_name = new CString(filename);

	FILE* file = {};

	errno_t err = fopen_s(&file, filename, "rb");

	if (err != 0)
	{
		return;
	}

	m_meshs = new CLinkList<CMesh>();

	short len;

	fread_s(&m_materialCount, sizeof(short), sizeof(short), 1, file);

	for (short mc = 0; mc < m_materialCount; mc++)
	{
		m_mesh = new CMesh();

		fread_s(&len, sizeof(short), sizeof(short), 1, file);

		char* str = (char*)calloc((size_t)len + 1, sizeof(char));

		fread_s(str, sizeof(char) * len, sizeof(char), len, file);

		m_mesh->m_material = new CMaterial(str);

		free(str);

		fread_s(&m_mesh->m_material->m_ambient, sizeof(XMFLOAT3), sizeof(float), 3, file);
		fread_s(&m_mesh->m_material->m_diffuse, sizeof(XMFLOAT3), sizeof(float), 3, file);
		fread_s(&m_mesh->m_material->m_specular, sizeof(XMFLOAT3), sizeof(float), 3, file);
		fread_s(&m_mesh->m_material->m_emissive, sizeof(XMFLOAT3), sizeof(float), 3, file);
		fread_s(&m_mesh->m_material->m_specularExponent, sizeof(float), sizeof(float), 1, file);
		fread_s(&m_mesh->m_material->m_opacity, sizeof(float), sizeof(float), 1, file);
		fread_s(&m_mesh->m_material->m_illum, sizeof(UINT), sizeof(UINT), 1, file);
		fread_s(&m_mesh->m_material->m_opticalDensity, sizeof(float), sizeof(float), 1, file);

		fread_s(&len, sizeof(short), sizeof(short), 1, file);
		if (len > 0)
		{
			char* str = (char*)calloc((size_t)len + 1, sizeof(char));

			fread_s(str, sizeof(char) * len, sizeof(char), len, file);

			CString* path = new CString("image\\");

			path->Concat(str);

			m_mesh->m_map_Ka = new CString(path->GetText());

			delete path;

			free(str);
		}

		fread_s(&len, sizeof(short), sizeof(short), 1, file);
		if (len > 0)
		{
			char* str = (char*)calloc((size_t)len + 1, sizeof(char));

			fread_s(str, sizeof(char) * len, sizeof(char), len, file);

			CString* path = new CString("image\\");

			path->Concat(str);

			m_mesh->m_map_Kd = new CString(path->GetText());

			delete path;

			free(str);
		}

		fread_s(&len, sizeof(short), sizeof(short), 1, file);
		if (len > 0)
		{
			char* str = (char*)calloc((size_t)len + 1, sizeof(char));

			fread_s(str, sizeof(char) * len, sizeof(char), len, file);

			CString* path = new CString("image\\");

			path->Concat(str);

			m_mesh->m_map_Ks = new CString(path->GetText());

			delete path;

			free(str);
		}

		fread_s(&len, sizeof(short), sizeof(short), 1, file);
		if (len > 0)
		{
			char* str = (char*)calloc((size_t)len + 1, sizeof(char));

			fread_s(str, sizeof(char) * len, sizeof(char), len, file);

			CString* path = new CString("image\\");

			path->Concat(str);

			m_mesh->m_map_Ns = new CString(path->GetText());

			delete path;

			free(str);
		}

		fread_s(&len, sizeof(short), sizeof(short), 1, file);
		if (len > 0)
		{
			char* str = (char*)calloc((size_t)len + 1, sizeof(char));

			fread_s(str, sizeof(char) * len, sizeof(char), len, file);

			CString* path = new CString("image\\");

			path->Concat(str);

			m_mesh->m_map_d = new CString(path->GetText());

			delete path;

			free(str);
		}

		fread_s(&len, sizeof(short), sizeof(short), 1, file);
		if (len > 0)
		{
			char* str = (char*)calloc((size_t)len + 1, sizeof(char));

			fread_s(str, sizeof(char) * len, sizeof(char), len, file);

			CString* path = new CString("image\\");

			path->Concat(str);

			m_mesh->m_map_Kb = new CString(path->GetText());

			delete path;

			free(str);
		}

		fread_s(&len, sizeof(short), sizeof(short), 1, file);
		if (len > 0)
		{
			char* str = (char*)calloc((size_t)len + 1, sizeof(char));

			fread_s(str, sizeof(char) * len, sizeof(char), len, file);

			CString* path = new CString("image\\");

			path->Concat(str);

			m_mesh->m_refl = new CString(path->GetText());

			delete path;

			free(str);
		}


		fread_s(&m_mesh->m_vertexCount, sizeof(UINT), sizeof(UINT), 1, file);

		size_t bufferSize = m_mesh->m_vertexCount * sizeof(CVertexNT);

		m_mesh->m_vertices = new BYTE[bufferSize];

		fread_s(m_mesh->m_vertices, sizeof(BYTE) * bufferSize, sizeof(BYTE), bufferSize, file);


		m_meshs->Add(m_mesh, m_mesh->m_material->m_name->GetText());
	}

	
	fclose(file);

	m_isInitialized = true;
}

/*
*/
CModel::~CModel()
{
	delete m_meshs;
}