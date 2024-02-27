/* See comment at end for file info*/
#pragma once

#include "framework.h"

class CTgaHeader
{
public:

	BYTE m_idLength;        /* 00h  Size of Image ID field */
	BYTE m_colorMapType;    /* 01h  Color map type */
	BYTE m_imageType;       /* 02h  Image type code */
	WORD m_cMapStart;       /* 03h  Color map origin */
	WORD m_cMapLength;      /* 05h  Color map length */
	BYTE m_cMapDepth;       /* 07h  Depth of color map entries */
	WORD m_xOffset;         /* 08h  X origin of image */
	WORD m_yOffset;         /* 0Ah  Y origin of image */
	WORD m_width;           /* 0Ch  Width of image */
	WORD m_height;          /* 0Eh  Height of image */
	BYTE m_pixelDepth;      /* 10h  Image pixel size */

	// bit 0-3 contain the number of attribute bits per pixel; bits are found only for 16 and 32 bit
	// bit 4 left or right
	// bit 5 top or bottom
	// bit 6-7 are unused
	BYTE m_imageDescriptor;

	CTgaHeader()
	{
		memset(this, 0x00, sizeof(CTgaHeader));
	}
};

class CTgaFooter
{
public:

	enum
	{
		E_SIGNATURE_SIZE = 18
	};

	DWORD m_extensionOffset;	/* Extension Area Offset */
	DWORD m_developerOffset;	/* Developer Directory Offset */

	char m_signature[CTgaFooter::E_SIGNATURE_SIZE];		/* TGA Signature */

	CTgaFooter()
	{
		memset(this, 0x00, sizeof(CTgaFooter));
	}

	void ReadValues(FILE* file)
	{
		fread_s(&m_extensionOffset, sizeof(DWORD), sizeof(DWORD), 1, file);
		fread_s(&m_developerOffset, sizeof(DWORD), sizeof(DWORD), 1, file);

		fread_s(&m_signature, sizeof(char) * CTgaFooter::E_SIGNATURE_SIZE, sizeof(char), CTgaFooter::E_SIGNATURE_SIZE, file);
	}
};

class CTgaTag
{
public:

	WORD m_tagNumber; /* ID Number of the tag */
	DWORD m_offset;	/* Offset location of the tag data */
	DWORD m_size; /* Size of the tag data in bytes */

	CTgaTag()
	{
		memset(this, 0x00, sizeof(CTgaTag));
	}
};

class CTgaExtension
{
public:

	enum
	{
		E_AUTHORCOMMENT_SIZE = 324,
		E_AUTHORNAME_SIZE = 41,
		E_JOBNAME_SIZE = 41,
		E_SOFTWAREID_SIZE = 41
	};

	WORD  m_size;                   /* Extension Size */
	char  m_authorName[CTgaExtension::E_AUTHORNAME_SIZE];         /* Author Name */
	char  m_authorComment[CTgaExtension::E_AUTHORCOMMENT_SIZE];     /* Author Comment */
	WORD  m_stampMonth;             /* Date/Time Stamp: Month */
	WORD  m_stampDay;               /* Date/Time Stamp: Day */
	WORD  m_stampYear;              /* Date/Time Stamp: Year */
	WORD  m_stampHour;              /* Date/Time Stamp: Hour */
	WORD  m_stampMinute;            /* Date/Time Stamp: Minute */
	WORD  m_stampSecond;            /* Date/Time Stamp: Second */
	char  m_jobName[CTgaExtension::E_JOBNAME_SIZE];            /* Job Name/ID */
	WORD  m_jobHour;                /* Job Time: Hours */
	WORD  m_jobMinute;              /* Job Time: Minutes */
	WORD  m_jobSecond;              /* Job Time: Seconds */
	char  m_softwareId[CTgaExtension::E_SOFTWAREID_SIZE];         /* Software ID */
	WORD  m_versionNumber;          /* Software Version Number */
	BYTE  m_versionLetter;          /* Software Version Letter */
	DWORD m_keyColor;               /* Key Color */
	WORD  m_pixelNumerator;         /* Pixel Aspect Ratio */
	WORD  m_pixelDenominator;       /* Pixel Aspect Ratio */
	WORD  m_gammaNumerator;         /* Gamma Value */
	WORD  m_gammaDenominator;       /* Gamma Value */
	DWORD m_colorOffset;            /* Color Correction Offset */
	DWORD m_stampOffset;            /* Postage Stamp Offset */
	DWORD m_scanOffset;             /* Scan-Line Table Offset */
	BYTE  m_attributesType;         /* Attributes Types */

	CTgaExtension()
	{
		memset(this, 0x00, sizeof(CTgaExtension));
	}
};

class CTgaColorCorrectionTable
{
public:

	SHORT m_A;
	SHORT m_R;
	SHORT m_G;
	SHORT m_B;

