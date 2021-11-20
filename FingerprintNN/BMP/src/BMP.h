#pragma once
#include <iostream>
#include <string>

typedef unsigned int uint;

typedef unsigned char BYTE;		// 8 bits
typedef unsigned short WORD;	// 16 bits
typedef unsigned int DWORD;		// 32 bits
typedef int LONG;				// 32 bits


struct ColorPalette
{
	ColorPalette(BYTE r = 0, BYTE g = 0, BYTE b = 0)
		: r(r), g(g), b(b)
	{
		colorHex = (r << 16) | (g << 8) | (b);
	}
	virtual ~ColorPalette() = default;

	DWORD colorHex = 0;
	BYTE r = 0, g = 0, b = 0;
	
	static const ColorPalette RED;
	static const ColorPalette GREEN;
	static const ColorPalette BLUE;
	static const ColorPalette YELLOW;
	static const ColorPalette PINK;
	static const ColorPalette CYAN;
	static const ColorPalette WHITE;
	static const ColorPalette BLACK;
	static const ColorPalette GRAY;
};

// TODO: constructor(Hex hex);

struct rgb555 : public ColorPalette
{
	rgb555(DWORD HEX, BYTE bytesPerPixel)
	{
		if (bytesPerPixel == 16)
		{
			r = (HEX >> 10) & 0b00011111;
			g = (HEX >> 5) & 0b00011111;
			b = HEX & 0b00011111;
			this->colorHex = HEX;
		}
		else if (bytesPerPixel == 24 || bytesPerPixel == 32)
		{
			r = (HEX >> 16) & 0xFF;
			g = (HEX >> 8) & 0xFF;
			b = HEX & 0xFF;
			this->colorHex = (this->r << 10) | (this->g << 5) | this->b;
		}
	}
	rgb555(BYTE c1, BYTE c2)
	{
		/*this->r = (c1 & 0b11111100) >> 3;
		this->g = ((c1 & 0b00000111) << 3) | ((c2 & 0b11100000) >> 5);
		this->b = (c2 & 0b00011111);
		this->colorHex = c1 * 256 + c2;*/

		this->r = (c1 & 0b01111100) >> 2;
		this->g = ((c1 & 0b00000011) << 3) | ((c2 & 0b11100000) >> 5);
		this->b = (c2 & 0b00011111);
		this->colorHex = c1 * 256 + c2;
	}
	rgb555(BYTE r, BYTE g, BYTE b)
	{
		this->r = r >> 3;
		this->g = g >> 3;
		this->b = b >> 3;

		this->colorHex = (this->r << 10) | (this->g << 5) | this->b;
		//this->colorHex = (this->r << 10) | (this->g << 5) | this->b;
	}


	/*friend std::ostream& operator<<(std::ostream& os, const rgb565& color)
	{
		std::cout << std::hex << num << std::dec << std::endl;
		return os;
	}*/
};

struct rgb565 : public ColorPalette
{
	rgb565(DWORD HEX, BYTE bytesPerPixel)
	{
		if (bytesPerPixel == 16)
		{
			r = (HEX >> 11) & 0b00011111;
			g = (HEX >> 5) & 0b00111111;
			b = HEX & 0b00011111;
			this->colorHex = HEX;
		}
		else if (bytesPerPixel == 24 || bytesPerPixel == 32)
		{
			r = (HEX >> 16) & 0xFF;
			g = (HEX >> 8) & 0xFF;
			b = HEX & 0xFF;
			this->colorHex = ((r & 0b11110000) << 7) | ((g & 0b11111100) << 3) | (b >> 3);
		}
	}
	rgb565(BYTE c1, BYTE c2)
	{
		this->r = (c1 & 0b11111000) >> 3;
		this->g = ((c1 & 0b00000111) << 3) | ((c2 & 0b11100000) >> 5);
		this->b = (c2 & 0b00011111);
		this->colorHex = c1 * 256 + c2;
	}
	rgb565(BYTE r, BYTE g, BYTE b)
	{
		this->r = (r * 249 + 1014) >> 11;
		this->g = (g * 253 + 505) >> 10;
		this->b = (b * 249 + 1014) >> 11;
		//this->colorHex = ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
		this->colorHex = ((r & 0b11110000) << 7) | ((g & 0b11111100) << 3) | (b >> 3);
	}

	/*friend std::ostream& operator<<(std::ostream& os, const rgb565& color)
	{
		std::cout << std::hex << num << std::dec << std::endl;
		return os;
	}*/
};

struct rgb : public ColorPalette
{
	rgb(DWORD HEX, BYTE bytesPerPixel)
	{
		if (bytesPerPixel == 16)
		{
			r = ((HEX >> 11) & 0b00011111) << 3;
			g = ((HEX >> 5) & 0b00011111) << 3;
			b = (HEX & 0b00011111) << 3;
			this->colorHex = (r << 16) | (g << 8) | b;
		}
		else if (bytesPerPixel == 24 || bytesPerPixel == 32)
		{
			r = (HEX >> 16) & 0xFF;
			g = (HEX >> 8) & 0xFF;
			b = HEX & 0xFF;
			this->colorHex = (r << 16) | (g << 8) | b;
		}
	}
	rgb(BYTE r = 0, BYTE g = 0, BYTE b = 0)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->colorHex = (r << 16) | (g << 8) | b;
	}

	friend std::ostream& operator<<(std::ostream& os, const rgb& color)
	{
		os << (int)color.r << " " << (int)color.g << " " << (int)color.b;
		return os;
	}
};

