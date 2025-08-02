#include "engine/Voxels/ChunkManager.h"

#include <iostream>

void ChunkManager::GenerateChunks()
{
	for (int x = 0; x < chunkCountX; ++x)
	for (int y = 0; y < chunkCountY; ++y)
	for (int z = 0; z < chunkCountZ; ++z)
	{

		Chunk chunk(x, y, z);
		chunk.GenerateTerrain();
		chunk.GenerateMesh();
		chunk.IsChunkGenerated = true;

		chunks.emplace(std::make_tuple(x, y, z), std::move(chunk));

		chunks[{x, y, z}].UploadMeshData();
	}
	std::cout << "Generated " << chunks.size() << " chunks." << std::endl;
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
