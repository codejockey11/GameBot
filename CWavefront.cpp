#include "CWavefront.h"

CWavefront::CWavefront()
{
	memset(this, 0x00, sizeof(CWavefront));
}

CWavefront::CWavefront(const char* filename, const char* materialname)
{
	memset(this, 0x00, sizeof(CWavefront));

	m_name = new CString(filename);

	m_materialname = new CString(materialname);

	m_meshs = new CLinkList<CMesh>();


	FILE* file = {};

	errno_t err = fopen_s(&file, filename, "r");

	if (err != 0)
	{
		return;
	}

	m_vertex = new CVertex[CWavefront::VIEW_COUNT]();
	m_normal = new CVertex[CWavefront::VIEW_COUNT]();
	m_uv = new CVertex2[CWavefront::VIEW_COUNT]();

	for (int i = 0; i < CMaterial::E_MATERIAL_COUNT; i++)
	{
		m_vnt[i] = new CVertexNT[CWavefront::VIEW_COUNT]();
		m_vntl[i] = new CVertexNT[CWavefront::VIEW_COUNT]();
	}


	BYTE ft = true;

	char line[CWavefront::LINE_LENGTH] = {};
	char path[CWavefront::LINE_LENGTH] = {};

	int mnbr = 0;
	int maxIndex = 0;

	int bytesRead = 0;

	UINT vindex = 0;
	UINT nindex = 0;
	UINT uvindex = 0;

	m_inbr = -1;

	if (file)
	{
		while (fscanf_s(file, "%s", line, CWavefront::LINE_LENGTH) != EOF)
		{
			if (strcmp(line, "usemtl") == 0)
			{
				m_inbr++;

				bytesRead = fscanf_s(file, "%s\n", &m_materialName[m_inbr], CMaterial::E_MATERIAL_NAME_LENGTH);

				if (ft)
				{
					ft = false;
				}
				else if (m_inbr > 0)
				{
					m_icount[m_inbr - 1] = m_vntcount;

					m_vntcount = 0;
				}
			}

			if (strcmp(line, "v") == 0)
			{
				bytesRead = fscanf_s(file, "%f %f %f\n", &m_vertex[vindex].p.x, &m_vertex[vindex].p.y, &m_vertex[vindex].p.z);

				vindex++;
			}

			if (strcmp(line, "vt") == 0)
			{
				bytesRead = fscanf_s(file, "%f %f\n", &m_uv[uvindex].p.x, &m_uv[uvindex].p.y);

				uvindex++;
			}

			if (strcmp(line, "vn") == 0)
			{
				bytesRead = fscanf_s(file, "%f %f %f\n", &m_normal[nindex].p.x, &m_normal[nindex].p.y, &m_normal[nindex].p.z);

				nindex++;
			}

			if (strcmp(line, "f") == 0)
			{
				for (UINT i = 0; i < 8; i++)
				{
					m_vi[i] = m_uvi[i] = m_ni[i] = 0;
				}

				bytesRead = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
					&m_vi[0], &m_uvi[0], &m_ni[0],
					&m_vi[1], &m_uvi[1], &m_ni[1],
					&m_vi[2], &m_uvi[2], &m_ni[2],
					&m_vi[3], &m_uvi[3], &m_ni[3],
					&m_vi[4], &m_uvi[4], &m_ni[4],
					&m_vi[5], &m_uvi[5], &m_ni[5],
					&m_vi[6], &m_uvi[6], &m_ni[6],
					&m_vi[7], &m_uvi[7], &m_ni[7]
				);

				for (UINT i = 0; i < 3; i++)
				{
					if ((m_vi[i] > 0) && (m_ni[i] > 0) && (m_uvi[i] > 0))
					{
						m_vnt[m_inbr][m_vntcount].p.x = m_vertex[m_vi[i] - 1].p.x;
						m_vnt[m_inbr][m_vntcount].p.y = m_vertex[m_vi[i] - 1].p.y;
						m_vnt[m_inbr][m_vntcount].p.z = m_vertex[m_vi[i] - 1].p.z;

						m_vnt[m_inbr][m_vntcount].n.x = m_normal[m_ni[i] - 1].p.x;
						m_vnt[m_inbr][m_vntcount].n.y = m_normal[m_ni[i] - 1].p.y;
						m_vnt[m_inbr][m_vntcount].n.z = m_normal[m_ni[i] - 1].p.z;

						m_vnt[m_inbr][m_vntcount].uv.x = m_uv[m_uvi[i] - 1].p.x;
						m_vnt[m_inbr][m_vntcount].uv.y = m_uv[m_uvi[i] - 1].p.y;

						m_vntcount++;
					}
				}

				maxIndex = 0;

				for (UINT i = 3; i < 8; i++)
				{
					if (m_vi[i] != 0)
					{
						maxIndex = i - 2;
					}
				}

				for (int i = 0; i < maxIndex; i++)
				{
					m_vnt[m_inbr][m_vntcount].p.x = m_vertex[m_vi[i] - 1].p.x;
					m_vnt[m_inbr][m_vntcount].p.y = m_vertex[m_vi[i] - 1].p.y;
					m_vnt[m_inbr][m_vntcount].p.z = m_vertex[m_vi[i] - 1].p.z;

					m_vnt[m_inbr][m_vntcount].n.x = m_normal[m_ni[i] - 1].p.x;
					m_vnt[m_inbr][m_vntcount].n.y = m_normal[m_ni[i] - 1].p.y;
					m_vnt[m_inbr][m_vntcount].n.z = m_normal[m_ni[i] - 1].p.z;

					m_vnt[m_inbr][m_vntcount].uv.x = m_uv[m_uvi[i] - 1].p.x;
					m_vnt[m_inbr][m_vntcount].uv.y = m_uv[m_uvi[i] - 1].p.y;

					m_vntcount++;


					m_vnt[m_inbr][m_vntcount].p.x = m_vertex[m_vi[i + 2] - 1].p.x;
					m_vnt[m_inbr][m_vntcount].p.y = m_vertex[m_vi[i + 2] - 1].p.y;
					m_vnt[m_inbr][m_vntcount].p.z = m_vertex[m_vi[i + 2] - 1].p.z;

					m_vnt[m_inbr][m_vntcount].n.x = m_normal[m_ni[i + 2] - 1].p.x;
					m_vnt[m_inbr][m_vntcount].n.y = m_normal[m_ni[i + 2] - 1].p.y;
					m_vnt[m_inbr][m_vntcount].n.z = m_normal[m_ni[i + 2] - 1].p.z;

					m_vnt[m_inbr][m_vntcount].uv.x = m_uv[m_uvi[i + 2] - 1].p.x;
					m_vnt[m_inbr][m_vntcount].uv.y = m_uv[m_uvi[i + 2] - 1].p.y;

					m_vntcount++;


					m_vnt[m_inbr][m_vntcount].p.x = m_vertex[m_vi[i + 3] - 1].p.x;
					m_vnt[m_inbr][m_vntcount].p.y = m_vertex[m_vi[i + 3] - 1].p.y;
					m_vnt[m_inbr][m_vntcount].p.z = m_vertex[m_vi[i + 3] - 1].p.z;

					m_vnt[m_inbr][m_vntcount].n.x = m_normal[m_ni[i + 3] - 1].p.x;
					m_vnt[m_inbr][m_vntcount].n.y = m_normal[m_ni[i + 3] - 1].p.y;
					m_vnt[m_inbr][m_vntcount].n.z = m_normal[m_ni[i + 3] - 1].p.z;

					m_vnt[m_inbr][m_vntcount].uv.x = m_uv[m_uvi[i + 3] - 1].p.x;
					m_vnt[m_inbr][m_vntcount].uv.y = m_uv[m_uvi[i + 3] - 1].p.y;

					m_vntcount++;
				}
			}
		}

		if ((m_inbr > -1) && (m_inbr < CMaterial::E_MATERIAL_COUNT))
		{
			m_icount[m_inbr] = m_vntcount;
		}

		fclose(file);
	}


	err = fopen_s(&file, materialname, "r");

	if (err)
	{
		return;
	}

	mnbr = -1;

	if (file)
	{
		while (fscanf_s(file, "%s", line, CWavefront::LINE_LENGTH) != EOF)
		{
			if (strcmp(line, "newmtl") == 0)
			{
				mnbr++;

				if (mnbr == CMaterial::E_MATERIAL_COUNT)
				{
					fclose(file);

					return;
				}

				bytesRead = fscanf_s(file, "%s\n", &line, CMaterial::E_MATERIAL_NAME_LENGTH);

				m_mesh[mnbr] = new CMesh();

				m_mesh[mnbr]->m_material = new CMaterial(line);
			}

			if (mnbr >= 0)
			{
				if (strcmp(line, "Ka") == 0)
				{
					bytesRead = fscanf_s(file, "%f %f %f\n",
						&m_mesh[mnbr]->m_material->m_ambient.x,
						&m_mesh[mnbr]->m_material->m_ambient.y,
						&m_mesh[mnbr]->m_material->m_ambient.z);
				}

				if (strcmp(line, "Kd") == 0)
				{
					bytesRead = fscanf_s(file, "%f %f %f\n",
						&m_mesh[mnbr]->m_material->m_diffuse.x,
						&m_mesh[mnbr]->m_material->m_diffuse.y,
						&m_mesh[mnbr]->m_material->m_diffuse.z);
				}

				if (strcmp(line, "Ks") == 0)
				{
					bytesRead = fscanf_s(file, "%f %f %f\n",
						&m_mesh[mnbr]->m_material->m_specular.x,
						&m_mesh[mnbr]->m_material->m_specular.y,
						&m_mesh[mnbr]->m_material->m_specular.z);
				}

				if (strcmp(line, "Ke") == 0)
				{
					bytesRead = fscanf_s(file, "%f %f %f\n",
						&m_mesh[mnbr]->m_material->m_emissive.x,
						&m_mesh[mnbr]->m_material->m_emissive.y,
						&m_mesh[mnbr]->m_material->m_emissive.z);
				}

				if (strcmp(line, "Ni") == 0)
				{
					bytesRead = fscanf_s(file, "%f\n", &m_mesh[mnbr]->m_material->m_opticalDensity);
				}

				if (strcmp(line, "d") == 0)
				{
					bytesRead = fscanf_s(file, "%f\n", &m_mesh[mnbr]->m_material->m_opacity);
				}

				if (strcmp(line, "illum") == 0)
				{
					bytesRead = fscanf_s(file, "%i\n", &m_mesh[mnbr]->m_material->m_illum);
				}

				if (strcmp(line, "map_Kd") == 0)
				{
					bytesRead = fscanf_s(file, "%s\n", &line, CMaterial::E_MATERIAL_NAME_LENGTH);

					m_mesh[mnbr]->m_map_Kd = new CString(line);
				}

				if (strcmp(line, "map_Ks") == 0)
				{
					bytesRead = fscanf_s(file, "%s\n", &line, CMaterial::E_MATERIAL_NAME_LENGTH);

					m_mesh[mnbr]->m_map_Ks = new CString(line);
				}

				if (strcmp(line, "map_Bump") == 0)
				{
					bytesRead = fscanf_s(file, "%s\n", &line, CMaterial::E_MATERIAL_NAME_LENGTH);

					m_mesh[mnbr]->m_map_Kb = new CString(line);
				}
			}
		}

		fclose(file);
	}

	CWavefront::LoadMeshList();

	m_isInitialized = true;
}

