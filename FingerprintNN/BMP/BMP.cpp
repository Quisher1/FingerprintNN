#include "BMP.h"


const ColorPalette ColorPalette::RED(255, 0, 0);
const ColorPalette ColorPalette::GREEN(0, 255, 0);
const ColorPalette ColorPalette::BLUE(0, 0, 255);
const ColorPalette ColorPalette::YELLOW(255, 255, 0);
const ColorPalette ColorPalette::PINK(255, 0, 255);
const ColorPalette ColorPalette::CYAN(0, 255, 255);
const ColorPalette ColorPalette::WHITE(255, 255, 255);
const ColorPalette ColorPalette::BLACK(0, 0, 0);



//const rgb rgb::RED(255, 0, 0);
//const rgb rgb::GREEN(0, 255, 0);
//const rgb rgb::BLUE(0, 0, 255);
//const rgb rgb::WHITE(255, 255, 255);
//const rgb rgb::BLACK(0, 0, 0);
//
//const rgb555 rgb555::RED(255, 0, 0);
//const rgb555 rgb555::GREEN(0, 255, 0);
//const rgb555 rgb555::BLUE(0, 0, 255);
//const rgb555 rgb555::WHITE(255, 255, 255);
//const rgb555 rgb555::BLACK(0, 0, 0);
//
//const rgb565 rgb565::RED(255, 0, 0);
//const rgb565 rgb565::GREEN(0, 255, 0);
//const rgb565 rgb565::BLUE(0, 0, 255);
//const rgb565 rgb565::WHITE(255, 255, 255);
//const rgb565 rgb565::BLACK(0, 0, 0);
//
//const rgba rgba::RED(255, 0, 0, 0);
//const rgba rgba::GREEN(0, 255, 0, 0);
//const rgba rgba::BLUE(0, 0, 255, 0);
//const rgba rgba::WHITE(255, 255, 255, 0);
//const rgba rgba::BLACK(0, 0, 0, 0);


void BitMapImage::open()
{
	std::string str(m_filename);
	str += ".bmp";
	str = "start " + str;
	system(str.c_str());
}


BMP::BMP()
{

}
BMP::BMP(const char* filename)
{
	m_filename = filename;

	load(m_filename);
}
BMP::BMP(const BMP& bmp)
{
	m_filename = bmp.m_filename;
	m_height = bmp.m_height;
	m_width = bmp.m_width;
	m_BitsPerPixel = bmp.m_BitsPerPixel;
	m_bitsPerMeterX = bmp.m_bitsPerMeterX;
	m_bitsPerMeterY = bmp.m_bitsPerMeterY;
	m_colorTableSize = bmp.m_colorTableSize;
	m_colorTableNumberOfElements = bmp.m_colorTableNumberOfElements;
	m_storageMethod = bmp.m_colorTableNumberOfElements;
	m_dataPosition = bmp.m_dataPosition;
	m_fileSize = bmp.m_fileSize;
	m_pixelDataSize = bmp.m_pixelDataSize;
	m_rowSize = bmp.m_rowSize;
	m_strideBytes = bmp.m_strideBytes;
	m_headerLenght = bmp.m_headerLenght;
	m_infoLenght = bmp.m_infoLenght;
	m_dataLenght = bmp.m_dataLenght;
	yAxisFlipped = bmp.yAxisFlipped;

	m_header = new BYTE[m_headerLenght];
	m_info = new BYTE[m_infoLenght];
	m_data = new BYTE[m_dataLenght];

	memcpy(m_header, bmp.m_header, m_headerLenght);
	memcpy(m_info, bmp.m_info, m_infoLenght);
	memcpy(m_data, bmp.m_data, m_dataLenght);
}


