#ifndef GBUFFER_H__
#define GBUFFER_H__ 

#include "CSCIx239.h"

/* Most of this code was taken from http://ogldev.atspace.co.uk/www/tutorial35/tutorial35.html,
 * However I had to fix some bugs and slightly change some initializations;
 */

class GBuffer
{
public:

    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_TEXCOORD,
        GBUFFER_NUM_TEXTURES
    };

    static bool init(unsigned int WindowWidth, unsigned int WindowHeight);
    static void cleanUp();

    static void bindForWriting();

    static void bindForReading();

    static void setReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);

public:

    static GLuint m_fbo;
    static GLuint b_fbo;
    static GLuint m_textures[GBUFFER_NUM_TEXTURES];
    static GLuint b_textures[GBUFFER_NUM_TEXTURES];
    static GLuint m_depthTexture;
    static GLuint b_depthTexture;
};
#endif
