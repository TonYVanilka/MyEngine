#pragma once

class Texture {

public:

	Texture(const char* path, bool flip_vertically = true, int slot = 0);
	~Texture();

	unsigned int GetID() const;
	void BindToSlot(int slot = 0) const;
	void Unbind() const;

private:

	unsigned int textureID;
	int width, height, nrChannels;

};