// BMP version - 3 
BMP::BMP(uint width, uint height, ColorPalette color, const char* filename, BMPFormat format)
{
	m_filename = filename;

	m_width = width;
	m_height = height;
	m_BitsPerPixel = format;

	m_bitsPerMeterX = 0; //
	m_bitsPerMeterY = 0; //

	m_rowSize = (((m_BitsPerPixel * m_width) + 31) & ~31) >> 3;
	m_strideBytes = m_rowSize - m_width * m_BitsPerPixel / 8;



	m_headerLenght = 14;
	m_header = new BYTE[m_headerLenght];
	m_infoLenght = 40;
	m_info = new BYTE[m_infoLenght];
	m_dataLenght = m_rowSize * height;
	m_data = new BYTE[m_dataLenght];


	memset(m_header, 0, m_headerLenght);
	memset(m_info, 0, m_infoLenght);
	memset(m_data, 0, m_dataLenght);

	m_fileSize = m_headerLenght + m_infoLenght + m_dataLenght;


	m_colorTableSize = 0;
	m_colorTableNumberOfElements = 0;
	m_storageMethod = 0;
	m_dataPosition = 54;
	m_pixelDataSize = m_rowSize * m_height;

	uint cpp = m_BitsPerPixel / 8;

	DWORD fillColor = 0;
	memcpy(&fillColor, &color.colorHex, cpp * sizeof(BYTE));
	if (m_BitsPerPixel == 16)
		if (is_rgb565)
			fillColor = rgb565(color.r, color.g, color.b).colorHex;
		else
			fillColor = rgb555(color.r, color.g, color.b).colorHex;

	for (int i = 0; i < m_height; ++i)
		for (int j = 0; j < m_width; ++j)
			memcpy(&m_data[(i * m_width + j) * cpp + 0 + m_strideBytes * i], &fillColor, cpp * sizeof(BYTE));


	VariablesToArray();
}


BMP::~BMP()
{
	if (m_header != nullptr)
	{
		delete[] m_header;
		delete[] m_info;
		delete[] m_data;
	}
}


BMP& BMP::operator=(const BMP& other)
{
	if (this == &other)
		return *this;

	m_filename = other.m_filename;
	m_height = other.m_height;
	m_width = other.m_width;
	m_BitsPerPixel = other.m_BitsPerPixel;
	m_bitsPerMeterX = other.m_bitsPerMeterX;
	m_bitsPerMeterY = other.m_bitsPerMeterY;
	m_colorTableSize = other.m_colorTableSize;
	m_colorTableNumberOfElements = other.m_colorTableNumberOfElements;
	m_storageMethod = other.m_colorTableNumberOfElements;
	m_dataPosition = other.m_dataPosition;
	m_fileSize = other.m_fileSize;
	m_pixelDataSize = other.m_pixelDataSize;
	m_rowSize = other.m_rowSize;
	m_strideBytes = other.m_strideBytes;
	m_headerLenght = other.m_headerLenght;
	m_infoLenght = other.m_infoLenght;
	m_dataLenght = other.m_dataLenght;
	yAxisFlipped = other.yAxisFlipped;

	if (m_header != nullptr)
	{
		delete[] m_header;
		delete[] m_info;
		delete[] m_data;
	}

	m_header = new BYTE[m_headerLenght];
	m_info = new BYTE[m_infoLenght];
	m_data = new BYTE[m_dataLenght];

	memcpy(m_header, other.m_header, m_headerLenght);
	memcpy(m_info, other.m_info, m_infoLenght);
	memcpy(m_data, other.m_data, m_dataLenght);

	return *this;
}




