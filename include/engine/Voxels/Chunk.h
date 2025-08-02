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

	glm::mat4 model = glm::mat4(1.0f);
	bool IsChunkGenerated = false;

	Chunk() = default;

	Chunk(int x, int y, int z);

	~Chunk() = default;

	void GenerateTerrain();
	void GenerateMesh();

	void Render(Shader& shader) const;

	//void SetBlock(int x, int y, int z, uint8_t blockType);

	void UploadMeshData();

private:

	static constexpr const uint8_t CHUNK_X = 16;
	static constexpr const uint8_t CHUNK_Y = 16;
	static constexpr const uint8_t CHUNK_Z = 16;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	uint32_t indexCount = 0;

	uint8_t blocks[CHUNK_X][CHUNK_Y][CHUNK_Z] = {};
	uint8_t blockUpdate[4][4][4] = {};

	Mesh c_mesh;

	bool IsFaceVisible(int x, int y, int z, int face) const;

	void GenFace(int x, int y, int z, int face, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, uint16_t indexOffset);

};