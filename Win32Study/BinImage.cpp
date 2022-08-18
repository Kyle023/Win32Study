#include "stdafx.h"
#include "BinImage.h"
#include <fstream>


CBinImage::CBinImage():
inBin(nullptr)
,grayBin(nullptr)
, w(0)
, h(0)
{
}


CBinImage::~CBinImage()
{
	if (inBin)
	{
		delete[] inBin;
		inBin = nullptr;
	}
	if (grayBin)
	{
		delete[] grayBin;
		grayBin = nullptr;
	}
}

bool CBinImage::CreateGrayBin(/*inBin, outBin*/)
{
	grayBin = new unsigned short[w*h];
	unsigned short *p = grayBin;

	for (unsigned short j = 0; j < h / 2; j++)
	{
		for (unsigned short i = 0; i < w / 2; i++)
		{
			int Gr_pos = i * 2 + 1 + j*w * 2;
			int Gb_pos = i * 2 + j*w * 2 + w;
			unsigned short gray = ((inBin[Gb_pos] - inBin[Gr_pos]) / 2 + (inBin[Gr_pos])); //G
			//unsigned short gray = inBin[i * 2 + 1 + j*w*2]; //Gr
			//unsigned short gray = inBin[i * 2 + j*w*2+w]; //Gb
			//unsigned short gray = (inBin[i * 2 + j*w * 2 + w] - inBin[i * 2 + 1 + j*w * 2]) / 2 + (inBin[i * 2 + 1 + j*w * 2]); //G

			//unsigned short gray = (inBin[i * 2 + j*w * 2]); //R
			//unsigned short gray = (inBin[i * 2+1 + j*w * 2+w]); //B
			*p++ = gray;
			*p++ = gray;
		}
		memcpy_s(p, w*sizeof(unsigned short), p - w, w*sizeof(unsigned short));
		p += w;
	}
	return true;
}


bool CBinImage::ReadBin(std::string filepath/*filepath,outBin*/)
{
	std::ifstream f;
	f.open(filepath, std::ios::binary | std::ios::in);
	if (f.good())
	{
		//f.read((char*)&w, 2);
		f.read((char*)&w, 2);
		f.read((char*)&h, 2);
		inBin = new unsigned short[w*h];
		unsigned short *p = inBin;
		/*for (unsigned short i=0; i < h; i++)
		{
			f.read((char*)p, w*sizeof(unsigned short));
			p += w;
		}*/
		f.read((char*)p, w*h*sizeof(unsigned short));
		f.close();
		return true;
	}
	f.close();
	return false;
}
bool CBinImage::SaveBin(std::string filename /*filename,inBin*/)
{
	std::ofstream f;
	f.open(filename, std::ios::binary | std::ios::out | std::ios::trunc);
	if (f.good())
	{
		//f << (unsigned short)(w) << (unsigned short)(h);
		f.write((char*)&w, 2);
		f.write((char*)&h, 2);
		f.write((char*)grayBin, w*h*sizeof(unsigned short));
		f.close();
		return true;
	}
	f.close();
	return false;
}
bool CBinImage::SaveBmpFromBin(std::string filename/*filename, inBin*/)
{
	BITMAPFILEHEADER bh;
	BITMAPINFOHEADER binfo;

	// BITMAPFILEHEADER bh;//位图头文件

	bh.bfType = ((WORD)('M' << 8) | 'B');//"BM"说明文件类型
	bh.bfReserved1 = 0;//保留
	bh.bfReserved2 = 0;//保留	
	bh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + w*h / 4;//文件大小
	bh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;

	//表示从头文件开始到实际图像数据数据之间的字节的偏移量。bfOffBits可以直接定位像素数据
	//BITMAPINFOHEADER bInfo;//位图信息头

	binfo.biSize = sizeof(BITMAPINFOHEADER);//说明BITMAPINFOHEADER结构所需的字数
	binfo.biWidth = w/2;//图像宽度，以像素为单位
	binfo.biHeight = h/2;//图像高度，以像素为单位
	binfo.biPlanes = 1;//为目表设备说明位面数，其中总是被设为1
	binfo.biBitCount = 8;//说明比特数/像素的颜色深度，值为0，1，4，8，16，24或32，256灰度级的颜色深度为8，因为2^8=256
	binfo.biCompression = BI_RGB;//说明图像数据压缩类型
	binfo.biSizeImage = binfo.biWidth*binfo.biHeight;//说明图像的大小，一字节为单位，
	binfo.biXPelsPerMeter = 0;//水平分辨率，可以设为0
	binfo.biYPelsPerMeter = 0;//垂直分辨率，可以设为0
	binfo.biClrUsed = 256;//说明位图实际使用的彩色表中颜色索引数
	binfo.biClrImportant = 0;//说明对图像显示有重要影响的颜色索引数目，为0表示都重要

	std::ofstream f;
	f.open(filename, std::ios::trunc | std::ios::binary | std::ios::out);
	if (f.good())
	{
		f.write((char*)&bh, sizeof(BITMAPFILEHEADER));
		f.write((char*)&binfo, sizeof(BITMAPINFOHEADER));

		// 调色板
		RGBQUAD rgb = { 0 };
		for (int i = 0; i < 256; i++)
		{
			rgb.rgbBlue = rgb.rgbGreen = rgb.rgbRed = i;
			f.write((char*)&rgb, sizeof(RGBQUAD));
		}

		char data;
		for (int i = 0; i < h / 2; i++)
		{
			for (int j = 0; j < w / 2; j++)
			{
				data = char(grayBin[j * 2 + i*w * 2] >> 2);
				f.write(&data, 1);
			}
		}
		f.close();
		return true;
	}
	f.close();
	return false;
}