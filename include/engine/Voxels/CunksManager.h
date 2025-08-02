#pragma once

#include <map>
#include "engine/Voxels/Chunk.h"
#include "engine/ShaderProgram.h"

class ChunksManager {

public:

	static constexpr int WORLD_WIDTH = 16;
	static constexpr int WORLD_DEPTH = 16;

	ChunksManager() = default;
	~ChunksManager() = default;

	void GenerateChunks();
	void RenderAll(Shader& shader);
	void UploadAllMeshes();

private:

	std::map<std::pair<int, int>, Chunk> chunks;

};