	CTgaColorCorrectionTable()
	{
		memset(this, 0x00, sizeof(CTgaColorCorrectionTable));
	}
};

class BGRA
{
public:

	BYTE m_B;
	BYTE m_G;
	BYTE m_R;
	BYTE m_A;

	friend bool operator == (const BGRA& p1, const BGRA& p2)
	{
		return ((p1.m_B == p2.m_B) && (p1.m_G == p2.m_G) && (p1.m_R == p2.m_R) && (p1.m_A == p2.m_A));
	}

	friend bool operator != (const BGRA& p1, const BGRA& p2)
	{
		return !(p1 == p2);
	}

	BGRA()
	{
		memset(this, 0x00, sizeof(BGRA));
	}
};

class RGBA
{
public:

	BYTE m_R;
	BYTE m_G;
	BYTE m_B;
	BYTE m_A;

	friend bool operator == (const RGBA& p1, const RGBA& p2)
	{
		return ((p1.m_A == p2.m_A) && (p1.m_R == p2.m_R) && (p1.m_G == p2.m_G) && (p1.m_B == p2.m_B));
	}

	friend bool operator != (const RGBA& p1, const RGBA& p2)
	{
		return !(p1 == p2);
	}

	RGBA()
	{
		memset(this, 0x00, sizeof(RGBA));
	}
};

class BGR
{
public:

	BYTE m_B;
	BYTE m_G;
	BYTE m_R;

	friend bool operator == (const BGR& p1, const BGR& p2)
	{
		return ((p1.m_B == p2.m_B) && (p1.m_G == p2.m_G) && (p1.m_R == p2.m_R));
	}

	friend bool operator != (const BGR& p1, const BGR& p2)
	{
		return !(p1 == p2);
	}

	BGR()
	{
		memset(this, 0x00, sizeof(BGR));
	}
};

// Windows has a defined RGB
#undef RGB

class RGB
{
public:

	BYTE m_R;
	BYTE m_G;
	BYTE m_B;

	friend bool operator == (const RGB& p1, const RGB& p2)
	{
		return ((p1.m_R == p2.m_R) && (p1.m_G == p2.m_G) && (p1.m_B == p2.m_B));
	}

	friend bool operator != (const RGB& p1, const RGB& p2)
	{
		return !(p1 == p2);
	}

	RGB()
	{
		memset(this, 0x00, sizeof(RGB));
	}
};

/*
compression event

byte 1
bit 7			1 for compressed byte(s)
bit 6 - 0 		count 0 - 127 and add 1

8 bit
byte 2			value 0 - 255

32 bit
byte 2,3,4,5	value 0 - 255
*/
class RLEPacket
{
public:

	BYTE m_count;

	RLEPacket()
	{
		memset(this, 0x00, sizeof(RLEPacket));
	}

	void ReadCount(FILE* file)
	{
		fread_s(&m_count, sizeof(BYTE), sizeof(BYTE), 1, file);
	}

	bool IsEncoded()
	{
		if (m_count & 128)
		{
			return true;
		}

		return false;
	}

	BYTE GetCount()
	{
		// remove the RLE bit
		BYTE c = m_count << 1;

		c = c >> 1;

		// add 1 for RLE packets
		c += 1;

		return c;
	}
};

class RLEPacket8 : public RLEPacket
{
public:

	BYTE m_value;

	RLEPacket8()
	{
		memset(this, 0x00, sizeof(RLEPacket8));
	}

	void ReadValue(FILE* file)
	{
		fread_s(&m_value, sizeof(BYTE), sizeof(BYTE), 1, file);
	}
};

class RLEPacket24 : public RLEPacket
{
public:

	BGR m_value;

	RLEPacket24()
	{
		memset(this, 0x00, sizeof(RLEPacket24));
	}

	void ReadValue(FILE* file)
	{
		fread_s(&m_value, sizeof(BGR), sizeof(BGR), 1, file);
	}
};

class RLEPacket32 : public RLEPacket
{
public:

	BGRA m_value;

	RLEPacket32()
	{
		memset(this, 0x00, sizeof(RLEPacket32));
	}

	void ReadValue(FILE* file)
	{
		fread_s(&m_value, sizeof(BGRA), sizeof(BGRA), 1, file);
	}
};

class CTgaImage
{
public:

	enum
	{
		E_COLORTABLE_SIZE = 256,
		E_IMAGEDESCRIPTION_SIZE = 256,
		E_MAX_TEXT = 1024
	};

	CTgaHeader* m_header;
	CTgaFooter* m_footer;
	CTgaExtension* m_extension;

	BGR m_colorTable[CTgaImage::E_COLORTABLE_SIZE];

	BYTE m_imageDescription[CTgaImage::E_IMAGEDESCRIPTION_SIZE];

	int m_size8;
	int m_size24;
	int m_size32;

	BYTE* m_pixels8;
	BYTE* m_pixels24;
	BYTE* m_pixels32;

