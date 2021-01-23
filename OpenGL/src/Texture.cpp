#include "hdr\Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB_IMAGE/stb_image.h>

unsigned int Texture::getUnit(unsigned int unit)
{
	switch (unit)
	{
	case 0:
		return GL_TEXTURE0;
	case 1:
		return GL_TEXTURE1;
	case 2:
		return GL_TEXTURE2;
	case 3:
		return GL_TEXTURE3;
	case 4:
		return GL_TEXTURE4;
	case 5:
		return GL_TEXTURE5;
	case 6:
		return GL_TEXTURE6;
	case 7:
		return GL_TEXTURE7;
	case 8:
		return GL_TEXTURE8;
	case 9:
		return GL_TEXTURE9;
	case 10:
		return GL_TEXTURE10;
	case 11:
		return GL_TEXTURE11;
	case 12:
		return GL_TEXTURE12;
	case 13:
		return GL_TEXTURE13;
	case 14:
		return GL_TEXTURE14;
	case 15:
		return GL_TEXTURE15;
	default:
		return 0;
	}
	return 0;
}

Texture::Texture()
{
	this->newPos = 0;
}

Texture::~Texture()
{
}

void Texture::AddTexture(const std::string path, unsigned int channel, bool flip)
{
	if (newPos > 14) { std::cout << "[ERROR] UNABLE TO ADD TEXTURE, maximun reached\n"; return; }

	glGenTextures(1, this->ids + this->newPos);
	glBindTexture(GL_TEXTURE_2D, this->ids[this->newPos]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->newPos += 1;


	int width, height, nrChannels;

	if (flip) stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);


	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channel, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		#ifdef DEBUG
			std::cout << "\t[DEBUG] Texture added\n";
		#endif // DEBUG

	}
	else
		std::cout << "Failed to load texture" << std::endl;

	stbi_image_free(data);
}

void Texture::Active(unsigned int id)
{
	if (id < 15)
	{
		glActiveTexture(this->getUnit(id));
		glBindTexture(GL_TEXTURE_2D, this->ids[id]);
	}
	else
	{
		std::cout << "[WARNING] OUT OF BOUNDS TO ACTIVE TEXTURE, select one from 0 to 15\n";
	}
}
