#include "StructuredMesh.h"

StructuredMesh::StructuredMesh()
{
}

StructuredMesh::~StructuredMesh()
{
}

void StructuredMesh::addTriangle(int v1, int v2, int v3)
{
	int idx = tris.size();
	tris.push_back(new Triangle(idx, v1, v2, v3));

	//set up structure

	verts[v1]->triList.push_back(idx);
	verts[v2]->triList.push_back(idx);
	verts[v3]->triList.push_back(idx);

	if (!makeVertsNeighbor(v1, v2))
		addEdge(v1, v2);

	if (!makeVertsNeighbor(v1, v3))
		addEdge(v1, v3);

	if (!makeVertsNeighbor(v2, v3))
		addEdge(v2, v3);

}

bool StructuredMesh::makeVertsNeighbor(int v1i, int v2i)
{
	//returns true if v1i already neighbor w/ v2i; false o/w

	for (int i = 0; i < verts[v1i]->vertList.size(); i++)
		if (verts[v1i]->vertList[i] == v2i)
			return true;


	verts[v1i]->vertList.push_back(v2i);
	verts[v2i]->vertList.push_back(v1i);
	return false;
}

void StructuredMesh::addVertex()
{
	int idx = verts.size();
	verts.push_back(new Vertex(idx));
}

void StructuredMesh::addEdge(int v1, int v2)
{
	int idx = edges.size();

	edges.push_back(new Edge(idx, v1, v2));

	verts[v1]->edgeList.push_back(idx);
	verts[v2]->edgeList.push_back(idx);
}

StructuredMesh::StructuredMesh(std::string off_name)
{
	glm::vec3 default_color(1.0f, 0.03f, 1.0f);
	minBound.x = FLT_MAX;
	minBound.y = FLT_MAX;
	minBound.z = FLT_MAX;

	maxBound.x = FLT_MIN;
	maxBound.y = FLT_MIN;
	maxBound.z = FLT_MIN;

	std::ifstream off_file;
	off_file.open(off_name);

	int nVerts, nTris, n;
	std::string type;

	std::getline(off_file, type);
	std::cout << type << std::endl;

	if (type != "OFF")
	{
		std::cout << "file type not supported!" << std::endl;
	}

	off_file >> nVerts >> nTris >> n;

	vertices = new float[nVerts * 3];
	normals = new float[nVerts * 3];
	colors = new float[nVerts * 3];
	indices = new int[nTris * 3];

	vertices_size = nVerts * 3;
	indices_size = nTris * 3;

	float x, y, z;
	for (int i = 0; i < nVerts; i++)
	{
		off_file >> x >> y >> z;
		addVertex();

		vertices[i * 3] = x;
		vertices[i * 3 + 1] = y;
		vertices[i * 3 + 2] = z;

		colors[i * 3] = default_color.x * x / 100.0f;
		colors[i * 3 + 1] = default_color.y * y / 100.0f;
		colors[i * 3 + 2] = default_color.z * z / 100.0f;

		if (x > maxBound.x) maxBound.x = x;
		if (y > maxBound.y) maxBound.y = y;
		if (z > maxBound.z) maxBound.z = z;

		if (x < minBound.x) minBound.x = x;
		if (y < minBound.y) minBound.y = y;
		if (z < minBound.z) minBound.z = z;
	}
	int c, v1, v2, v3;

	int* normal_count = new int[nVerts];
	glm::vec3* normalsx = new glm::vec3[nVerts];
	for (int i = 0; i < nVerts; i++)
	{
		normalsx[i] = glm::vec3(0.0f, 0.0f, 0.0f);
		normal_count[i] = 0;
	}

	for (int i = 0; i < nTris; i++)
	{
		off_file >> c >> v1 >> v2 >> v3;
		addTriangle(v1, v2, v3);
		indices[i * 3] = v1;
		indices[i * 3 + 1] = v2;
		indices[i * 3 + 2] = v3;

		glm::vec3 vert0(vertices[v1 * 3], vertices[v1 * 3 + 1], vertices[v1 * 3 + 2]);
		glm::vec3 vert1(vertices[v2 * 3], vertices[v2 * 3 + 1], vertices[v2 * 3 + 2]);
		glm::vec3 vert2(vertices[v3 * 3], vertices[v3 * 3 + 1], vertices[v3 * 3 + 2]);

		glm::vec3 normal = glm::cross(vert2 - vert0, vert1 - vert0);
		normal = glm::normalize(normal);
		//std::cout << "normal " << i << " x: " << normal.x << " y: " << normal.y << " z: " << normal.z << std::endl;

		normal_count[v1]++;
		normal_count[v2]++;
		normal_count[v3]++;

		normalsx[v1] += normal;
		normalsx[v2] += normal;
		normalsx[v3] += normal;
	}

	for (int i = 0; i < nVerts; i++)
	{
		//std::cout << normal_count[i] << std::endl;
		normalsx[i] = normalsx[i] / (float)normal_count[i];

		normals[i * 3] = normalsx[i].x;
		normals[i * 3 + 1] = normalsx[i].y;
		normals[i * 3 + 2] = normalsx[i].z;
		//std::cout << "normal " << i << " x: " << normalsx[i].x << " y: " << normalsx[i].y << " z: " << normalsx[i].z << std::endl;
	}

	off_file.close();
	return;
}

void StructuredMesh::initVBO()
{
	glGenVertexArrays(1, &VAO);
	//assert(vao > 0);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &glVertexAtrrib);
	glGenBuffers(1, &glIndices);

	glBindBuffer(GL_ARRAY_BUFFER, glVertexAtrrib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glIndices);


	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_size * 3, vertices, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * vertices_size, sizeof(float) * vertices_size, normals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * vertices_size * 2, sizeof(float) * vertices_size, colors);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices_size, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (char*)0 + sizeof(float) * vertices_size);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (char*)0 + sizeof(float) * vertices_size * 2);
}

void StructuredMesh::draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices_size * 3, GL_UNSIGNED_INT, 0);
}

GLuint StructuredMesh::getglIndices()
{
	return glIndices;
}

GLuint StructuredMesh::getglVertexAtrrib()
{
	return glVertexAtrrib;
}

GLuint StructuredMesh::getVAO()
{
	return VAO;
}