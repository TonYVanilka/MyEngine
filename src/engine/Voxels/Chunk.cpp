#include "engine/Voxels/Chunk.h"

#include <iostream>

Chunk::Chunk(int chunkX, int chunkY, int chunkZ)
{
	m_chunkX = chunkX;
	m_chunkY = chunkY;
	m_chunkZ = chunkZ;

	model = glm::translate(glm::mat4(1.0f),
	glm::vec3(m_chunkX * WIDTH, m_chunkY * HEIGHT, m_chunkZ * DEPTH));

}

void Chunk::GenerateTerrain()
{
	for (int x = 0; x < WIDTH; ++x)
	for (int y = 0; y < HEIGHT; ++y)
	for (int z = 0; z < DEPTH; ++z) {
		//if (x % 3 == 0 && y % 3 == 0 && z % 3 == 0) {
		//	// Пример генерации: ставим блоки только на четных координатах
		//	SetBlock(x, y, z, 1); // Например, блок типа 1
		//} else {
		//	// Пустой блок
		//SetBlock(x, y, z, 0);
		//}
		SetBlock(x, y, z, 1);
	}
}

void Chunk::GenerateMesh()
{
	for (int x = 0; x < WIDTH; ++x)
	for (int y = 0; y < HEIGHT; ++y)
	for (int z = 0; z < DEPTH; ++z) {
		uint8_t block = blocks[x][y][z];

		if (block == 0) continue;

		for (int face = 0; face < 6; ++face) {
			if (IsFaceVisible(x, y, z, face)) {
				AddFace(x, y, z, face, vertices, indices, static_cast<uint32_t>(vertices.size()));
			}
		}
	}
	//std::cout << "Chunk mesh generated with " << vertices.size() << " vertices and " << indices.size() << " indices." << std::endl;
}

void Chunk::AddFace(int x, int y, int z, int face, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, uint32_t indexOffset)
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

void Chunk::Render(Shader& shader) const {
	if (!IsChunkGenerated) {
		std::cerr << "Chunk not generated yet!" << std::endl;
		return;
	}
	shader.setMat4("model", model);
	mesh.Bind();
	mesh.Draw(indices.size());
}

uint8_t Chunk::GetBlockPos(int x, int y, int z) const {
	if (x < 0 || x >= WIDTH ||
		y < 0 || y >= HEIGHT ||
		z < 0 || z >= DEPTH)
		return 0; // пусто

	return blocks[x][y][z];
}

void Chunk::SetBlock(int x, int y, int z, uint8_t blockType)
{
	blocks[x][y][z] = blockType;
}

bool Chunk::IsFaceVisible(int x, int y, int z, int face) const {
	int nx = x, ny = y, nz = z;

	switch (face) {
	case 0: nx += 1; break;
	case 1: nx -= 1; break;
	case 2: ny += 1; break;
	case 3: ny -= 1; break;
	case 4: nz += 1; break;
	case 5: nz -= 1; break;
	}

	// Вне чанка — считаем пустым
	if (nx < 0 || nx >= WIDTH ||
		ny < 0 || ny >= HEIGHT ||
		nz < 0 || nz >= DEPTH)
		return true;

	return GetBlockPos(nx, ny, nz) == 0;
}

void Chunk::UploadMesh() {

	mesh.UploadData(vertices, indices);
	//std::cout << "Mesh uploaded with " << vertices.size() << " vertices and " << indices.size() << " indices." << std::endl;
}