#pragma once
#include <string>

class CBinImage
{
public:
	CBinImage();
	~CBinImage();
	// process bin, use G channel to create a gray Bin, the same size as raw image, but fill 4 channel with G
	bool CreateGrayBin(/*inBin, outBin*/);
	bool ReadBin(std::string filepath/*filepath,outBin*/);
	bool SaveBin(std::string filename /*filename,inBin*/);
	bool SaveBmpFromBin(std::string filename/*filename, inBin*/);

private:
	unsigned short *inBin;
	unsigned short *grayBin;
	unsigned short w, h;
};

