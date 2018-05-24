#pragma once
#include"cTexture.h"

class TextureBuilder {

public:
	eae6320::Graphics::cTexture::Handle s_Texture;
	eae6320::cResult LoadTexture(char * textureTarget);

};