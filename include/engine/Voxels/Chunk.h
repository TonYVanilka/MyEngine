#pragma once

#include <vector>
#include "engine/Vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "engine/Mesh.h"
#include "engine/ShaderProgram.h"

class ChunksManager;

class Chunk {

public:
	
	static constexpr const uint8_t WIDTH = 160;
	static constexpr const uint8_t HEIGHT = WIDTH;
	static constexpr const int DEPTH = WIDTH;
	bool IsChunkGenerated = false;

	Chunk() : m_chunkX(0), m_chunkY(0), m_chunkZ(0), model(glm::mat4(1.0f)) {
		memset(blocks, 0, sizeof(blocks));
	}

	Chunk(int chunkX, int chunkY, int chunkZ);

	~Chunk() = default;

	void GenerateTerrain();
	void GenerateMesh();

	void Render(Shader& shader) const;

	uint8_t GetBlockPos(int x, int y, int z) const;

	void SetBlock(int x, int y, int z, uint8_t blockType);

	void UploadMesh();

private:

	glm::mat4 model = glm::mat4(1.0f);

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	int m_chunkX, m_chunkY, m_chunkZ = 0;
	uint8_t blocks[WIDTH][HEIGHT][DEPTH] = {};

	uint8_t blockUpdate[4][4][4] = {};

	Mesh mesh;

	bool IsFaceVisible(int x, int y, int z, int face) const;

	void AddFace(int x, int y, int z, int face, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, uint32_t indexOffset);

};