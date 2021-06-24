#include "LauncherCore.h"
#include <WinBase.h>
#include <shared/Ini.h>
#include "Resource.h"

LauncherCore::LauncherCore()
{
	s_pSocket = new CAPISocket();
}

void LauncherCore::Init(HWND hwndBase)
{
	char szPath[_MAX_PATH] = "";
	GetCurrentDirectory(_MAX_PATH, szPath);
	SetPath(szPath);

	// NOTE: we are anticipating an Options file to exist within this directory
	char szIniPath[_MAX_PATH] = "";
	lstrcpy(szIniPath, GetPath().c_str());
	lstrcat(szIniPath, "Server.ini");

	CIni ini(szIniPath);

	char szRegistrationSite[_MAX_PATH];
	memset(szRegistrationSite, 0, sizeof(szRegistrationSite));

	GetPrivateProfileString("Join", "Registration site", "", szRegistrationSite, _MAX_PATH, szIniPath);
	ServerInfo.s_RegistrationSite = std::string(szRegistrationSite);


	ServerInfo.iLoginPort = GetPrivateProfileInt("Server", "PLOGIN", 0, szIniPath);
	ServerInfo.iServerCount = GetPrivateProfileInt("Server", "Count", 0, szIniPath);

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
			hBrushConnectionLabel = hBrushERR;
			return;
		}
		else
		{
			hBrushConnectionLabel = hBrushOK;
			//// 게임 서버 리스트 요청..
			//int iOffset = 0;
			//uint8_t byBuffs[4];
			//CAPISocket::MP_AddByte(byBuffs, iOffset, N3_GAMESERVER_GROUP_LIST);
			//s_pSocket->Send(byBuffs, iOffset);
		}
	}
	else
	{
		//this->MessageBoxPost("No server list", "LogIn Server fail", MB_OK, BEHAVIOR_EXIT); // 끝낸다.
	}
}
