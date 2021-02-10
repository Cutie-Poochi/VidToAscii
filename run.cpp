#include <iostream>
#include <fstream>
#include <sstream>
#include "Magick++.h"

std::string getValue(const char* path, const char* key)
{
	std::ifstream file(path);
	file.seekg(std::ios::beg);
	std::string inputWord;
	while (file >> inputWord)
	{
		if (inputWord == key) {
			file >> inputWord; 
			file.close();
			return inputWord;
		}
	}
	throw std::runtime_error("Fix the file paths");
	return "Error";
}

inline char convertNumToChar(char num) //if you want a different sequence of characters then you have to change this function.
{
	// num is in the range [0:availableCharacters - 1]
	// so 0 + 33 is the ascii of space, that's how the current function works.
	return static_cast<char>(num + 32);
}

int main()
{
	const int availableCharacters = 94;
	const char* detailsFile = "README.txt";
	const std::string inputFile = getValue(detailsFile, "InputFile:");
	const std::string characterMap = getValue(detailsFile, "CharacterImages:");
	const std::string outputFile = getValue(detailsFile, "OutputFile:");
	const std::string fps = getValue(detailsFile, "Fps:");
	const int charactersInXAxis = std::stoi(getValue(detailsFile, "CharacterWidth:"));
	const int charactersInYAxis = std::stoi(getValue(detailsFile, "CharacterHeight:"));
	const int totalNumberOfCharacters = charactersInXAxis*charactersInYAxis;

	Magick::Image characters;
	try { characters.read(characterMap); characters.type(Magick::GrayscaleType); }
	catch (const Magick::ErrorBlob &error)
	{ std::cout << "Received error:" << error.what() << "\n" << "Make sure the path to character images is alright." << "\n"; return -1; }
	const int characterWidth = static_cast<int>(characters.columns() / availableCharacters);
	const int characterHeight = static_cast<int>(characters.rows());
	const int totalPixelsPerCharacter = characterWidth*characterHeight;
	unsigned short characterPixels[totalPixelsPerCharacter*availableCharacters];
	for (int characterIndex = 0; characterIndex < availableCharacters; ++characterIndex)
	{
		Magick::Image currentCharacter(characters);
		currentCharacter.crop(Magick::Geometry(characterWidth, characterHeight, characterWidth*characterIndex));
		for (int y = 0; y < characterHeight; ++y)
			for (int x = 0; x < characterWidth; ++x)
				characterPixels[characterIndex*characterWidth+y*characterHeight+x] = static_cast<unsigned short>(currentCharacter.pixelColor(x, y).quantumRed());
	}
	std::stringstream command;
	command << "mkdir images || true && ffmpeg -i " << inputFile << " -vf \"hue=s=0, scale=" << characterWidth*charactersInXAxis <<  ':' <<
	characterHeight*charactersInYAxis << "\" -r " << fps << " -q:v 2 -f image2 images/frame%d.jpg && ls images | wc -l > " << outputFile;
	std::cout << '\n' << std::system(command.str().c_str()) << '\n';
	std::fstream outputFileStream(outputFile.c_str());
	if (!outputFileStream) { std::cout << "Couldn't open output file, make sure the path is alright. \n"; return -1; }
	unsigned int numberOfFrames;
	outputFileStream >> numberOfFrames;
	for (int frameNumber = 1; frameNumber <= numberOfFrames; ++frameNumber)
	{
		std::stringstream result;
		Magick::Image currentImage;
		const std::string filename = "images/frame"+std::to_string(frameNumber)+".jpg";
		currentImage.read(filename);
		for (int i_y = 0; i_y < charactersInYAxis; ++i_y)
		{
			for (int i_x = 0; i_x < charactersInXAxis; ++i_x)
			{
				const Magick::Geometry Portion (characterWidth, characterHeight, i_x*characterWidth, i_y*characterHeight);
				Magick::Image croppedPortion(currentImage);
				croppedPortion.crop(Portion);
				int difference = INT32_MAX;
				char closest_char;
				unsigned short pixels[totalPixelsPerCharacter];
				for (int y = 0; y < characterHeight; ++y)
					for (int x = 0; x < characterWidth; ++x)
						pixels[y*characterWidth+x] = croppedPortion.pixelColor(x, y).quantumRed();
				for (int character = 0; character < availableCharacters; ++character)
				{
					int current_difference = 0;
					for (int pixelIndex = 0; pixelIndex < characterWidth*characterHeight; ++pixelIndex)
						current_difference += std::abs(int(characterPixels[character*totalPixelsPerCharacter+pixelIndex]) - pixels[pixelIndex]);
					if (current_difference < difference)
					{
						difference = current_difference;
						closest_char = character;
					}
				}
				result << convertNumToChar(closest_char);
			}
			result << '\n';
		}
		outputFileStream << result.str();
	}
	outputFileStream.close();
	std::cout << std::system("rm -rf images") << '\n';
	return 0;
}
