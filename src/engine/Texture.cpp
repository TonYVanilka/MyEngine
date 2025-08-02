#include "engine/Texture.h"

#include <glad/glad.h>
#include "engine/stb_image.h"
#include <iostream>

Texture::Texture(const char* path, bool flip_vetically, int slot) : width(0), height(0), nrChannels(0)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data) {
		std::cout << "Failed to load texture at path: " << path << std::endl;
        textureID = 0;
        return;
    }

    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format = GL_RGB;
    if (nrChannels == 4)
        format = GL_RGBA;
    else if (nrChannels == 1)
        format = GL_RED;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

}

Texture::~Texture()
{
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}

void Texture::BindToSlot(int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::GetID() const {
    return textureID;
}