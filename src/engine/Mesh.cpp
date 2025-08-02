#include "engine/Mesh.h"

#include <iostream>

Mesh::~Mesh() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
}

void Mesh::UploadData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    if (vertices.empty()) {
        std::cerr << "Error: Attempting to upload empty vertex data." << std::endl;
        return;
	}

    // ������� ������ ������ (���� ��� ����)
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);

    // ������ VAO, VBO � (���� �����) EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // ����������� VAO
    glBindVertexArray(VAO);

    // ��������� ������� � VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // ���� ���� ������� � ��������� �� � EBO
    if (!indices.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
        vertexCount = indices.size();
        usesIndices = true;
    }
    else {
        vertexCount = vertices.size();
        usesIndices = false;
    }

    // ����������� �������� ������ (������� � �������)
    // 0: position (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // 1: texCoords (vec2)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(1);

    // 2: normal (vec3)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    // ���������� VAO ��� ������������
    glBindVertexArray(0);
}

void Mesh::Draw(uint32_t count) const {

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // ��� ������������

}

void Mesh::Bind() const
{
    glBindVertexArray(VAO);
}

