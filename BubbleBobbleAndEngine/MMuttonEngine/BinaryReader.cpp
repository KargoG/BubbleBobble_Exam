#include "pch.h"
#include "BinaryReader.h"

bool BinaryReader::m_IsOpen{ false };
std::ifstream BinaryReader::m_Ifstream{ };

void BinaryReader::OpenReader( const std::string &fileName )
{
	if (m_IsOpen)
	{
		throw std::exception("Reader is already Open. Please Close the reader before Opening!" );
	}

	m_Ifstream = std::ifstream(fileName, std::ios::out | std::ios::binary);
	m_IsOpen = true;
}

void BinaryReader::CloseReader()
{
	if (!m_IsOpen)
	{
		throw std::exception("Reader is already Closed. Please Open the reader before Closing!" );
	}
	
	m_Ifstream.close();
	m_IsOpen = false;
}

void BinaryReader::ReadStringFromFile( std::string &str )
{
	if (!m_IsOpen)
	{
		throw std::exception("Reader is Closed. Please Open the reader before reading!" );
	}

	size_t strSize{ };
	m_Ifstream.read((char*)&strSize, sizeof(size_t));

	char* buff = new char[strSize] {};
	
	m_Ifstream.read(buff, strSize);
	str.append(buff, strSize);
	delete[] buff;
}
