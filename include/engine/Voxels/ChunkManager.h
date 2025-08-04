#pragma once 

#include <unordered_map>
#include <tuple>
#include "engine/Voxels/Chunk.h"
#include "engine/ShaderProgram.h"

class ChunkManager {

public:

	ChunkManager() = default;
	~ChunkManager() = default;
	void GenerateTerrains();
	void GenerateMeshes();
	void RenderChunks(Shader& shader) const;
	void UpdateChunks();

	struct ChunkCoordHash {
		size_t operator()(const std::tuple<int, int, int>& key) const {
			auto& [x, y, z] = key;
			return std::hash<int>()(x) ^ (std::hash<int>()(y) << 1) ^ (std::hash<int>()(z) << 2);
		}
	};

	using ChunkCoord = std::tuple<int, int, int>;
	std::unordered_map<ChunkCoord, Chunk, ChunkCoordHash> chunks;

private:

	int chunkCountX = 4;
	int chunkCountY = 2;
	int chunkCountZ = 4;

	uint8_t GetBlockAt(int gx, int gy, int gz) const;

};