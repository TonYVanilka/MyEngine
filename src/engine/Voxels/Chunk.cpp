#include "engine/Voxels/Chunk.h"

#include "engine/Vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

Chunk::Chunk(int x, int y, int z)
{
	model = glm::translate(glm::mat4(1.0f),	glm::vec3(x * CHUNK_X, y * CHUNK_Y, z * CHUNK_Z));
}

void Chunk::GenerateTerrain()
{
	for (int x = 0; x < CHUNK_X; ++x)
	for (int y = 0; y < CHUNK_Y; ++y)
	for (int z = 0; z < CHUNK_Z; ++z)
	{
		if ((x + y + z) % 2 == 0) {
			// Set block type (1 for example)
			// You can set different block types based on your needs
			blocks[x][y][z] = 1; // Example: setting all blocks to type 1
		} else {
			// Set empty block (0)
			blocks[x][y][z] = 0;
		}
		//blocks[x][y][z] = 1;
	} 
}

void Chunk::GenerateMesh()
{
	for (int x = 0; x < CHUNK_X; ++x)
	for (int y = 0; y < CHUNK_Y; ++y)
	for (int z = 0; z < CHUNK_Z; ++z)
	{
		uint8_t block = blocks[x][y][z];

		if (block == 0)
			continue;

		for (int face = 0; face < 6; ++face) {
			if (IsFaceVisible(x, y, z, face)) {
			GenFace(x, y, z, face, vertices, indices, static_cast<uint16_t>(vertices.size()));
			}
		}
	}
	//std::cout << "Vertices: " << vertices.size() << ", Indices: " << indices.size() << std::endl;
}

bool Chunk::IsFaceVisible(int x, int y, int z, int face) const
{
	int nx = x, ny = y, nz = z;

	switch (face) {
	case 0: nx += 1; break;
	case 1: nx -= 1; break;
	case 2: ny += 1; break;
	case 3: ny -= 1; break;
	case 4: nz += 1; break;
	case 5: nz -= 1; break;
	}

	// if coord out chunk 

	if (nx < 0 || nx >= CHUNK_X ||
		ny < 0 || ny >= CHUNK_Y ||
		nz < 0 || nz >= CHUNK_Z)
		return true;

	return blocks[nx][ny][nz] == 0;
}

void Chunk::GenFace(int x, int y, int z, int face, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, uint16_t indexOffset)
{

	static const glm::vec3 faceNormals[6] = {
		{ 1,  0,  0},  // +X
		{-1,  0,  0},  // -X
		{ 0,  1,  0},  // +Y
		{ 0, -1,  0},  // -Y
		{ 0,  0,  1},  // +Z
		{ 0,  0, -1}   // -Z
	};
	static const glm::vec3 faceVertices[6][4] = {
		{ {1, 0, 1}, {1, 1, 1}, {1, 1, 0}, {1, 0, 0} }, // +X
		{ {0, 0, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1} }, // -X
		{ {0, 1, 1}, {0, 1, 0}, {1, 1, 0}, {1, 1, 1} }, // +Y
		{ {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1} }, // -Y
		{ {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1} }, // +Z
		{ {1, 0, 0}, {0, 0, 0}, {0, 1, 0}, {1, 1, 0} }, // -Z
	};
	static const glm::vec2 texCoords[4] = {
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
	};

	for (int i = 0; i < 4; ++i) {
		glm::vec3 pos = faceVertices[face][i] + glm::vec3(x, y, z);
		Vertex v;
		v.position = pos;
		v.texCoords = texCoords[i];
		v.normal = faceNormals[face];
		vertices.push_back(v);
	}

	indices.push_back(indexOffset + 0);
	indices.push_back(indexOffset + 1);
	indices.push_back(indexOffset + 2);
	indices.push_back(indexOffset + 2);
	indices.push_back(indexOffset + 3);
	indices.push_back(indexOffset + 0);

}

//void Chunk::UpdateBlock(uint8_t block[x][y][z])
//{
//	
//}

void Chunk::UploadMeshData()
{
	c_mesh.UploadData(vertices, indices);
	IsChunkGenerated = true;
	indexCount = indices.size();
	vertices.clear();
	indices.clear();
	//std::cout << "Mesh data uploaded. Chunk generated: " << IsChunkGenerated << std::endl;
}

void Chunk::Render(Shader& shader) const
{
	if (!IsChunkGenerated) {
		std::cerr << "Chunk not generated yet!" << std::endl;
		return;
	}

	shader.setMat4("model", model);
	c_mesh.Bind();
	c_mesh.Draw(indexCount);
}