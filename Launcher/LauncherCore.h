#pragma once

#include <string>
#include <vector>
#include <shared/APISocket.h>


#define PACKET_VERSION 0x01
#define PACKET_DOWNLOADINFO 0x02
#define PACKET_NEWS 0xF6

struct ServerIni
{
	std::string s_RegistrationSite;
	std::vector<std::string> v_Servers;
	int iServerCount;
	int iLoginPort;
	int iFiles;
};

enum InitReturn { CONNECTION_OK, CONNECTION_FAILED, NO_SERVER };
enum FTPDownloadReturn { CANNOT_CONNECT_INTERNET, CANNOT_CONNECT_FTP, CANNOT_SET_DIRECTORY, CANNOT_DOWNLOAD_FILE, NO_NEW_VERSION, NEW_VERSIONS_DOWNLOADED, CANNOT_DELETE_ZIP };

class LauncherCore
{
private: 
	std::string 				s_IniPath;
	HWND hwndCONNECTIONLABEL;
public:
	void SetPath(char* path) {
		s_IniPath = path;
		if (s_IniPath.size() <= 0) return;

		// NOTE: this puts the entire string into lowercase characters
		CharLower(&(s_IniPath[0])); // make sure to give lowercase
		if (s_IniPath.size() > 1)
		{
			// NOTE: this checks if the last character is '\'; if not it will add it
			if (s_IniPath[s_IniPath.size() - 1] != '\\') s_IniPath += '\\';
		}
	}
	std::string GetPath() { return s_IniPath; }

	ServerIni					ServerInfo;
	InitReturn Init(HWND hwndBase, HWND hwndCONNECTIONLABEL);
	void Tick();
	bool ProcessPacket(Packet pkt);
	bool OpenInternetConnection();
	void ShowLastError(const char* err);
	LauncherCore();
	CAPISocket* s_pSocket;
};