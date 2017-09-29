#pragma once
#include <string>
#include <fstream>
#include <iostream>

namespace mme {

	// this function uses an input file stream object and reads its buffer into a string stream object that is converted to the string that is returned
	static std::string loadShader(const char *filePath)
	{
		std::ifstream in(filePath, std::ios::in);
		std::string str = "";

		if (in.is_open()) {
			//std::cout << filePath << " loaded" << std::endl;
			std::string line = "";
			while (!in.eof()) {
				std::getline(in, line);
				str.append(line + "\n");
			}
		}
		else {
			//std::cout << filePath << " failed to load" << std::endl;
		}

		//std::cout << str;
		in.close();
		return str;
	}

}