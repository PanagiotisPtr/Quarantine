#ifndef UTILITY_IMAGELOADER_H
#define UTILITY_IMAGELOADER_H

#include "GL/glew.h"
#include <stdio.h>
#include <exception>
#include <string>
#include <utility>

#include "StringFormatter.h"

#pragma pack(1)

namespace Utility {
	using WORD = unsigned short;
	using DWORD = unsigned int;
	using LONG = int;

	class LoadBitmapException : public std::exception {
	public:
		explicit LoadBitmapException(const char* message): msg(message) {}

		explicit LoadBitmapException(std::string&& message): msg(std::move(message)) {}

		virtual ~LoadBitmapException() noexcept {}

		virtual const char* what() const noexcept {
			return this->msg.c_str();
		}
	protected:
		std::string msg;
	};

	class ImageLoader {
	public:
		static GLuint loadImage(const char* filename) {
			unsigned char* pixelBuffer = NULL;
			BITMAPINFOHEADER infoHeader;
			BITMAPFILEHEADER fileHeader;

			loadBitmap(filename, pixelBuffer, &infoHeader, &fileHeader);


			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);

			GLuint texObject;
			glGenTextures(1, &texObject);
			glBindTexture(GL_TEXTURE_2D, texObject);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


			glTexImage2D(GL_TEXTURE_2D,
				0,
				infoHeader.biBitCount == 32 ? GL_RGBA : GL_RGB,
				infoHeader.biWidth,
				infoHeader.biHeight,
				0,
				infoHeader.biBitCount == 32 ? GL_RGBA : GL_BGR_EXT,
				GL_UNSIGNED_BYTE,
				pixelBuffer
			);


			delete[] pixelBuffer;

			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);

			return texObject;
		}
	protected:
		typedef struct tagBITMAPFILEHEADER {
			WORD  bfType;
			DWORD bfSize;
			WORD  bfReserved1;
			WORD  bfReserved2;
			DWORD bfOffBits;
		} BITMAPFILEHEADER, * PBITMAPFILEHEADER;

		typedef struct tagBITMAPINFOHEADER {
			DWORD biSize;
			LONG biWidth;
			LONG biHeight;
			WORD biPlanes;
			WORD biBitCount;
			DWORD biCompression;
			DWORD biSizeImage;
			LONG biXPelsPerMeter;
			LONG biYPelsPerMeter;
			DWORD biClrUsed;
			DWORD biClrImportant;
		} BITMAPINFOHEADER;

		static GLuint loadBitmap(const char* filename, unsigned char*& pixelBuffer, BITMAPINFOHEADER* infoHeader, BITMAPFILEHEADER* fileHeader) {
			FILE* bitmapFile;

#ifdef _MSC_VER
			errno_t err = fopen_s(&bitmapFile, filename, "rb");
			if (err != 0)
			{
				throw new LoadBitmapException(StringFormatter::formatString("loadbitmap - open failed for %s\n", filename));
				return NULL;
			}
#else
			bitmapFile = fopen(filename, "rb");
#endif

			if (bitmapFile == NULL) {
				throw new LoadBitmapException(StringFormatter::formatString("loadbitmap - open failed for %s\n", filename));
				return NULL;
			}

			fread(fileHeader, 14, 1, bitmapFile);

			if (fileHeader->bfType != 0x4D42) {
				throw new LoadBitmapException(StringFormatter::formatString("loadbitmap - type failed \n"));

				return NULL;
			}

			fread(infoHeader, sizeof(BITMAPINFOHEADER), 1, bitmapFile);
			if (infoHeader->biBitCount < 24) {
				throw new LoadBitmapException(StringFormatter::formatString("loadbitmap - bitcount failed = %d\n", infoHeader->biBitCount));

				return NULL;
			}

			fseek(bitmapFile, fileHeader->bfOffBits, SEEK_SET);
			pixelBuffer = new unsigned char[infoHeader->biWidth * infoHeader->biHeight * (infoHeader->biBitCount / 8)];
			fread(pixelBuffer, sizeof(unsigned char), infoHeader->biWidth * infoHeader->biHeight * (infoHeader->biBitCount / 8), bitmapFile);

			fclose(bitmapFile);

			if (infoHeader->biBitCount == 32) {
				unsigned char c;
				for (int i = 0; i < infoHeader->biWidth * infoHeader->biHeight; i++) {
					c = pixelBuffer[i * 4];
					pixelBuffer[i * 4] = pixelBuffer[i * 4 + 3];
					pixelBuffer[i * 4 + 3] = c;
					c = pixelBuffer[i * 4 + 1];
					pixelBuffer[i * 4 + 1] = pixelBuffer[i * 4 + 2];
					pixelBuffer[i * 4 + 2] = c;
				}
			}

			return 1;
		}
	};

} // namespace Utility

#endif