struct rgba : public ColorPalette
{
	rgba(DWORD HEX, BYTE bytesPerPixel)
	{
		if (bytesPerPixel == 16)
		{
			a = 0;
			r = ((HEX >> 11) & 0b00011111) << 3;
			g = ((HEX >> 5) & 0b00011111) << 3;
			b = (HEX & 0b00011111) << 3;
			this->colorHex = (r << 16) | (g << 8) | b;
		}
		else if (bytesPerPixel == 24)
		{
			a = 0;
			r = HEX >> 16;
			g = (HEX >> 8) & 0xFF;
			b = HEX & 0xFF;
			this->colorHex = (r << 16) | (g << 8) | b;
		}
		else if (bytesPerPixel == 32)
		{
			a = (HEX >> 24) & 0xFF;
			r = (HEX >> 16) & 0xFF;
			g = (HEX >> 8) & 0xFF;
			b = HEX & 0xFF;
			this->colorHex = (r << 16) | (g << 8) | b;
		}
	}
	rgba(BYTE r = 0, BYTE g = 0, BYTE b = 0, BYTE a = 0)
		: a(a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->colorHex = (a << 24) | (r << 16) | (g << 8) | b;
	}
	BYTE a = 0;

	friend std::ostream& operator<<(std::ostream& os, const rgba& color)
	{
		os << (int)color.r << " " << (int)color.g << " " << (int)color.b << " " << (int)color.a;
		return os;
	}
};

//struct rgb
//{
//	rgb(BYTE r = 0, BYTE g = 0, BYTE b = 0)
//		: r(r), g(g), b(b)
//	{}
//	BYTE r = 0, g = 0, b = 0;
//};
//struct rgba
//{
//	rgba(BYTE r = 0, BYTE g = 0, BYTE b = 0, BYTE a = 0)
//		: r(r), g(g), b(b), a(a)
//	{}
//	BYTE r = 0, g = 0, b = 0, a = 0;
//
//	friend std::ostream& operator<<(std::ostream& os, const rgba& color)
//	{
//		os << (int)color.r << " " << (int)color.g << " " << (int)color.b << " " << (int)color.a;
//		return os;
//	}
//
//	static const rgba RED;
//	static const rgba GREEN;
//	static const rgba BLUE;
//	static const rgba WHITE;
//	static const rgba BLACK;
//};

enum BMPFormat
{
	BMP8 = 9,
	BMP16 = 16,
	BMP24 = 24,
	BMP32 = 32
};


class BitMapImage
{
public:
	BitMapImage() {}
	virtual bool load(const char* filename) = 0;
	virtual bool save() = 0;
	virtual bool saveAs(const char* newfilename) = 0;

	virtual ~BitMapImage() {}


	virtual void flipYAxis() = 0;

	void open();

	uint width() const { return m_width; }
	uint height() const { return m_height; }
	BYTE bitsPerPixel() const { return m_BitsPerPixel; }
	const char* filename() const { return m_filename; }

	virtual void infolog() const = 0;


protected:
	const char* m_filename = nullptr;

	LONG m_height = 0, m_width = 0;
	LONG m_bitsPerMeterX = 0, m_bitsPerMeterY = 0;

	WORD m_BitsPerPixel = 0;

	DWORD m_colorTableSize = 0;
	DWORD m_colorTableNumberOfElements = 0;
	DWORD m_storageMethod = 0;
	DWORD m_dataPosition = 0;
	DWORD m_fileSize = 0;
	DWORD m_pixelDataSize = 0;


	uint m_headerLenght = 14;
	BYTE* m_header = nullptr;
	uint m_infoLenght = 0;
	BYTE* m_info = nullptr;
	uint m_dataLenght = 0;
	BYTE* m_data = nullptr;


	uint m_rowSize = 0;
	uint m_strideBytes = 0;


	bool yAxisFlipped = false;
};


// BMP16 & BMP24 & BMP32 without color Table
class BMP : public BitMapImage
{
public:
	BMP();
	BMP(const char* filename);
	BMP(const BMP& bmp);
	BMP(uint width, uint height, ColorPalette fillColor, const char* filename, BMPFormat format);

	~BMP() override;

	BMP& operator=(const BMP& other);

	void flipYAxis() override;
	// for 16-bit BMP
	void setRGB565(bool value = false) { is_rgb565 = value; }

	DWORD getPixel(uint x, uint y);
	void setPixel(uint x, uint y, BYTE r = 0, BYTE g = 0, BYTE b = 0, BYTE a = 0);


	bool load(const char* filename) override;
	bool save() override;
	bool saveAs(const char* newfilename) override;


	void infolog() const override;

private:

	void ArrayToVariables();
	void VariablesToArray();

	bool is_rgb565 = false;
};


// BMP8
class BMP8 : public BitMapImage
{
public:
	BMP8();
	BMP8(const char* filename);
	BMP8(const BMP8& bmp);
	BMP8(uint width, uint height, rgb fillcolor, const char* filename);

	~BMP8() override;

	BMP8& operator=(const BMP8& other);

	DWORD getPixel(uint x, uint y);
	void setPixel(uint x, uint y, BYTE r = 0, BYTE g = 0, BYTE b = 0, BYTE a = 0);

	bool load(const char* filename) override;
	bool save() override;
	bool saveAs(const char* newfilename) override;

	void infolog() const override;

private:


};




//  image
//	#####################################
//	# BITMAPFILEHEADER | BITMAPFILEINFO #
//  # BITMAPFILEDATA                    #
//  #                                   #
//  #                                   #
//	#####################################