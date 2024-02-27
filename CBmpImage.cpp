#include "CBmpImage.h"

/*
*/
CBmpImage::CBmpImage()
{
	memset(this, 0x00, sizeof(CBmpImage));
}

/*
*/
CBmpImage::CBmpImage(const char* filename)
{
	memset(this, 0x00, sizeof(CBmpImage));

	m_err = fopen_s(&m_file, filename, "rb");

	if (m_err)
	{
		return;
	}

	m_bytesRead = fread_s(&m_bmapHeader.bfType, sizeof(WORD), sizeof(WORD), 1, m_file);
	m_bytesRead = fread_s(&m_bmapHeader.bfSize, sizeof(DWORD), sizeof(DWORD), 1, m_file);
	m_bytesRead = fread_s(&m_bmapHeader.bfReserved1, sizeof(WORD), sizeof(WORD), 1, m_file);
	m_bytesRead = fread_s(&m_bmapHeader.bfReserved2, sizeof(WORD), sizeof(WORD), 1, m_file);
	m_bytesRead = fread_s(&m_bmapHeader.bfOffBits, sizeof(DWORD), sizeof(DWORD), 1, m_file);

	m_bytesRead = fread_s(&m_bmapInfo.bmiHeader.biSize, sizeof(DWORD), sizeof(DWORD), 1, m_file);
	m_bytesRead = fread_s(&m_bmapInfo.bmiHeader.biWidth, sizeof(LONG), sizeof(LONG), 1, m_file);
	m_bytesRead = fread_s(&m_bmapInfo.bmiHeader.biHeight, sizeof(LONG), sizeof(LONG), 1, m_file);
	m_bytesRead = fread_s(&m_bmapInfo.bmiHeader.biPlanes, sizeof(WORD), sizeof(WORD), 1, m_file);
	m_bytesRead = fread_s(&m_bmapInfo.bmiHeader.biBitCount, sizeof(WORD), sizeof(WORD), 1, m_file);

	m_bytesPerPixel = m_bmapInfo.bmiHeader.biBitCount / 8;

	m_bytesRead = fread_s(&m_bmapInfo.bmiHeader.biCompression, sizeof(DWORD), sizeof(DWORD), 1, m_file);

	// only uncompressed images
	if (m_bmapInfo.bmiHeader.biCompression != BI_RGB)
	{
		fclose(m_file);

		return;
	}

	m_bytesRead = fread_s(&m_bmapInfo.bmiHeader.biSizeImage, sizeof(DWORD), sizeof(DWORD), 1, m_file);
	m_bytesRead = fread_s(&m_bmapInfo.bmiHeader.biXPelsPerMeter, sizeof(LONG), sizeof(LONG), 1, m_file);
	m_bytesRead = fread_s(&m_bmapInfo.bmiHeader.biYPelsPerMeter, sizeof(LONG), sizeof(LONG), 1, m_file);
	m_bytesRead = fread_s(&m_bmapInfo.bmiHeader.biClrUsed, sizeof(DWORD), sizeof(DWORD), 1, m_file);
	m_bytesRead = fread_s(&m_bmapInfo.bmiHeader.biClrImportant, sizeof(DWORD), sizeof(DWORD), 1, m_file);

	if (m_bmapInfo.bmiHeader.biBitCount == 8)
	{
		m_bytesRead = fread_s(&m_paletteentry, 256 * sizeof(PALETTEENTRY), sizeof(PALETTEENTRY), 256, m_file);
	}


	// reading and flipping the image
	m_size8 = (size_t)m_bmapInfo.bmiHeader.biWidth * m_bmapInfo.bmiHeader.biHeight * m_bytesPerPixel;

	m_pixels8 = (BYTE*)malloc(m_size8);

	if (!m_pixels8)
	{
		return;
	}


	m_pixels8 += (m_size8 - ((size_t)m_bmapInfo.bmiHeader.biWidth * m_bytesPerPixel));

	fseek(m_file, 0, SEEK_SET);
	fseek(m_file, m_bmapHeader.bfOffBits, SEEK_SET);

	m_count = 0;

	while (m_count < m_size8)
	{
		m_bytesRead = fread_s(m_pixels8, m_size8, ((size_t)m_bmapInfo.bmiHeader.biWidth * m_bytesPerPixel), 1, m_file);

		m_pixels8 -= ((size_t)m_bmapInfo.bmiHeader.biWidth * m_bytesPerPixel);

		m_count += m_bmapInfo.bmiHeader.biWidth * m_bytesPerPixel;
	}

	m_pixels8 += ((size_t)m_bmapInfo.bmiHeader.biWidth * m_bytesPerPixel);

	fclose(m_file);



	m_size32 = (size_t)m_bmapInfo.bmiHeader.biWidth * m_bmapInfo.bmiHeader.biHeight * 4;

	m_pixels32 = (BYTE*)malloc(m_size32);

	if (!m_pixels32)
	{
		return;
	}

	m_p8 = m_pixels8;
	m_p32 = m_pixels32;

	for (int i = 0; i < m_bmapInfo.bmiHeader.biWidth * m_bmapInfo.bmiHeader.biHeight; i++)
	{
		*m_p32 = m_paletteentry[*m_p8].peRed;
		m_p32++;

		*m_p32 = m_paletteentry[*m_p8].peGreen;
		m_p32++;

		*m_p32 = m_paletteentry[*m_p8].peBlue;
		m_p32++;

		*m_p32 = 255;
		m_p32++;

		m_p8++;
	}

	m_isInitialized = true;
}

