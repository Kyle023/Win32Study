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

	// BITMAPFILEHEADER bh;//λͼͷ�ļ�

	bh.bfType = ((WORD)('M' << 8) | 'B');//"BM"˵���ļ�����
	bh.bfReserved1 = 0;//����
	bh.bfReserved2 = 0;//����	
	bh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + w*h / 4;//�ļ���С
	bh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;

	//��ʾ��ͷ�ļ���ʼ��ʵ��ͼ����������֮����ֽڵ�ƫ������bfOffBits����ֱ�Ӷ�λ��������
	//BITMAPINFOHEADER bInfo;//λͼ��Ϣͷ

	binfo.biSize = sizeof(BITMAPINFOHEADER);//˵��BITMAPINFOHEADER�ṹ���������
	binfo.biWidth = w/2;//ͼ���ȣ�������Ϊ��λ
	binfo.biHeight = h/2;//ͼ��߶ȣ�������Ϊ��λ
	binfo.biPlanes = 1;//ΪĿ���豸˵��λ�������������Ǳ���Ϊ1
	binfo.biBitCount = 8;//˵��������/���ص���ɫ��ȣ�ֵΪ0��1��4��8��16��24��32��256�Ҷȼ�����ɫ���Ϊ8����Ϊ2^8=256
	binfo.biCompression = BI_RGB;//˵��ͼ������ѹ������
	binfo.biSizeImage = binfo.biWidth*binfo.biHeight;//˵��ͼ��Ĵ�С��һ�ֽ�Ϊ��λ��
	binfo.biXPelsPerMeter = 0;//ˮƽ�ֱ��ʣ�������Ϊ0
	binfo.biYPelsPerMeter = 0;//��ֱ�ֱ��ʣ�������Ϊ0
	binfo.biClrUsed = 256;//˵��λͼʵ��ʹ�õĲ�ɫ������ɫ������
	binfo.biClrImportant = 0;//˵����ͼ����ʾ����ҪӰ�����ɫ������Ŀ��Ϊ0��ʾ����Ҫ

	std::ofstream f;
	f.open(filename, std::ios::trunc | std::ios::binary | std::ios::out);
	if (f.good())
	{
		f.write((char*)&bh, sizeof(BITMAPFILEHEADER));
		f.write((char*)&binfo, sizeof(BITMAPINFOHEADER));

		// ��ɫ��
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