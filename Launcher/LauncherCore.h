#pragma once

#include <string>
#include <vector>
#include <shared/APISocket.h>

struct ServerIni
{
	std::string s_RegistrationSite;
	std::vector<std::string> v_Servers;
	int iServerCount;
	int iLoginPort;
};


class LauncherCore
{
private: 
	std::string 				s_IniPath;

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
	void Init(HWND hwndBase);
	LauncherCore();
private:
	CAPISocket* s_pSocket;
};