/*
*/
CBmpImage::~CBmpImage()
{
	if (m_pixels8)
	{
		free(m_pixels8);
	}

	if (m_pixels32)
	{
		free(m_pixels32);
	}
}

/*
*/
void CBmpImage::WriteBitmapHeader(UINT width, UINT height, FILE* file)
{
	BITMAPFILEHEADER bmapHeader;
	BITMAPINFO bmapInfo;

	bmapHeader.bfType = 19778;
	fwrite(&bmapHeader.bfType, sizeof(WORD), 1, file);

	bmapHeader.bfSize = 0;
	fwrite(&bmapHeader.bfSize, sizeof(DWORD), 1, file);

	bmapHeader.bfReserved1 = 0;
	fwrite(&bmapHeader.bfReserved1, sizeof(WORD), 1, file);

	bmapHeader.bfReserved2 = 0;
	fwrite(&bmapHeader.bfReserved2, sizeof(WORD), 1, file);

	bmapHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (256 * sizeof(PALETTEENTRY));
	fwrite(&bmapHeader.bfOffBits, sizeof(DWORD), 1, file);

	bmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	fwrite(&bmapInfo.bmiHeader.biSize, sizeof(DWORD), 1, file);

	bmapInfo.bmiHeader.biWidth = width;
	fwrite(&bmapInfo.bmiHeader.biWidth, sizeof(LONG), 1, file);

	bmapInfo.bmiHeader.biHeight = height;
	fwrite(&bmapInfo.bmiHeader.biHeight, sizeof(LONG), 1, file);

	bmapInfo.bmiHeader.biPlanes = 1;
	fwrite(&bmapInfo.bmiHeader.biPlanes, sizeof(WORD), 1, file);

	bmapInfo.bmiHeader.biBitCount = 8;
	fwrite(&bmapInfo.bmiHeader.biBitCount, sizeof(WORD), 1, file);

	bmapInfo.bmiHeader.biCompression = BI_RGB;
	fwrite(&bmapInfo.bmiHeader.biCompression, sizeof(DWORD), 1, file);

	bmapInfo.bmiHeader.biSizeImage = bmapInfo.bmiHeader.biWidth * bmapInfo.bmiHeader.biHeight;
	fwrite(&bmapInfo.bmiHeader.biSizeImage, sizeof(DWORD), 1, file);

	bmapInfo.bmiHeader.biXPelsPerMeter = 0;
	fwrite(&bmapInfo.bmiHeader.biXPelsPerMeter, sizeof(LONG), 1, file);

	bmapInfo.bmiHeader.biYPelsPerMeter = 0;
	fwrite(&bmapInfo.bmiHeader.biYPelsPerMeter, sizeof(LONG), 1, file);

	bmapInfo.bmiHeader.biClrUsed = 0;
	fwrite(&bmapInfo.bmiHeader.biClrUsed, sizeof(DWORD), 1, file);

	bmapInfo.bmiHeader.biClrImportant = 0;
	fwrite(&bmapInfo.bmiHeader.biClrImportant, sizeof(DWORD), 1, file);
}

/*
*/
void CBmpImage::WriteGreyscalePalette(FILE* file)
{
	PALETTEENTRY paletteentry;

	for (UINT i = 0; i < 256; i++)
	{
		paletteentry.peRed = i;
		paletteentry.peGreen = i;
		paletteentry.peBlue = i;
		paletteentry.peFlags = 0;

		fwrite(&paletteentry, sizeof(PALETTEENTRY), 1, file);
	}
}