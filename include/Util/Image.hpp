/*
	Copyright (C) 2012 Alexander Overvoorde

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
	the Software, and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
*/

#pragma once

#ifndef OOGL_IMAGE_HPP
#define OOGL_IMAGE_HPP

#pragma once

#ifndef OOGL_PLATFORM_HPP
#define OOGL_PLATFORM_HPP

/*
	Platform identification
*/

#if defined( _WIN32 )
	#define OOGL_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <WindowsX.h>
	#include <GL/GL.h>
#elif defined( __linux__ )
	#define OOGL_PLATFORM_LINUX
	#include <X11/Xlib.h>
	#include <GL/gl.h>
	#include <GL/glx.h>
	#include <sys/time.h>
#elif defined( __APPLE__ )
	#define OOGL_PLATFORM_OSX
#endif

/*
	Types
*/

namespace GL
{
	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef unsigned int uint;
	typedef unsigned long ulong;

	namespace Type
	{
		enum type_t
		{
			Byte = GL_BYTE,
			UnsignedByte = GL_UNSIGNED_BYTE,
			Short = GL_SHORT,
			UnsignedShort = GL_UNSIGNED_SHORT,
			Int = GL_INT,
			UnsignedInt = GL_UNSIGNED_INT,
			Float = GL_FLOAT,
			Double = GL_DOUBLE
		};
	}

	typedef GLint Attribute;
	typedef GLint Uniform;
}

#endif
#include <Util/Color.hpp>
#include <Util/ByteBuffer.hpp>
#include <string>
#include <exception>

namespace GL
{
	/*
		Supported image file formats
	*/
	namespace ImageFileFormat
	{
		enum image_file_format_t
		{
			BMP,
			TGA,
			JPEG,
			PNG
		};
	}

	/*
		Exceptions
	*/
	class FileException : public std::exception
	{
		virtual const char* what() const throw()
		{
			return "File could not be opened!";
		}
	};
	class FormatException : public std::exception
	{
		virtual const char* what() const throw()
		{
			return "Image file format not supported!";
		}
	};

	/*
		Image
	*/
	class Image
	{
	public:
		Image();

		Image( ushort width, ushort height, const Color& background );
		Image( ushort width, ushort height, uchar* pixels );
		Image( const std::string& filename );

		~Image();

		void Load( const std::string& filename );
		void Save( const std::string& filename, ImageFileFormat::image_file_format_t format );

		ushort GetWidth() const;
		ushort GetHeight() const;
		const Color* GetPixels() const;

		Color GetPixel( uint x, uint y ) const;
		void SetPixel( uint x, uint y, const Color& color );

	private:
		Color* image;
		ushort width, height;

		Image( const Image& );
		const Image& operator=( const Image& );
		
		void LoadBMP( ByteReader& data );
		void SaveBMP( const std::string& filename );

		void LoadTGA( ByteReader& data );
		void DecodeRLE( ByteReader& data, uint decodedLength, uchar bytesPerPixel );
		void SaveTGA( const std::string& filename );
		void EncodeRLE( ByteWriter& data, std::vector<uchar>& pixels, ushort width );

		void LoadJPEG( ByteReader& data );
		void SaveJPEG( const std::string& filename );

		void LoadPNG( ByteReader& data );
		void SavePNG( const std::string& filename );
	};
}

#endif
