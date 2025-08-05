#include "engine/Voxels/ChunkManager.h"

#include <iostream>

void ChunkManager::GenerateTerrains()
{
	for (int x = -chunkCountX / 2; x < chunkCountX / 2; ++x)
	for (int y = -chunkCountY / 2; y < chunkCountY / 2; ++y)
	for (int z = -chunkCountZ / 2; z < chunkCountZ / 2; ++z)
	{
		Chunk chunk(x, y, z);
		chunk.GenerateTerrain();
		//chunk.GenerateMesh();
		chunks.emplace(std::make_tuple(x, y, z), std::move(chunk));
		//chunks[{x, 0, z}].UploadMeshData();
	}
	std::cout << "Generated " << chunks.size() << " chunks." << std::endl;
}

uint8_t ChunkManager::GetBlockAt(int gx, int gy, int gz) const {
	int chunkX = gx / Chunk::CHUNK_X;
	int chunkY = gy / Chunk::CHUNK_Y;
	int chunkZ = gz / Chunk::CHUNK_Z;

	int localX = gx % Chunk::CHUNK_X;
	int localY = gy % Chunk::CHUNK_Y;
	int localZ = gz % Chunk::CHUNK_Z;

	if (localX < 0) { chunkX--; localX += Chunk::CHUNK_X; }
	if (localY < 0) { chunkY--; localY += Chunk::CHUNK_Y; }
	if (localZ < 0) { chunkZ--; localZ += Chunk::CHUNK_Z; }

	auto it = chunks.find({ chunkX, chunkY, chunkZ });
	if (it == chunks.end()) return 0;

	return it->second.blocks[localX][localY][localZ];
}

void ChunkManager::GenerateMeshes() {
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

