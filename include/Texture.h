#ifndef TEXTURE_H__
#define TEXTURE_H__ 

#include <CSCIx239.h>
#include <String>
#include <Util/Image.hpp>

class Texture
{
private:
	int textureUnit;
	int texture;

	// Static attributes
	static int defaultTexture;
	static int defaultNormalMap;
public:
	Texture(int textureUnit, bool isNormalMap = false);
	Texture(int textureUnit, std::string fileName);
	~Texture();
	
	/**
	 *	Active this texture
	 */
	void active() const;

	/**
	 *	Bind this texture
	 */
	void bind() const;

	// Static methods
	static int getDefaultTexture();
	static int getDefaultNormalMap();
};

#endif
