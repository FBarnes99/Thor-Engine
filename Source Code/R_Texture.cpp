#include "R_Texture.h"
#include "OpenGL.h"

R_Texture::R_Texture() : Resource(Resource::TEXTURE)
{

}

R_Texture::~R_Texture()
{

}

void R_Texture::LoadOnMemory()
{

}

void R_Texture::ReleaseMemory()
{
	glDeleteBuffers(1, &buffer);
}
