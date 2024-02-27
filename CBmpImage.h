#pragma once

#include "framework.h"

class CBmpImage
{
public:
		
	BITMAPFILEHEADER m_bmapHeader;
	
	BITMAPINFO m_bmapInfo;
	
	BYTE* m_pixels32;
	BYTE* m_pixels8;
	
	PALETTEENTRY m_paletteentry[256];
	
	bool m_isInitialized;
	
	int m_bytesPerPixel;

	CBmpImage();
	CBmpImage(const char* name);
	~CBmpImage();

	void WriteBitmapHeader(UINT width, UINT height, FILE* file);
	void WriteGreyscalePalette(FILE* file);

private:

	BYTE* m_p32;
	BYTE* m_p8;
	
	FILE* m_file;

	errno_t m_err;

	size_t m_bytesRead;
	size_t m_count;
	size_t m_size32;
	size_t m_size8;
};