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
		::remove(fileName.c_str()); // �ӽ� ���� �����..
		return FALSE;
	}

	// ���� ������ �а�..
	uint8_t* pDatas = new uint8_t[dwSizeLow];
	DWORD dwRWC = 0;
	::ReadFile(hFile, pDatas, dwSizeLow, &dwRWC, NULL); // ��ȣȭ�� ������ �а�..
	CloseHandle(hFile); // ���� ���� �ݰ�

	// ���̺� ����� ������ ���� Ű�� ���� Ű..
	uint16_t key_r = 0x0816;
	uint16_t key_c1 = 0x6081;
	uint16_t key_c2 = 0x1608;


	for (uint32_t i = 0; i < dwSizeLow; i++)
	{
		uint8_t byData = (pDatas[i] ^ (key_r >> 8));
		key_r = (byData + key_r) * key_c1 + key_c2;
		pDatas[i] = byData;
	}

	

	// �ӽ� ���Ͽ� ������.. �ٽ� ����..
	hFile = ::CreateFile(targetFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	::WriteFile(hFile, pDatas, dwSizeLow, &dwRWC, NULL); // �ӽ����Ͽ� ��ȣȭ Ǯ�� ������ ����
	CloseHandle(hFile); // �ӽ� ���� �ݱ�
	delete[] pDatas; pDatas = NULL;

	
	return 0;
}