CWavefront::~CWavefront()
{
	delete m_name;

	delete m_materialname;

	delete m_meshs;
}

void CWavefront::WriteVertices(FILE* f, CVertex* p)
{
	if (f == nullptr)
	{
		return;
	}

	XMFLOAT3 pos = {};
	XMFLOAT3 nrm = {};
	XMFLOAT3 uv = {};

	int mcount = m_inbr + 1;

	fwrite(&mcount, sizeof(int), 1, f);

	for (int mc = 0; mc < m_inbr + 1; mc++)
	{
		fwrite(&m_icount[mc], sizeof(int), 1, f);

		for (int vc = 0; vc < m_icount[mc]; vc++)
		{
			pos.x = m_vnt[mc][vc].p.x + p->p.x;
			pos.y = m_vnt[mc][vc].p.y + p->p.y;
			pos.z = m_vnt[mc][vc].p.z + p->p.z;

			nrm.x = m_vnt[mc][vc].n.x;
			nrm.y = m_vnt[mc][vc].n.y;
			nrm.z = m_vnt[mc][vc].n.z;

			uv.x = m_vnt[mc][vc].uv.x;
			uv.y = m_vnt[mc][vc].uv.y;

			fwrite(&pos.x, sizeof(float), 1, f);
			fwrite(&pos.y, sizeof(float), 1, f);
			fwrite(&pos.z, sizeof(float), 1, f);

			fwrite(&nrm.x, sizeof(float), 1, f);
			fwrite(&nrm.y, sizeof(float), 1, f);
			fwrite(&nrm.z, sizeof(float), 1, f);

			fwrite(&uv.x, sizeof(float), 1, f);
			fwrite(&uv.y, sizeof(float), 1, f);
		}
	}
}

