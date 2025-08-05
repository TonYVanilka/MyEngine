#pragma once

#include <vector>
#include <glad/glad.h>
#include "Vertex.h"

class Mesh {

public:

	Mesh() = default;

	~Mesh();

	void UploadData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices = {});

	void Draw(uint32_t count) const;
	void Bind() const;

private:

	GLuint VAO = 0, VBO = 0, EBO = 0;
	size_t vertexCount = 0; 
	bool usesIndices = false; 

};