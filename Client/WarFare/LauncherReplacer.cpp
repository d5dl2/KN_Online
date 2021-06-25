#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "LauncherReplacer.h"
#include <stdlib.h>
#include <LogWriter.h>

int fileExists(TCHAR* file)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile(file, &FindFileData);
	int found = handle != INVALID_HANDLE_VALUE;
	if (found)
	{
		FindClose(handle);
	}
	return found;
}

void ReplaceLauncherIfUpdated()
{
	CLogWriter::Open("Log.txt");
	char directory[_MAX_PATH] = "";
	GetCurrentDirectory(_MAX_PATH, directory);
	char launcher2[_MAX_PATH] = "";
	lstrcpy(launcher2, directory);
	lstrcat(launcher2, "\\Launcher2.exe");

	if (fileExists(launcher2))
	{
		char launcher[_MAX_PATH] = "";
		lstrcpy(launcher, directory);
		lstrcat(launcher, "\\Launcher.exe");
		DeleteFileA(launcher);

		int err = rename(launcher2, launcher);
		if (err == 0) // success 
		{
			CLogWriter::Write("Launcher updated !");
			return;
		}

		std::string errStr = "Couldn't replace launcher. Code: " + err;
		CLogWriter::Write(errStr.c_str());

	}


	return;
}