	bool m_isInitialized;

	CTgaImage();
	CTgaImage(const char* filename);
	~CTgaImage();

	bool Load();
	void Flip();
	void DumpHeader();
	void DumpFooter();
	void DumpColorMap();
	void DumpRawPixels8();
	void DumpRawPixels24();
	void DumpRawPixels32();

private:

	FILE* m_file;

	size_t m_err;

	char m_text[CTgaImage::E_MAX_TEXT];

	void LoadHeader();
	void LoadImageDescription();
	void LoadColorTable();
	void LoadPixelData();
	void Load8BitUncompressed();
	void Load24BitUncompressed();
	void Load32BitUncompressed();
	void Load8BitCompressed();
	void Load24BitCompressed();
	void Load32BitCompressed();
	void LoadExtensions();
};

/*
www.gamers.org/dEngine/quake3/TGA.txt

TGA specs, from the 2D graphics format web collection.
This was annotated with a remark that there are a few
inaccuracies in this ASCII version.          - b.

-----------------------------------------------------------------------
This file has been created to satisfy numerous requests for information
on Targa image file formats.  The information has been taken from
Appendix C of the Truevision Technical Guide.  Requests for further
information could be directed to:

		   AT&T
		   Electronic Photography and Imaging Center
		   2002 Wellesley Ave.
		   Indianapolis, IN 42619

This document does not pretend to be complete, but it does pretend to
be accurate.  If you discover any finger checks or erroneous information
please let me know, ( David McDuffee, 75530,2626), and I will upload the
corrections.  Thanks.

The lack of completeness is due to the fact that the Targa recognizes
over half a dozen image file formats, some of which are more widely
used than others.  I have chosen to re-key the details on only those
formats which I actually use.  Again, if you want to know more about
formats not covered here, you could contact your Truevision representative.

All Targa formats are identified by a Data Type field, which is a one
byte binary integer located in byte three of the file.  The various
file types specified by this field are as follows:

	   0  -  No image data included.
	   1  -  Uncompressed, color-mapped images.
	   2  -  Uncompressed, RGB images.
	   3  -  Uncompressed, black and white images.
	   9  -  Runlength encoded color-mapped images.
	  10  -  Runlength encoded RGB images.
	  11  -  Compressed, black and white images.
	  32  -  Compressed color-mapped data, using Huffman, Delta, and
					runlength encoding.
	  33  -  Compressed color-mapped data, using Huffman, Delta, and
					runlength encoding.  4-pass quadtree-type process.

This document will describe only four formats: 1, 2, 9, and 10.




--------------------------------------------------------------------------------
DATA TYPE 1:  Color-mapped images.                                             |
_______________________________________________________________________________|
| Offset | Length |                     Description                            |
|--------|--------|------------------------------------------------------------|
|--------|--------|------------------------------------------------------------|
|    0   |     1  |  Number of Characters in Identification Field.             |
|        |        |                                                            |
|        |        |  This field is a one-byte unsigned integer, specifying     |
|        |        |  the length of the Image Identification Field.  Its range  |
|        |        |  is 0 to 255.  A value of 0 means that no Image            |
|        |        |  Identification Field is included.                         |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    1   |     1  |  Color Map Type.                                           |
|        |        |                                                            |
|        |        |  This field contains a binary 1 for Data Type 1 images.    |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    2   |     1  |  Image Type Code.                                          |
|        |        |                                                            |
|        |        |  This field will always contain a binary 1.                |
|        |        |  ( That's what makes it Data Type 1 ).                     |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    3   |     5  |  Color Map Specification.                                  |
|        |        |                                                            |
|    3   |     2  |  Color Map Origin.                                         |
|        |        |  Integer ( lo-hi ) index of first color map entry.         |
|        |        |                                                            |
|    5   |     2  |  Color Map Length.                                         |
|        |        |  Integer ( lo-hi ) count of color map entries.             |
|        |        |                                                            |
|    7   |     1  |  Color Map Entry Size.                                     |
|        |        |  Number of bits in each color map entry.  16 for           |
|        |        |  the Targa 16, 24 for the Targa 24, 32 for the Targa 32.   |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    8   |    10  |  Image Specification.                                      |
|        |        |                                                            |
|    8   |     2  |  X Origin of Image.                                        |
|        |        |  Integer ( lo-hi ) X coordinate of the lower left corner   |
|        |        |  of the image.                                             |
|        |        |                                                            |
|   10   |     2  |  Y Origin of Image.                                        |
|        |        |  Integer ( lo-hi ) Y coordinate of the lower left corner   |
|        |        |  of the image.                                             |
|        |        |                                                            |
|   12   |     2  |  Width of Image.                                           |
|        |        |  Integer ( lo-hi ) width of the image in pixels.           |
|        |        |                                                            |
|   14   |     2  |  Height of Image.                                          |
|        |        |  Integer ( lo-hi ) height of the image in pixels.          |
|        |        |                                                            |
|   16   |     1  |  Image Pixel Size.                                         |
|        |        |  Number of bits in a stored pixel index.                   |
|        |        |                                                            |
|   17   |     1  |  Image Descriptor Byte.                                    |
|        |        |  Bits 3-0 - number of attribute bits associated with each  |
|        |        |             pixel.                                         |
|        |        |  Bit 4    - reserved.  Must be set to 0.                   |
|        |        |  Bit 5    - screen origin bit.                             |
|        |        |             0 = Origin in lower left-hand corner.          |
|        |        |             1 = Origin in upper left-hand corner.          |
|        |        |             Must be 0 for Truevision images.               |
|        |        |  Bits 7-6 - Data storage interleaving flag.                |
|        |        |             00 = non-interleaved.                          |
|        |        |             01 = two-way (even/odd) interleaving.          |
|        |        |             10 = four way interleaving.                    |
|        |        |             11 = reserved.                                 |
|        |        |  This entire byte should be set to 0.  Don't ask me.       |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|   18   | varies |  Image Identification Field.                               |
|        |        |  Contains a free-form identification field of the length   |
|        |        |  specified in byte 1 of the image record.  It's usually    |
|        |        |  omitted ( length in byte 1 = 0 ), but can be up to 255    |
|        |        |  characters.  If more identification information is        |
|        |        |  required, it can be stored after the image data.          |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
| varies | varies |  Color map data.                                           |
|        |        |                                                            |
|        |        |  The offset is determined by the size of the Image         |
|        |        |  Identification Field.  The length is determined by        |
|        |        |  the Color Map Specification, which describes the          |
|        |        |  size of each entry and the number of entries.             |
|        |        |  Each color map entry is 2, 3, or 4 bytes.                 |
|        |        |  Unused bits are assumed to specify attribute bits.        |
|        |        |                                                            |
|        |        |  The 4 byte entry contains 1 byte for blue, 1 byte         |
|        |        |  for green, 1 byte for red, and 1 byte of attribute        |
|        |        |  information, in that order.
|        |        |                                                            |
|        |        |  The 3 byte entry contains 1 byte each of blue, green,     |
|        |        |  and red.                                                  |
|        |        |                                                            |
|        |        |  The 2 byte entry is broken down as follows:               |
|        |        |  ARRRRRGG GGGBBBBB, where each letter represents a bit.    |
|        |        |  But, because of the lo-hi storage order, the first byte   |
|        |        |  coming from the file will actually be GGGBBBBB, and the   |
|        |        |  second will be ARRRRRGG. "A" represents an attribute bit. |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
| varies | varies |  Image Data Field.                                         |
|        |        |                                                            |
|        |        |  This field specifies (width) x (height) color map         |
|        |        |  indices.  Each index is stored as an integral number      |
|        |        |  of bytes (typically 1 or 2).   All fields are unsigned.   |
|        |        |  The low-order byte of a two-byte field is stored first.   |
|        |        |                                                            |
--------------------------------------------------------------------------------






--------------------------------------------------------------------------------
DATA TYPE 2:  Unmapped RGB images.                                             |
_______________________________________________________________________________|
| Offset | Length |                     Description                            |
|--------|--------|------------------------------------------------------------|
|--------|--------|------------------------------------------------------------|
|    0   |     1  |  Number of Characters in Identification Field.             |
|        |        |                                                            |
|        |        |  This field is a one-byte unsigned integer, specifying     |
|        |        |  the length of the Image Identification Field.  Its value  |
|        |        |  is 0 to 255.  A value of 0 means that no Image            |
|        |        |  Identification Field is included.                         |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    1   |     1  |  Color Map Type.                                           |
|        |        |                                                            |
|        |        |  This field contains either 0 or 1.  0 means no color map  |
|        |        |  is included.  1 means a color map is included, but since  |
|        |        |  this is an unmapped image it is usually ignored.  TIPS    |
|        |        |  ( a Targa paint system ) will set the border color        |
|        |        |  the first map color if it is present.                     |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    2   |     1  |  Image Type Code.                                          |
|        |        |                                                            |
|        |        |  This field will always contain a binary 2.                |
|        |        |  ( That's what makes it Data Type 2 ).                     |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    3   |     5  |  Color Map Specification.                                  |
|        |        |                                                            |
|        |        |  Ignored if Color Map Type is 0; otherwise, interpreted    |
|        |        |  as follows:                                               |
|        |        |                                                            |
|    3   |     2  |  Color Map Origin.                                         |
|        |        |  Integer ( lo-hi ) index of first color map entry.         |
|        |        |                                                            |
|    5   |     2  |  Color Map Length.                                         |
|        |        |  Integer ( lo-hi ) count of color map entries.             |
|        |        |                                                            |
|    7   |     1  |  Color Map Entry Size.                                     |
|        |        |  Number of bits in color map entry.  16 for the Targa 16,  |
|        |        |  24 for the Targa 24, 32 for the Targa 32.                 |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    8   |    10  |  Image Specification.                                      |
|        |        |                                                            |
|    8   |     2  |  X Origin of Image.                                        |
|        |        |  Integer ( lo-hi ) X coordinate of the lower left corner   |
|        |        |  of the image.                                             |
|        |        |                                                            |
|   10   |     2  |  Y Origin of Image.                                        |
|        |        |  Integer ( lo-hi ) Y coordinate of the lower left corner   |
|        |        |  of the image.                                             |
|        |        |                                                            |
|   12   |     2  |  Width of Image.                                           |
|        |        |  Integer ( lo-hi ) width of the image in pixels.           |
|        |        |                                                            |
|   14   |     2  |  Height of Image.                                          |
|        |        |  Integer ( lo-hi ) height of the image in pixels.          |
|        |        |                                                            |
|   16   |     1  |  Image Pixel Size.                                         |
|        |        |  Number of bits in a pixel.  This is 16 for Targa 16,      |
|        |        |  24 for Targa 24, and .... well, you get the idea.         |
|        |        |                                                            |
|   17   |     1  |  Image Descriptor Byte.                                    |
|        |        |  Bits 3-0 - number of attribute bits associated with each  |
|        |        |             pixel.  For the Targa 16, this would be 0 or   |
|        |        |             1.  For the Targa 24, it should be 0.  For     |
|        |        |             Targa 32, it should be 8.                      |
|        |        |  Bit 4    - reserved.  Must be set to 0.                   |
|        |        |  Bit 5    - screen origin bit.                             |
|        |        |             0 = Origin in lower left-hand corner.          |
|        |        |             1 = Origin in upper left-hand corner.          |
|        |        |             Must be 0 for Truevision images.               |
|        |        |  Bits 7-6 - Data storage interleaving flag.                |
|        |        |             00 = non-interleaved.                          |
|        |        |             01 = two-way (even/odd) interleaving.          |
|        |        |             10 = four way interleaving.                    |
|        |        |             11 = reserved.                                 |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|   18   | varies |  Image Identification Field.                               |
|        |        |  Contains a free-form identification field of the length   |
|        |        |  specified in byte 1 of the image record.  It's usually    |
|        |        |  omitted ( length in byte 1 = 0 ), but can be up to 255    |
|        |        |  characters.  If more identification information is        |
|        |        |  required, it can be stored after the image data.          |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
| varies | varies |  Color map data.                                           |
|        |        |                                                            |
|        |        |  If the Color Map Type is 0, this field doesn't exist.     |
|        |        |  Otherwise, just read past it to get to the image.         |
|        |        |  The Color Map Specification describes the size of each    |
|        |        |  entry, and the number of entries you'll have to skip.     |
|        |        |  Each color map entry is 2, 3, or 4 bytes.                 |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
| varies | varies |  Image Data Field.                                         |
|        |        |                                                            |
|        |        |  This field specifies (width) x (height) pixels.  Each     |
|        |        |  pixel specifies an RGB color value, which is stored as    |
|        |        |  an integral number of bytes.                              |
|        |        |                                                            |
|        |        |  The 2 byte entry is broken down as follows:               |
|        |        |  ARRRRRGG GGGBBBBB, where each letter represents a bit.    |
|        |        |  But, because of the lo-hi storage order, the first byte   |
|        |        |  coming from the file will actually be GGGBBBBB, and the   |
|        |        |  second will be ARRRRRGG. "A" represents an attribute bit. |
|        |        |                                                            |
|        |        |  The 3 byte entry contains 1 byte each of blue, green,     |
|        |        |  and red.                                                  |
|        |        |                                                            |
|        |        |  The 4 byte entry contains 1 byte each of blue, green,     |
|        |        |  red, and attribute.  For faster speed (because of the     |
|        |        |  hardware of the Targa board itself), Targa 24 images are  |
|        |        |  sometimes stored as Targa 32 images.                      |
|        |        |                                                            |
--------------------------------------------------------------------------------





--------------------------------------------------------------------------------
DATA TYPE 9:  Run Length Encoded, color-mapped images.                         |
_______________________________________________________________________________|
| Offset | Length |                     Description                            |
|--------|--------|------------------------------------------------------------|
|--------|--------|------------------------------------------------------------|
|    0   |     1  |  Number of Characters in Identification Field.             |
|        |        |                                                            |
|        |        |  This field is a one-byte unsigned integer, specifying     |
|        |        |  the length of the Image Identification Field.  Its value  |
|        |        |  is 0 to 255.  A value of 0 means that no Image            |
|        |        |  Identification Field is included.                         |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    1   |     1  |  Color Map Type.                                           |
|        |        |                                                            |
|        |        |  This field is always 1 for color-mapped images.           |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    2   |     1  |  Image Type Code.                                          |
|        |        |                                                            |
|        |        |  A binary 9 for this data type.                            |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    3   |     5  |  Color Map Specification.                                  |
|        |        |                                                            |
|    3   |     2  |  Color Map Origin.                                         |
|        |        |  Integer ( lo-hi ) index of first color map entry.         |
|        |        |                                                            |
|    5   |     2  |  Color Map Length.                                         |
|        |        |  Integer ( lo-hi ) count of color map entries.             |
|        |        |                                                            |
|    7   |     1  |  Color Map Entry Size.                                     |
|        |        |  Number of bits in each color map entry.  16 for the       |
|        |        |  Targa 16, 24 for the Targa 24, 32 for the Targa 32.       |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    8   |    10  |  Image Specification.                                      |
|        |        |                                                            |
|    8   |     2  |  X Origin of Image.                                        |
|        |        |  Integer ( lo-hi ) X coordinate of the lower left corner   |
|        |        |  of the image.                                             |
|        |        |                                                            |
|   10   |     2  |  Y Origin of Image.                                        |
|        |        |  Integer ( lo-hi ) Y coordinate of the lower left corner   |
|        |        |  of the image.                                             |
|        |        |                                                            |
|   12   |     2  |  Width of Image.                                           |
|        |        |  Integer ( lo-hi ) width of the image in pixels.           |
|        |        |                                                            |
|   14   |     2  |  Height of Image.                                          |
|        |        |  Integer ( lo-hi ) height of the image in pixels.          |
|        |        |                                                            |
|   16   |     1  |  Image Pixel Size.                                         |
|        |        |  Number of bits in a pixel.  This is 16 for Targa 16,      |
|        |        |  24 for Targa 24, and .... well, you get the idea.         |
|        |        |                                                            |
|   17   |     1  |  Image Descriptor Byte.                                    |
|        |        |  Bits 3-0 - number of attribute bits associated with each  |
|        |        |             pixel.  For the Targa 16, this would be 0 or   |
|        |        |             1.  For the Targa 24, it should be 0.  For the |
|        |        |             Targa 32, it should be 8.                      |
|        |        |  Bit 4    - reserved.  Must be set to 0.                   |
|        |        |  Bit 5    - screen origin bit.                             |
|        |        |             0 = Origin in lower left-hand corner.          |
|        |        |             1 = Origin in upper left-hand corner.          |
|        |        |             Must be 0 for Truevision images.               |
|        |        |  Bits 7-6 - Data storage interleaving flag.                |
|        |        |             00 = non-interleaved.                          |
|        |        |             01 = two-way (even/odd) interleaving.          |
|        |        |             10 = four way interleaving.                    |
|        |        |             11 = reserved.                                 |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|   18   | varies |  Image Identification Field.                               |
|        |        |  Contains a free-form identification field of the length   |
|        |        |  specified in byte 1 of the image record.  It's usually    |
|        |        |  omitted ( length in byte 1 = 0 ), but can be up to 255    |
|        |        |  characters.  If more identification information is        |
|        |        |  required, it can be stored after the image data.          |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
| varies | varies |  Color map data.                                           |
|        |        |                                                            |
|        |        |  The offset is determined by the size of the Image         |
|        |        |  Identification Field.  The length is determined by        |
|        |        |  the Color Map Specification, which describes the          |
|        |        |  size of each entry and the number of entries.             |
|        |        |  Each color map entry is 2, 3, or 4 bytes.                 |
|        |        |  Unused bits are assumed to specify attribute bits.        |
|        |        |                                                            |
|        |        |  The 4 byte entry contains 1 byte for blue, 1 byte         |
|        |        |  for green, 1 byte for red, and 1 byte of attribute        |
|        |        |  information, in that order.
|        |        |                                                            |
|        |        |  The 3 byte entry contains 1 byte each of blue, green,     |
|        |        |  and red.                                                  |
|        |        |                                                            |
|        |        |  The 2 byte entry is broken down as follows:               |
|        |        |  ARRRRRGG GGGBBBBB, where each letter represents a bit.    |
|        |        |  But, because of the lo-hi storage order, the first byte   |
|        |        |  coming from the file will actually be GGGBBBBB, and the   |
|        |        |  second will be ARRRRRGG. "A" represents an attribute bit. |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
| varies | varies |  Image Data Field.                                         |
|        |        |                                                            |
|        |        |  This field specifies (width) x (height) color map         |
|        |        |  indices.  The indices are stored in packets.  There       |
|        |        |  two types of packets:  Run-length packets, and Raw        |
|        |        |  packets.  Both types of packets consist of a 1-byte       |
|        |        |  header, identifying the type of event and specifying a   |
|        |        |  count, followed by a variable-length body.                |
|        |        |  The high-order bit of the header is "1" for the           |
|        |        |  run length event, and "0" for the raw event.            |
|        |        |                                                            |
|        |        |  For the run-length event, the header consists of:        |
|        |        |      __________________________________________________    |
|        |        |      | 1 bit |   7 bit repetition count minus 1.      |    |
|        |        |      |   ID  |   Since the maximum value of this      |    |
|        |        |      |       |   field is 127, the largest possible   |    |
|        |        |      |       |   run size would be 128.               |    |
|        |        |      |-------|----------------------------------------|    |
|        |        |      |   1   |  C     C     C     C     C     C    C  |    |
|        |        |      --------------------------------------------------    |
|        |        |                                                            |
|        |        |  For the raw event, the header consists of:               |
|        |        |      __________________________________________________    |
|        |        |      | 1 bit |   7 bit number of pixels minus 1.      |    |
|        |        |      |   ID  |   Since the maximum value of this      |    |
|        |        |      |       |   field is 127, there can never be     |    |
|        |        |      |       |   more than 128 pixels per event.     |    |
|        |        |      |-------|----------------------------------------|    |
|        |        |      |   0   |  N     N     N     N     N     N    N  |    |
|        |        |      --------------------------------------------------    |
|        |        |                                                            |
|        |        |  For the run length event, the header is followed by      |
|        |        |  a single color index, which is assumed to be repeated     |
|        |        |  the number of times specified in the header.  The RLE     |
|        |        |  event may cross scan lines ( begin on one line and end   |
|        |        |  on the next ).                                            |
|        |        |                                                            |
|        |        |  For the raw event, the header is followed by the number  |
|        |        |  of color indices specified in the header.  The raw        |
|        |        |  event may cross scan lines ( begin on one line and end   |
|        |        |  on the next ).                                            |
|        |        |                                                            |
--------------------------------------------------------------------------------





--------------------------------------------------------------------------------
DATA TYPE 10:  Run Length Encoded, RGB images.                                 |
_______________________________________________________________________________|
| Offset | Length |                     Description                            |
|--------|--------|------------------------------------------------------------|
|--------|--------|------------------------------------------------------------|
|    0   |     1  |  Number of Characters in Identification Field.             |
|        |        |                                                            |
|        |        |  This field is a one-byte unsigned integer, specifying     |
|        |        |  the length of the Image Identification Field.  Its range  |
|        |        |  is 0 to 255.  A value of 0 means that no Image            |
|        |        |  Identification Field is included.                         |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    1   |     1  |  Color Map Type.                                           |
|        |        |                                                            |
|        |        |  This field contains either 0 or 1.  0 means no color map  |
|        |        |  is included.  1 means a color map is included, but since  |
|        |        |  this is an unmapped image it is usually ignored.  TIPS    |
|        |        |  ( a Targa paint system ) will set the border color        |
|        |        |  the first map color if it is present.  Wowie zowie.       |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    2   |     1  |  Image Type Code.                                          |
|        |        |                                                            |
|        |        |  Binary 10 for this type of image.                         |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    3   |     5  |  Color Map Specification.                                  |
|        |        |                                                            |
|        |        |  Ignored if Color Map Type is 0; otherwise, interpreted    |
|        |        |  as follows:                                               |
|        |        |                                                            |
|    3   |     2  |  Color Map Origin.                                         |
|        |        |  Integer ( lo-hi ) index of first color map entry.         |
|        |        |                                                            |
|    5   |     2  |  Color Map Length.                                         |
|        |        |  Integer ( lo-hi ) count of color map entries.             |
|        |        |                                                            |
|    7   |     1  |  Color Map Entry Size.                                     |
|        |        |  Number of bits in color map entry.  This value is 16 for  |
|        |        |  the Targa 16, 24 for the Targa 24, 32 for the Targa 32.   |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|    8   |    10  |  Image Specification.                                      |
|        |        |                                                            |
|    8   |     2  |  X Origin of Image.                                        |
|        |        |  Integer ( lo-hi ) X coordinate of the lower left corner   |
|        |        |  of the image.                                             |
|        |        |                                                            |
|   10   |     2  |  Y Origin of Image.                                        |
|        |        |  Integer ( lo-hi ) Y coordinate of the lower left corner   |
|        |        |  of the image.                                             |
|        |        |                                                            |
|   12   |     2  |  Width of Image.                                           |
|        |        |  Integer ( lo-hi ) width of the image in pixels.           |
|        |        |                                                            |
|   14   |     2  |  Height of Image.                                          |
|        |        |  Integer ( lo-hi ) height of the image in pixels.          |
|        |        |                                                            |
|   16   |     1  |  Image Pixel Size.                                         |
|        |        |  Number of bits in a pixel.  This is 16 for Targa 16,      |
|        |        |  24 for Targa 24, and .... well, you get the idea.         |
|        |        |                                                            |
|   17   |     1  |  Image Descriptor Byte.                                    |
|        |        |  Bits 3-0 - number of attribute bits associated with each  |
|        |        |             pixel.  For the Targa 16, this would be 0 or   |
|        |        |             1.  For the Targa 24, it should be 0.  For the |
|        |        |             Targa 32, it should be 8.                      |
|        |        |  Bit 4    - reserved.  Must be set to 0.                   |
|        |        |  Bit 5    - screen origin bit.                             |
|        |        |             0 = Origin in lower left-hand corner.          |
|        |        |             1 = Origin in upper left-hand corner.          |
|        |        |             Must be 0 for Truevision images.               |
|        |        |  Bits 7-6 - Data storage interleaving flag.                |
|        |        |             00 = non-interleaved.                          |
|        |        |             01 = two-way (even/odd) interleaving.          |
|        |        |             10 = four way interleaving.                    |
|        |        |             11 = reserved.                                 |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
|   18   | varies |  Image Identification Field.                               |
|        |        |  Contains a free-form identification field of the length   |
|        |        |  specified in byte 1 of the image record.  It's usually    |
|        |        |  omitted ( length in byte 1 = 0 ), but can be up to 255    |
|        |        |  characters.  If more identification information is        |
|        |        |  required, it can be stored after the image data.          |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
| varies | varies |  Color map data.                                           |
|        |        |                                                            |
|        |        |  If the Color Map Type is 0, this field doesn't exist.     |
|        |        |  Otherwise, just read past it to get to the image.         |
|        |        |  The Color Map Specification, describes the size of each   |
|        |        |  entry, and the number of entries you'll have to skip.     |
|        |        |  Each color map entry is 2, 3, or 4 bytes.                 |
|        |        |                                                            |
|--------|--------|------------------------------------------------------------|
| varies | varies |  Image Data Field.                                         |
|        |        |                                                            |
|        |        |  This field specifies (width) x (height) pixels.  The      |
|        |        |  RGB color information for the pixels is stored in         |
|        |        |  packets.  There are two types of packets:  Run-length     |
|        |        |  encoded packets, and raw packets.  Both have a 1-byte     |
|        |        |  header, identifying the type of event and specifying a   |
|        |        |  count, followed by a variable-length body.                |
|        |        |  The high-order bit of the header is "1" for the           |
|        |        |  run length event, and "0" for the raw event.            |
|        |        |                                                            |
|        |        |  For the run-length event, the header consists of:        |
|        |        |      __________________________________________________    |
|        |        |      | 1 bit |   7 bit repetition count minus 1.      |    |
|        |        |      |   ID  |   Since the maximum value of this      |    |
|        |        |      |       |   field is 127, the largest possible   |    |
|        |        |      |       |   run size would be 128.               |    |
|        |        |      |-------|----------------------------------------|    |
|        |        |      |   1   |  C     C     C     C     C     C    C  |    |
|        |        |      --------------------------------------------------    |
|        |        |                                                            |
|        |        |  For the raw event, the header consists of:               |
|        |        |      __________________________________________________    |
|        |        |      | 1 bit |   7 bit number of pixels minus 1.      |    |
|        |        |      |   ID  |   Since the maximum value of this      |    |
|        |        |      |       |   field is 127, there can never be     |    |
|        |        |      |       |   more than 128 pixels per event.     |    |
|        |        |      |-------|----------------------------------------|    |
|        |        |      |   0   |  N     N     N     N     N     N    N  |    |
|        |        |      --------------------------------------------------    |
|        |        |                                                            |
|        |        |                                                            |
|        |        |  For the run length event, the header is followed by      |
|        |        |  a single color value, which is assumed to be repeated     |
|        |        |  the number of times specified in the header.  The         |
|        |        |  event may cross scan lines ( begin on one line and end   |
|        |        |  on the next ).                                            |
|        |        |                                                            |
|        |        |  For the raw event, the header is followed by             |
|        |        |  the number of color values specified in the header.       |
|        |        |                                                            |
|        |        |  The color entries themselves are two bytes, three bytes,  |
|        |        |  or four bytes ( for Targa 16, 24, and 32 ), and are       |
|        |        |  broken down as follows:                                   |
|        |        |                                                            |
|        |        |  The 2 byte entry -                                        |
|        |        |  ARRRRRGG GGGBBBBB, where each letter represents a bit.    |
|        |        |  But, because of the lo-hi storage order, the first byte   |
|        |        |  coming from the file will actually be GGGBBBBB, and the   |
|        |        |  second will be ARRRRRGG. "A" represents an attribute bit. |
|        |        |                                                            |
|        |        |  The 3 byte entry contains 1 byte each of blue, green,     |
|        |        |  and red.                                                  |
|        |        |                                                            |
|        |        |  The 4 byte entry contains 1 byte each of blue, green,     |
|        |        |  red, and attribute.  For faster speed (because of the     |
|        |        |  hardware of the Targa board itself), Targa 24 image are   |
|        |        |  sometimes stored as Targa 32 images.                      |
|        |        |                                                            |
--------------------------------------------------------------------------------
*/