void CWavefront::LoadMeshList()
{
	for (int mc = 0; mc < m_inbr + 1; mc++)
	{
		if (m_icount[mc] > 0)
		{
			CMesh* mesh = new CMesh();


			mesh->m_vertexCount = m_icount[mc];

			size_t bufferSize = mesh->m_vertexCount * sizeof(CVertexNT);

			mesh->m_vertices = new BYTE[bufferSize];

			memcpy((void*)mesh->m_vertices, (void*)m_vnt[mc], bufferSize);


			mesh->m_material = new CMaterial();

			memcpy((void*)mesh->m_material, (void*)m_mesh[mc]->m_material, sizeof(CMaterial));

			mesh->m_material->m_name = new CString(m_mesh[mc]->m_material->m_name->GetText());


			if (m_mesh[mc]->m_map_Ka != nullptr)
			{
				mesh->m_map_Ka = new CString(m_mesh[mc]->m_map_Ka->GetText());
			}

			if (m_mesh[mc]->m_map_Kd != nullptr)
			{
				mesh->m_map_Kd = new CString(m_mesh[mc]->m_map_Kd->GetText());
			}

			if (m_mesh[mc]->m_map_Ks != nullptr)
			{
				mesh->m_map_Ks = new CString(m_mesh[mc]->m_map_Ks->GetText());
			}

			if (m_mesh[mc]->m_map_Ns != nullptr)
			{
				mesh->m_map_Ns = new CString(m_mesh[mc]->m_map_Ns->GetText());
			}

			if (m_mesh[mc]->m_map_d != nullptr)
			{
				mesh->m_map_d = new CString(m_mesh[mc]->m_map_d->GetText());
			}

			if (m_mesh[mc]->m_map_Kb != nullptr)
			{
				mesh->m_map_Kb = new CString(m_mesh[mc]->m_map_Kb->GetText());
			}

			if (m_mesh[mc]->m_refl != nullptr)
			{
				mesh->m_refl = new CString(m_mesh[mc]->m_refl->GetText());
			}

			m_meshs->Add(mesh, mesh->m_material->m_name->GetText());
		}
	}

	delete[] m_vertex;
	delete[] m_normal;
	delete[] m_uv;

	for (int i = 0; i < CMaterial::E_MATERIAL_COUNT; i++)
	{
		delete[] m_vnt[i];
		delete[] m_vntl[i];

		if (m_mesh[i])
		{
			delete m_mesh[i];
		}
	}
}