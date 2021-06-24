#include "LauncherCore.h"
#include <WinBase.h>
#include <shared/Ini.h>
#include "Resource.h"
#include <vector>
#include <wininet.h>
#include "unzip.h"
LauncherCore::LauncherCore()
{
	s_pSocket = new CAPISocket();
}

InitReturn LauncherCore::Init(HWND hwndBase, HWND handleLabel)
{
	hwndCONNECTIONLABEL = handleLabel;
	char szPath[_MAX_PATH] = "";
	GetCurrentDirectory(_MAX_PATH, szPath);
	SetPath(szPath);
	s_IniPath = GetPath() + "Server.ini";
	const char* szIniPath = s_IniPath.c_str();
	char szRegistrationSite[_MAX_PATH];
	memset(szRegistrationSite, 0, sizeof(szRegistrationSite));

	GetPrivateProfileString("Join", "Registration site", "", szRegistrationSite, _MAX_PATH, szIniPath);
	ServerInfo.s_RegistrationSite = std::string(szRegistrationSite);


	ServerInfo.iLoginPort = GetPrivateProfileInt("Server", "PLOGIN", 0, szIniPath);
	ServerInfo.iServerCount = GetPrivateProfileInt("Server", "Count", 0, szIniPath);
	ServerInfo.iFiles = GetPrivateProfileInt("Version", "Files", 0, szIniPath);
	for (int i = 0; i < ServerInfo.iServerCount; i++)
	{
		char szKey[32] = "";
		sprintf_s(szKey, "IP%d", i);
		char szIp[32] = "";
		GetPrivateProfileString("Server", szKey, "", szIp, 32, szIniPath);
		ServerInfo.v_Servers.push_back(std::string(szIp));
	}
	int iServer = -1;
	if (ServerInfo.iServerCount > 0) iServer = rand() % ServerInfo.iServerCount;
	std::string selectedServer = ServerInfo.v_Servers.at(iServer);
	if (iServer >= 0 && lstrlen(selectedServer.c_str()))
	{
		int iErr = s_pSocket->Connect(hwndBase, selectedServer.c_str(), ServerInfo.iLoginPort);

		if (iErr)
		{
			return InitReturn::CONNECTION_FAILED;
		}
		else
		{
			int iOffset = 0;
			uint8_t byBuffs[4];
			CAPISocket::MP_AddByte(byBuffs, iOffset, PACKET_VERSION);
			s_pSocket->Send(byBuffs, iOffset);
			return InitReturn::CONNECTION_OK;
		}
	}
	else
	{
		return InitReturn::NO_SERVER;
	}
}

void LauncherCore::Tick()
{
	while (!s_pSocket->m_qRecvPkt.empty())
	{
		auto pkt = s_pSocket->m_qRecvPkt.front();
		if (!ProcessPacket(*pkt))
			break;

		delete pkt;
		s_pSocket->m_qRecvPkt.pop();

	}
}

bool LauncherCore::ProcessPacket(Packet pkt)
{
	int iCmd = pkt.read<uint8_t>();
	switch (iCmd)
	{
	case PACKET_VERSION:
	{
		int16_t versionInfo = pkt.read<int16_t>();	// Ä¿¸àµå ÆÄ½Ì..

		if (versionInfo > ServerInfo.iFiles)
		{
			int iOffset = 0;
			uint8_t byBuffs[4];
			CAPISocket::MP_AddByte(byBuffs, iOffset, PACKET_DOWNLOADINFO);
			s_pSocket->Send(byBuffs, iOffset);
		}
		else
		{
			SetWindowTextA(hwndCONNECTIONLABEL, "GAME IS UP TO DATE");
		}

		return true;
	}
	case PACKET_DOWNLOADINFO:
	{
		char buff[100];
		int16_t size = pkt.read<int16_t>();
		std::string ftpUrl;
		pkt.readString(ftpUrl, size);

		size = pkt.read<int16_t>();
		std::string folder;
		pkt.readString(folder, size);

		int16_t zipSize = pkt.read<int16_t>();

		std::vector<std::string> zips;
		for (int i = 0; i < zipSize; i++)
		{
			size = pkt.read<int16_t>();
			std::string temp;
			pkt.readString(temp, size);

			zips.push_back(temp);
		}

		HINTERNET handle = InternetOpenA(
			"Mozilla/4.0 (compatible)",
			INTERNET_OPEN_TYPE_DIRECT,
			NULL,
			NULL,
			0
		);

		if (!handle)
		{
			ShowLastError("InternetOpenA failed");
			return true;
		}



		HINTERNET session = InternetConnectA(
			handle,
			ftpUrl.c_str(),
			INTERNET_DEFAULT_FTP_PORT,
			"K",
			"KNServer2021.",
			INTERNET_SERVICE_FTP,
			NULL,
			NULL
		);

		if (!session)
		{
			ShowLastError("InternetConnectA failed ");
			InternetCloseHandle(handle);
			return true;
		}

		if (!FtpSetCurrentDirectory(session, folder.c_str()))
		{
			ShowLastError("FtpSetCurrentDirectory failed ");
			InternetCloseHandle(session);
			return true;
		}

		DWORD dwTransferType = FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD;

		for (int i = 0; i < zips.size(); i++)
		{
			std::string fileName = zips.at(i);
			if (!FtpGetFile(session, fileName.c_str(), fileName.c_str(), FALSE,
				FILE_ATTRIBUTE_NORMAL, dwTransferType, 0))
			{
				ShowLastError("FtpGetFile failed ");
				return true;
			}
			//
			HZIP hz = OpenZip(fileName.c_str(), 0);
			ZIPENTRY ze; GetZipItem(hz, -1, &ze); int numitems = ze.index;
			for (int i = 0; i < numitems; i++)
			{
				GetZipItem(hz, i, &ze);
				UnzipItem(hz, i, ze.name);
			}
			CloseZip(hz);

			std::string delimiter = ".";
			std::string token = fileName.substr(0, fileName.find(delimiter));
			delimiter = "patch";
			std::string target = token.substr(token.find_last_of(delimiter) + 1, token.size() - delimiter.size());
			bool temp  = WritePrivateProfileStringA(TEXT("Version"), TEXT("Files"), target.c_str(), GetPath().c_str());
			
			if (!DeleteFile(fileName.c_str()))
			{
				ShowLastError("DeleteFile failed ");
				return true;
			}
		}
	}
	return true;
	}

	return true;
}

void LauncherCore::ShowLastError(const char* err)
{
	char buff[255];
	snprintf(buff, sizeof(buff), "%s- Code:%d", err, GetLastError());
	SetWindowTextA(hwndCONNECTIONLABEL, buff);
}
