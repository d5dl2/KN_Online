#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;


int main()
{
	string fileName = "C:\\Users\\HMT\\source\\repos\\KN_Online\\Client\\data\\Data\\Texts_us.tbl.tmp";
	string targetFileName = "C:\\Users\\HMT\\source\\repos\\KN_Online\\Client\\data\\Data\\Texts_us_2.tbl";;

	HANDLE hFile = ::CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		cout << "Cant open file " << fileName;
		return -1;
	}

	DWORD dwSizeHigh = 0;
	DWORD dwSizeLow = ::GetFileSize(hFile, &dwSizeHigh);
	if (dwSizeLow <= 0)
	{
		CloseHandle(hFile);
		::remove(fileName.c_str()); // 임시 파일 지우기..
		return FALSE;
	}

	// 원래 파일을 읽고..
	uint8_t* pDatas = new uint8_t[dwSizeLow];
	DWORD dwRWC = 0;
	::ReadFile(hFile, pDatas, dwSizeLow, &dwRWC, NULL); // 암호화된 데이터 읽고..
	CloseHandle(hFile); // 원래 파일 닫고

	// 테이블 만드는 툴에서 쓰는 키와 같은 키..
	uint16_t key_r = 0x0816;
	uint16_t key_c1 = 0x6081;
	uint16_t key_c2 = 0x1608;


	for (uint32_t i = 0; i < dwSizeLow; i++)
	{
		uint8_t byData = (pDatas[i] ^ (key_r >> 8));
		key_r = (byData + key_r) * key_c1 + key_c2;
		pDatas[i] = byData;
	}

	

	// 임시 파일에 쓴다음.. 다시 연다..
	hFile = ::CreateFile(targetFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	::WriteFile(hFile, pDatas, dwSizeLow, &dwRWC, NULL); // 임시파일에 암호화 풀린 데이터 쓰기
	CloseHandle(hFile); // 임시 파일 닫기
	delete[] pDatas; pDatas = NULL;

	
	return 0;
}