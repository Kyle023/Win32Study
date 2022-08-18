// Win32Study.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include "BinImage.h"

int test_sscanf()
{
	char szInput[] = "1234443";
	int regData = 0;
	_stscanf_s(szInput, "%d", &regData);
	printf("%d\n", regData);
	system("pause");
	return 0;
}

int test_sscanf1()
{
	//char szInput[10] = { 0 };
	//BYTE regData[10] = { 0 };

	char szInput[] = "123456789022";
	BYTE regData[8] = { 0 };

	//scanf("%s", szInput);
	//printf("%s\n", szInput);
	_stscanf_s(szInput, "%x%x", regData);
	//printf("%x\n", regData[0]);
	printf("%x\n", regData[0]);

	system("pause");
	return 0;
}

int test_binimage()
{
	CBinImage bin;
	bin.ReadBin("D:\\xxx.bin");
	bin.CreateGrayBin();
	bin.SaveBin("D:\\xxxx.bin");
	bin.SaveBmpFromBin("D:\\xxxx.bmp");
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//return test_sscanf();
	test_binimage();
}

