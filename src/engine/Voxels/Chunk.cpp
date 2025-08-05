#include "engine/Voxels/Chunk.h"

#include "engine/Vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

Chunk::Chunk(int x, int y, int z)
	: chunkX(x), chunkY(y), chunkZ(z)
{
	model = glm::translate(glm::mat4(1.0f),	glm::vec3(x * CHUNK_X, y * CHUNK_Y, z * CHUNK_Z));
}

void Chunk::GenerateTerrain()
{
	for (int x = 0; x < CHUNK_X; ++x)
	for (int y = 0; y < CHUNK_Y; ++y)
	for (int z = 0; z < CHUNK_Z; ++z)
	{
		int globalX = x + chunkX * CHUNK_X;
		int globalY = y + chunkY * CHUNK_Y;
		int globalZ = z + chunkZ * CHUNK_Z;
		if (globalY >= ((sin(globalX * 0.1f) * 0.8f + 0.8f) * 10))
			blocks[x][y][z] = 0;
		else if (globalY >= ((sin(globalX * 0.1f) * 0.8f + 0.8f) * 10) / 2) blocks[x][y][z] = 2;
		else blocks[x][y][z] = 1;
	} 
}

void Chunk::GenerateMesh(std::function<uint8_t(int, int, int)> getBlockAt)
{
	for (int x = 0; x < CHUNK_X; ++x)
	for (int y = 0; y < CHUNK_Y; ++y)
	for (int z = 0; z < CHUNK_Z; ++z)
	{
		uint8_t block = blocks[x][y][z];

		if (block == 0)
			continue;

		for (int face = 0; face < 6; ++face) {
			if (IsFaceVisible(x, y, z, face, getBlockAt)) {
				GenFace(x, y, z, face, vertices, indices, static_cast<uint16_t>(vertices.size()), block);
			}
		}
	}
	//std::cout << "Vertices: " << vertices.size() << ", Indices: " << indices.size() << std::endl;
}

bool Chunk::IsFaceVisible(int x, int y, int z, int face, std::function<uint8_t(int, int, int)> getBlockAt)
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

	int gx = chunkX * CHUNK_X + nx;
	int gy = chunkY * CHUNK_Y + ny;
	int gz = chunkZ * CHUNK_Z + nz;

	return getBlockAt(gx, gy, gz) == 0;
}

void Chunk::GenFace(int x, int y, int z, int face, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, uint16_t indexOffset, uint8_t block)
{
	const float TILE_SIZE = 1.0f / 16.0f; // јтлас 16x16

	static const glm::vec3 faceNormals[6] = {
		{ 1,  0,  0},  // +X
		{-1,  0,  0},  // -X
		{ 0,  1,  0},  // +Y
		{ 0, -1,  0},  // -Y
		{ 0,  0,  1},  // +Z
		{ 0,  0, -1}   // -Z
	};
	static const glm::vec3 faceVertices[6][4] = {
		// +X (права€ грань) - CW
		{ {1, 0, 0}, {1, 0, 1}, {1, 1, 1}, {1, 1, 0} },

		// -X (лева€ грань) - CW
		{ {0, 0, 1}, {0, 0, 0}, {0, 1, 0}, {0, 1, 1} },

		// +Y (верхн€€ грань) - CW
		{ {0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1} },

		// -Y (нижн€€ грань) - CW
		{ {0, 0, 0}, {0, 0, 1}, {1, 0, 1}, {1, 0, 0} },

		// +Z (задн€€ грань) - CW
		{ {0, 0, 1}, {0, 1, 1}, {1, 1, 1}, {1, 0, 1} },

		// -Z (передн€€ грань) - CW
		{ {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 0} },
	};

	static const glm::vec2 texCoords[4] = {
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
	};

	int tileX = block % 16; 
	int tileY = block / 16;
	int flippedTileY = 15 - tileY;

	for (int i = 0; i < 4; ++i) {
		glm::vec3 pos = faceVertices[face][i] + glm::vec3(x, y, z);
		Vertex v;
		v.position = pos;
		v.texCoords = glm::vec2(
			tileX * TILE_SIZE + texCoords[i].x * TILE_SIZE,
			flippedTileY * TILE_SIZE + texCoords[i].y * TILE_SIZE
		);
		v.normal = faceNormals[face];
		vertices.push_back(v);
	}

	indices.push_back(indexOffset + 0);
	indices.push_back(indexOffset + 1);
	indices.push_back(indexOffset + 2);

	indices.push_back(indexOffset + 0);
	indices.push_back(indexOffset + 2);
	indices.push_back(indexOffset + 3);

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