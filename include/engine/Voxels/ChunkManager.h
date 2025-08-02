#pragma once 

#include <unordered_map>
#include <tuple>
#include "engine/Voxels/Chunk.h"
#include "engine/ShaderProgram.h"

class ChunkManager {

public:

	ChunkManager() = default;
	~ChunkManager() = default;
	void GenerateChunks();
	void RenderChunks(Shader& shader) const;
	void UpdateChunks();

private:

	struct ChunkCoordHash {
		size_t operator()(const std::tuple<int, int, int>& key) const {
			auto &[x, y, z] = key;
			return std::hash<int>()(x) ^ (std::hash<int>()(y) << 1) ^ (std::hash<int>()(z) << 2);
		}
	};

	using ChunkCoord = std::tuple<int, int, int>;
	std::unordered_map<ChunkCoord, Chunk, ChunkCoordHash> chunks;

	int chunkCountX = 2;
	int chunkCountY = 2;
	int chunkCountZ = 2;

};