//
//  Load texture from BMP file
//
//  WARNING:  This is a minimalist implementation of a texture loader.  It will
//  only load a very specific type of image file.  It is intended to be a
//  starting point to allow you to load textures.
//
//  If you want to use more commonly used and flexible formats like PNG, TIFF or
//  JPEG, you need to implement your own loader.  Libraries like libtiff,
//  libjpeg and libpng can be used to load the files into memory.
//  Alternatively, Qt and SDL provides functionality to load textures.
//

#include "CSCIx239.h"
#include <Util/Image.hpp>
#include <gl/glu.h>

/*
 *  Reverse n bytes
 */
static void Reverse(void* x,const int n)
{
   int k;
   char* ch = (char*)x;
   for (k=0;k<n/2;k++)
   {
      char tmp = ch[k];
      ch[k] = ch[n-1-k];
      ch[n-1-k] = tmp;
   }
}

/*
 *  Load texture from BMP file
 */
unsigned int LoadTexBMP(const char* file)
{
   unsigned int   texture;    // Texture name
   FILE*          f;          // File pointer
   unsigned short magic;      // Image magic
   unsigned int   dx,dy,size; // Image dimensions
   unsigned short nbp,bpp;    // Planes and bits per pixel
   unsigned char* image;      // Image data
   unsigned int   k;          // Counter
   unsigned int   off;        // Image offset
   int            max;        // Maximum texture dimensions

//   //  Open file
//   f = fopen(file,"rb");
//   if (!f) Fatal("Cannot open file %s\n",file);
//   //  Check image magic
//   if (fread(&magic,2,1,f)!=1) Fatal("Cannot read magic from %s\n",file);
//   if (magic!=0x4D42 && magic!=0x424D) Fatal("Image magic not BMP in %s\n",file);
//   //  Seek to and read header
//   if (fseek(f,8,SEEK_CUR) || fread(&off,4,1,f)!=1 ||
//       fseek(f,4,SEEK_CUR) || fread(&dx,4,1,f)!=1 || fread(&dy,4,1,f)!=1 ||
//       fread(&nbp,2,1,f)!=1 || fread(&bpp,2,1,f)!=1 || fread(&k,4,1,f)!=1)
//     Fatal("Cannot read header from %s\n",file);
//   //  Reverse bytes on big endian hardware (detected by backwards magic)
//   if (magic==0x424D)
//   {
//      Reverse(&off,4);
//      Reverse(&dx,4);
//      Reverse(&dy,4);
//      Reverse(&nbp,2);
//      Reverse(&bpp,2);
//      Reverse(&k,4);
//   }
//   //  Check image parameters
//   glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max);
//   if (dx<1 || dx>max) Fatal("%s image width %d out of range 1-%d\n",file,dx,max);
//   if (dy<1 || dy>max) Fatal("%s image height %d out of range 1-%d\n",file,dy,max);
//   if (nbp!=1)  Fatal("%s bit planes is not 1: %d\n",file,nbp);
//   if (bpp!=24) Fatal("%s bits per pixel is not 24: %d\n",file,bpp);
//   if (k!=0)    Fatal("%s compressed files not supported\n",file);
//#ifndef GL_VERSION_2_0
//   //  OpenGL 2.0 lifts the restriction that texture size must be a power of two
//   for (k=1;k<dx;k*=2);
//   if (k!=dx) Fatal("%s image width not a power of two: %d\n",file,dx);
//   for (k=1;k<dy;k*=2);
//   if (k!=dy) Fatal("%s image height not a power of two: %d\n",file,dy);
//#endif
//
//   //  Allocate image memory
//   size = 3*dx*dy;
//   image = (unsigned char*) malloc(size);
//   if (!image) Fatal("Cannot allocate %d bytes of memory for image %s\n",size,file);
//   //  Seek to and read image
//   if (fseek(f,off,SEEK_SET) || fread(image,size,1,f)!=1) Fatal("Error reading data from image %s\n",file);
//   fclose(f);
//   //  Reverse colors (BGR -> RGB)
//   for (k=0;k<size;k+=3)
//   {
//      unsigned char temp = image[k];
//      image[k]   = image[k+2];
//      image[k+2] = temp;
//   }

   //  Sanity check
   GL::Image im;
   std::string fn(file);
   im.Load(fn);
   ErrCheck("LoadTexBMP");
   //  Generate 2D texture
   glGenTextures(1,&texture);
   glBindTexture(GL_TEXTURE_2D,texture);
   //  Copy image
   gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,im.GetWidth(),im.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,im.GetPixels());
   if (glGetError()) Fatal("Error in glTexImage2D %s %dx%d\n",file,dx,dy);
   //  Scale linearly when image size doesn't match
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );
   printf("Fui chamado\n");

//   //  Free image memory
//   free(image);
   //  Return texture name
   return texture;
}
