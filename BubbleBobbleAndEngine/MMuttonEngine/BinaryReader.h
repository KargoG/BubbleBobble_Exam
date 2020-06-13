#pragma once
#include <fstream>
#include <iostream>

class BinaryReader
{
public:
	static void OpenReader(const std::string& fileName);
	static void CloseReader();

	template<typename T>
	static void ReadVarFromFile(T& var);
	
	static void ReadStringFromFile(std::string& str);

private:
	static bool m_IsOpen;
	static std::ifstream m_Ifstream;
};

template<typename T>
void BinaryReader::ReadVarFromFile(T& var)
{
	if (!m_IsOpen)
	{
		throw std::exception("Reading failed. Open Reader before reading!" );
	}
	if (!__is_pod(T))
	{
		throw std::exception("Reading failed. Method can only read PODs!");
	}

	m_Ifstream.read((char*)&var, sizeof(T));
}
