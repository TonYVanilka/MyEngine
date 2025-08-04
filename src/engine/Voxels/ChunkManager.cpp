#include "engine/Voxels/ChunkManager.h"

#include <iostream>

void ChunkManager::GenerateTerrains()
{
	for (int x = 0; x < chunkCountX; ++x)
	for (int z = 0; z < chunkCountZ; ++z)
	{

		Chunk chunk(x, 0, z);
		chunk.GenerateTerrain();
		//chunk.GenerateMesh();
		chunks.emplace(std::make_tuple(x, 0, z), std::move(chunk));
		//chunks[{x, 0, z}].UploadMeshData();
	}
	std::cout << "Generated " << chunks.size() << " chunks." << std::endl;
}

// В ChunkManager.cpp
uint8_t ChunkManager::GetBlockAt(int gx, int gy, int gz) const {
	int chunkX = gx / 16;
	int chunkY = gy / 16;
	int chunkZ = gz / 16;

	int localX = gx % 16;
	int localY = gy % 16;
	int localZ = gz % 16;

	if (localX < 0) { chunkX--; localX += 16; }
	if (localY < 0) { chunkY--; localY += 16; }
	if (localZ < 0) { chunkZ--; localZ += 16; }

	auto it = chunks.find({ chunkX, chunkY, chunkZ });
	if (it == chunks.end()) return 0;

	return it->second.blocks[localX][localY][localZ];
}


void ChunkManager::GenerateMeshes() {  // Исправлено название
	for (auto& [pos, chunk] : chunks) {
		if (!chunk.blocks) {
			std::cerr << "Error: borderBlocksAir is null!" << std::endl;
			continue;
		}
		chunk.GenerateMesh([&](int gx, int gy, int gz) {
			return GetBlockAt(gx, gy, gz);
			});
		chunk.UploadMeshData();
	}
	std::cout << "Generated meshes for " << chunks.size() << " chunks." << std::endl;
}

void ChunkManager::RenderChunks(Shader& shader) const
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& [pos, chunk] : chunks) 
	{
		chunk.Render(shader);
	}
}

void ChunkManager::UpdateChunks()
{
}

