#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::Mesh(std::string off_name)
{
	minx = FLT_MAX;
	miny = FLT_MAX;
	minz = FLT_MAX;

	maxx = FLT_MIN;
	maxy = FLT_MIN;
	maxz = FLT_MIN;

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
	indices = new int[nTris * 3];

	vertices_size = nVerts * 3;
	indices_size = nTris * 3;

	float x, y, z;
	for (size_t i = 0; i < nVerts; i++)
	{
		off_file >> x >> y >> z;
		vertices[i * 3] = x;
		vertices[i * 3 + 1] = y;
		vertices[i * 3 + 2] = z;

		if (x > maxx) maxx = x;
		if (y > maxy) maxy = y;
		if (z > maxz) maxz = z;

		if (x < minx) minx = x;
		if (y < miny) miny = y;
		if (z < minz) minz = z;
				
	}

	int c, v1, v2, v3;
	for (size_t i = 0; i < nTris; i++)
	{
		off_file >> c >> v1 >> v2 >> v3;
		indices[i * 3] = v1;
		indices[i * 3 + 1] = v2;
		indices[i * 3 + 2] = v3;
	}

	std::cout << maxx << " " << minx << std::endl;
	std::cout << maxy << " " << miny << std::endl;
	std::cout << maxz << " " << minz << std::endl;


	off_file.close();
	return;
}

void Mesh::initVBO()
{
	glGenVertexArrays(1, &VAO);
	//assert(vao > 0);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);

	glGenBuffers(1, &glVertexAtrrib);
	glGenBuffers(1, &glIndices);

	glBindBuffer(GL_ARRAY_BUFFER, glVertexAtrrib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glIndices);


	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_size, vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices_size, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Mesh::draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices_size * 3, GL_UNSIGNED_INT, 0);
}

GLuint Mesh::getglIndices()
{
	return glIndices;
}

GLuint Mesh::getglVertexAtrrib()
{
	return glVertexAtrrib;
}

GLuint Mesh::getVAO()
{
	return VAO;
}

Mesh::~Mesh()
{
}