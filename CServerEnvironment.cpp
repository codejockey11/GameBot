#include "CServerEnvironment.h"

/*
*/
CServerEnvironment::CServerEnvironment()
{
	memset(this, 0x00, sizeof(CServerEnvironment));
}

/*
* 
* Collision
*
* B  E-F
* |\  \|
* A-C  D
*
* BAC is N1
* DFE is N2
*
* m_gridUnits represents a collection of triangles for an area of the terrain stored in a list.
* This will determine the number of terrain cubes for the width and height of the terrain.
* If the terrain is 256x256 dividing the terrain size by 8 m_gridUnits yields 32x32 cubes.
* This grid will enable the use of another list that defines what groups will be drawn during rendering.
*
*/
CServerEnvironment::CServerEnvironment(const char* filename)
{
	memset(this, 0x00, sizeof(CServerEnvironment));

	m_name = new CString(filename);

	m_modelManager = new CModelManager();

	m_wavefrontManager = new CWavefrontManager();


	FILE* file = {};

	errno_t err = fopen_s(&file, "terrain\\collision.txt", "rb");

	if (err != 0)
	{
		return;
	}

	size_t bytesRead = 0;

	bytesRead = fread_s(&m_width, sizeof(int), sizeof(int), 1, file);
	bytesRead = fread_s(&m_height, sizeof(int), sizeof(int), 1, file);
	bytesRead = fread_s(&m_primSize, sizeof(int), sizeof(int), 1, file);


	// Number of vertices contained in one grid cube
	m_gridUnits = 8;

	m_gridHeight = (m_height / m_gridUnits) + 1;
	m_gridWidth = (m_width / m_gridUnits) + 1;


	// These collectable items would be defined in the environment file from the editor or compiler.
	// Allocating the space for the objects as this does not perform the constructor. 
	m_collectables = new CHeapArray(sizeof(CLinkList<CObject>), true, true, 2, m_gridWidth, m_gridHeight);

	CWavefront* wavefront = m_wavefrontManager->Create("model\\cube.wft", "model\\cube.mtl");

	CObject* collectable = new CObject(wavefront->m_meshs);

	collectable->SetScale(0.250f, 0.250f, 0.250f);
	collectable->SetPosition(5.0f, 0.50f, 5.0f);
	collectable->UpdateServer();

	// Determine which cube list to add the object.
	// Would need to test for all the vertices of the models making them contained in each block they belong with.
	m_px = (int)(collectable->m_position.x + (m_width / 2.0f)) / m_gridUnits;
	m_pz = (int)(collectable->m_position.z + (m_height / 2.0f)) / m_gridUnits;

	CLinkList<CObject>* cube = (CLinkList<CObject>*)m_collectables->GetElement(2, m_px, m_pz);

	if (cube != nullptr)
	{
		// Need to perform the constructor when this object is the first one to be added.
		if (cube->m_list == nullptr)
		{
			cube->Constructor();
		}

		cube->Add(collectable, "item01");
	}

	
	// These items would be defined in the environment file from the editor or compiler
	m_redTeamStarts = new CLinkList<CTeamStart>();
	m_blueTeamStarts = new CLinkList<CTeamStart>();

	// Not using a heap array for these
	m_redTeamStarts->Add(new CTeamStart(true, CVertex(-8.0f, 8.0f, -8.0f), CVertex(0.0f, 359.0f, 0.0f)), 0);
	m_redTeamStarts->Add(new CTeamStart(true, CVertex(-4.0f, 8.0f, -8.0f), CVertex(0.0f, 359.0f, 0.0f)), 1);
	m_redTeamStarts->Add(new CTeamStart(true, CVertex(0.0f, 8.0f, -8.0f), CVertex(0.0f, 359.0f, 0.0f)), 2);
	m_redTeamStarts->Add(new CTeamStart(true, CVertex(4.0f, 8.0f, -8.0f), CVertex(0.0f, 359.0f, 0.0f)), 3);
	m_redTeamStarts->Add(new CTeamStart(true, CVertex(8.0f, 8.0f, -8.0f), CVertex(0.0f, 359.0f, 0.0f)), 4);

	m_blueTeamStarts->Add(new CTeamStart(true, CVertex(-8.0f, 8.0f, 8.0f), CVertex(0.0f, 180.0f, 0.0f)), 0);
	m_blueTeamStarts->Add(new CTeamStart(true, CVertex(-4.0f, 8.0f, 8.0f), CVertex(0.0f, 180.0f, 0.0f)), 1);
	m_blueTeamStarts->Add(new CTeamStart(true, CVertex(0.0f, 8.0f, 8.0f), CVertex(0.0f, 180.0f, 0.0f)), 2);
	m_blueTeamStarts->Add(new CTeamStart(true, CVertex(4.0f, 8.0f, 8.0f), CVertex(0.0f, 180.0f, 0.0f)), 3);
	m_blueTeamStarts->Add(new CTeamStart(true, CVertex(8.0f, 8.0f, 8.0f), CVertex(0.0f, 180.0f, 0.0f)), 4);

	
	// Now the collision primitives from the terrain.
	// Any models defined in the environment would need to have their collisions added as well.
	m_collisions = new CHeapArray(sizeof(CLinkList<CTerrainCollision>), true, true, 2, m_gridWidth, m_gridHeight);

	CTerrainCollision* collision = new CTerrainCollision();

	bytesRead = fread_s(&collision->m_a, sizeof(CVertex), sizeof(CVertex), 1, file);

	while (!feof(file))
	{
		bytesRead = fread_s(&collision->m_b, sizeof(CVertex), sizeof(CVertex), 1, file);
		bytesRead = fread_s(&collision->m_c, sizeof(CVertex), sizeof(CVertex), 1, file);
		bytesRead = fread_s(&collision->m_n1, sizeof(CVertex), sizeof(CVertex), 1, file);

		// Simple centroid for the terrain primitives
		CVertex v = v.Centroid(&collision->m_a, &collision->m_b, &collision->m_c);

		m_px = (int)((v.p.x + ((m_width / 2.0f))) / (m_gridUnits));
		m_pz = (int)((v.p.z + ((m_height / 2.0f))) / (m_gridUnits));

		CLinkList<CTerrainCollision>* cube = (CLinkList<CTerrainCollision>*)m_collisions->GetElement(2, m_px, m_pz);

		if (cube != nullptr)
		{
			// Performing constructor on first add.
			if (cube->m_list == nullptr)
			{
				cube->Constructor();
			}

			cube->Add(collision, 0);
		}

		collision = new CTerrainCollision();

		bytesRead = fread_s(&collision->m_a, sizeof(CVertex), sizeof(CVertex), 1, file);
	}

	// Must delete the last collision object when end-of-file happens as it is not needed.
	delete collision;

	fclose(file);
}

/*
*/
CServerEnvironment::~CServerEnvironment()
{
	// Must delete each cube object's list
	for (UINT pz = 0; pz < m_gridHeight; pz++)
	{
		for (UINT px = 0; px < m_gridWidth; px++)
		{
			CLinkList<CTerrainCollision>* collisions = (CLinkList<CTerrainCollision>*)m_collisions->GetElement(2, px, pz);

			if (collisions->m_list != nullptr)
			{
				CLinkListNode<CTerrainCollision>* lln = collisions->m_list;

				while (lln->m_object)
				{
					delete lln->m_object;

					lln = lln->m_next;
				}
			}
			CLinkList<CObject>* collectables = (CLinkList<CObject>*)m_collectables->GetElement(2, px, pz);

			if (collectables->m_list != nullptr)
			{
				CLinkListNode<CObject>* lln = collectables->m_list;

				while (lln->m_object)
				{
					delete lln->m_object;

					lln = lln->m_next;
				}
			}
		}
	}

	delete m_collisions;
	delete m_collectables;
	delete m_blueTeamStarts;
	delete m_redTeamStarts;
	delete m_wavefrontManager;
	delete m_modelManager;
	delete m_name;
}