bool BMP::load(const char* filename)
{
	std::string name(filename);
	name += ".bmp";

	FILE* file = fopen(name.c_str(), "rb");
	if (!file)
		throw std::runtime_error("file doesn't exists");

	if (m_header != nullptr && m_info != nullptr && m_data != nullptr)
	{
		delete[] m_header;
		delete[] m_info;
		delete[] m_data;
	}

	fseek(file, 0L, SEEK_END);
	long long end_pos = ftell(file);
	fseek(file, 0L, SEEK_SET);


	m_header = new BYTE[m_headerLenght];
	fread(m_header, sizeof(BYTE), m_headerLenght, file);


	if (m_header[0] != 'B' && m_header[1] != 'M') {
		fclose(file);
		throw std::runtime_error("this is not a bmp file format");
	}


	DWORD infolLenght;
	long long curpos = ftell(file);
	fread(&infolLenght, sizeof(DWORD), 1, file);
	fseek(file, curpos, SEEK_SET);
	m_infoLenght = infolLenght;



	// 32-bit info field - Version 3

	// if(m_infoLenght >= 40)

	m_info = new BYTE[m_infoLenght];
	fread(m_info, sizeof(BYTE), m_infoLenght, file);


	m_fileSize = *(DWORD*)(&m_header[0x02]);
	m_dataPosition = *(DWORD*)(&m_header[0x0A]);


	m_width = abs(*(LONG*)(&m_info[0x04]));
	m_height = abs(*(LONG*)(&m_info[0x08]));
	m_BitsPerPixel = *(WORD*)(&m_info[0x0E]);
	m_storageMethod = *(WORD*)(&m_info[0x10]);
	m_pixelDataSize = *(DWORD*)(&m_info[0x14]);
	m_bitsPerMeterX = *(LONG*)(&m_info[0x18]);
	m_bitsPerMeterY = *(LONG*)(&m_info[0x1C]);
	m_colorTableSize = *(DWORD*)(&m_info[0x20]);
	m_colorTableNumberOfElements = *(DWORD*)(&m_info[0x24]);


	if (*(WORD*)(&m_info[0x0C]) != 1)
		std::cout << "file is using for cursors!?" << std::endl;
	if (m_BitsPerPixel != 24 && m_BitsPerPixel != 32)
		std::cout << "Warning: " << ("this class doesn't support BMP where bits per pixel equals: " + std::to_string(m_BitsPerPixel)) << std::endl;
	//if (m_BitsPerPixel == 24)
		//m_rowPadding = abs(((m_width * 3 + 3) & (~3)) - m_width * 3);
		//m_rowPadding = (3 * m_width + 3) & (-4);



	// 32-bit info field - Version 4
	// doesn't supported yet
	// TODO 32-bit info field Ver4

	// if(m_infoLenght >= 40)

	// 32-bit info field - Version 5
	// doesn't supported yet
	// TODO 32-bit info field Ver5


	fseek(file, m_dataPosition, SEEK_SET);

	//uint rowSize_floor = floor(((m_BitsPerPixel * m_width + 31) / 32.f) * 4);
	//uint rowSize_ceil = ceil(((m_BitsPerPixel * m_width) / 32.f) * 4);

	m_rowSize = (((m_BitsPerPixel * m_width) + 31) & ~31) >> 3;
	m_strideBytes = m_rowSize - m_width * m_BitsPerPixel / 8;
	//uint rowSize = 4 * int(((m_BitsPerPixel * m_width) + 31) / 32);

	m_dataLenght = m_rowSize * m_height;
	m_data = new BYTE[m_dataLenght];
	fread(m_data, sizeof(BYTE), m_dataLenght, file);

	fclose(file);
	return true;
}

// rewrite origin file to the one stored in this class
bool BMP::save()
{
	std::string name(m_filename);
	name += ".bmp";

	remove(name.c_str());

	FILE* file = fopen(name.c_str(), "wb");

	if (!file)
		throw std::runtime_error("problem to open file " + name);

	fwrite(m_header, sizeof(BYTE), m_headerLenght, file);
	fwrite(m_info, sizeof(BYTE), m_infoLenght, file);
	fwrite(m_data, sizeof(BYTE), m_dataLenght, file);

	fclose(file);
	return true;
}

// create new file and save data to it
bool BMP::saveAs(const char* newfilename)
{
	std::string name(newfilename);
	name += ".bmp";
	FILE* file = fopen(name.c_str(), "wb");

	if (!file)
		throw std::runtime_error("problem to create new file or rewrite file " + name);

	fwrite(m_header, sizeof(BYTE), m_headerLenght, file);
	fwrite(m_info, sizeof(BYTE), m_infoLenght, file);
	fwrite(m_data, sizeof(BYTE), m_dataLenght, file);

	fclose(file);
	return true;
}


void BMP::flipYAxis()
{
	yAxisFlipped = !yAxisFlipped;
}

DWORD BMP::getPixel(uint x, uint y)
{
	if (x < m_width && y < m_height)
	{
		if (yAxisFlipped)
			y = m_height - y - 1;

		uint cpp = m_BitsPerPixel / 8;

		DWORD out = 0;
		if (m_BitsPerPixel == 8)
		{
			/*static rgb565 color(m_data[(y * m_width + x) * cpp + 0 + m_strideBytes * y],
				m_data[(y * m_width + x) * cpp + 1 + m_strideBytes * y]);
			return color;*/
		}
		else if (m_BitsPerPixel == 16)
		{
			memcpy(&out, &m_data[(y * m_width + x) * cpp + 0 + m_strideBytes * y], 2 * sizeof(BYTE));
		}
		else if (m_BitsPerPixel == 24)
		{
			memcpy(&out, &m_data[(y * m_width + x) * cpp + 0 + m_strideBytes * y], 3 * sizeof(BYTE));
		}
		else if (m_BitsPerPixel == 32)
		{
			memcpy(&out, &m_data[(y * m_width + x) * cpp + 0 + m_strideBytes * y], 4 * sizeof(BYTE));
		}
		return out;
	}
	throw std::runtime_error("inndex out of range");
}

void BMP::setPixel(uint x, uint y, BYTE r, BYTE g, BYTE b, BYTE a)
{
	if (x < m_width && y < m_height)
	{
		if (yAxisFlipped)
			y = m_height - y - 1;

		uint cpp = m_BitsPerPixel / 8;
		DWORD color = 0;
		if (m_BitsPerPixel == 16)
			if (is_rgb565)
				color = rgb565(r, g, b).colorHex;
			else
				color = rgb555(r, g, b).colorHex;
		else
			color = rgba(r, g, b, a).colorHex;

		memcpy(&m_data[(y * m_width + x) * cpp + 0 + m_strideBytes * y], &color, cpp * sizeof(BYTE));
		return;
	}
	throw std::runtime_error("inndex out of range");
}



void BMP::infolog() const
{
	std::cout << "File Name: \"" << m_filename << ".bmp\"" << std::endl;

	std::cout << "Image Width: " << m_width << "\tHeight: " << m_height << std::endl;

	std::cout << "File Size: " << m_fileSize << std::endl;


	std::cout << "Bits per pixel: " << m_BitsPerPixel << std::endl;

	std::cout << "Header structure lenght: " << m_headerLenght << std::endl;
	std::cout << "Info structure lenght: " << m_infoLenght << std::endl;
	std::cout << "Data structure lenght: " << m_dataLenght << std::endl;

	std::cout << "Row size with stride: " << m_rowSize << "\tstride bytes count: " << m_strideBytes << std::endl;
}


//TODO: array to variables
//TODO: variables to array
void BMP::ArrayToVariables()
{
	// BMP Version - 3

	m_fileSize = *(DWORD*)(&m_header[0x02]);
	m_dataPosition = *(DWORD*)(&m_header[0x0A]);

	m_width = abs(*(LONG*)(&m_info[0x04]));
	m_height = abs(*(LONG*)(&m_info[0x08]));
	m_BitsPerPixel = *(WORD*)(&m_info[0x0E]);
	m_storageMethod = *(WORD*)(&m_info[0x10]);
	m_pixelDataSize = *(DWORD*)(&m_info[0x14]);
	m_bitsPerMeterX = *(LONG*)(&m_info[0x18]);
	m_bitsPerMeterY = *(LONG*)(&m_info[0x1C]);
	m_colorTableSize = *(DWORD*)(&m_info[0x20]);
	m_colorTableNumberOfElements = *(DWORD*)(&m_info[0x24]);
}
void BMP::VariablesToArray()
{
	// BMP Version - 3

	memset(&m_header[0x00], 'B', sizeof(BYTE));
	memset(&m_header[0x01], 'M', sizeof(BYTE));

	memcpy(&m_header[0x02], &m_fileSize, sizeof(DWORD));
	memcpy(&m_header[0x0A], &m_dataPosition, sizeof(DWORD));

	memcpy(&m_info[0x00], &m_infoLenght, sizeof(LONG));
	memcpy(&m_info[0x04], &m_width, sizeof(LONG));
	memcpy(&m_info[0x08], &m_height, sizeof(LONG));
	memcpy(&m_info[0x0E], &m_BitsPerPixel, sizeof(WORD));
	memcpy(&m_info[0x10], &m_storageMethod, sizeof(WORD));
	memcpy(&m_info[0x14], &m_pixelDataSize, sizeof(DWORD));
	memcpy(&m_info[0x18], &m_bitsPerMeterX, sizeof(LONG));
	memcpy(&m_info[0x1C], &m_bitsPerMeterY, sizeof(LONG));
	memcpy(&m_info[0x20], &m_colorTableSize, sizeof(DWORD));
	memcpy(&m_info[0x24], &m_colorTableNumberOfElements, sizeof(DWORD));

	memset(&m_info[0x0C], 1, sizeof(BYTE));
}


