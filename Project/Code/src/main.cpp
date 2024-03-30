
#include "stdafx.h"
#include "strlcpy.h"
#include "SingleInstance.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// The command line parameters you use, e.g. -win -quickstart
// Note: This application also passes through other parameters
// which are issued to this executable
#define COMMANDLINE L" -win"

// Use shell execute. Create Process does not work properly on some clients.
//#define USE_SHELLEXECUTE 1

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CTokenExtractor
{
public:
	CTokenExtractor(const wchar_t* wszText)
	{
		const size_t wszTextLen = ::wcslen(wszText);
		m_wszTextCopy = new wchar_t[wszTextLen+1];
		::wcslcpy(m_wszTextCopy, wszText, wszTextLen+1);
		m_wszTextNextToken = m_wszTextCopy;
	}

	~CTokenExtractor()
	{
		delete m_wszTextCopy;
	}

	const wchar_t* GetNext(const wchar_t* wszToken)
	{
		const wchar_t* wszResult = ::wcstok(m_wszTextNextToken, wszToken);
		m_wszTextNextToken = NULL;
		return wszResult;
	}

private:
	wchar_t* m_wszTextCopy;
	wchar_t* m_wszTextNextToken;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CPathStringW
{
public:
	CPathStringW() {}
	CPathStringW(const wchar_t* wszPath)
	{
		Assign(wszPath);
	}

	void Assign(const wchar_t* wszPath)
	{
		m_path.assign(wszPath);
	}

	const wchar_t* GetCStr() const
	{
		return m_path.c_str();
	}

	bool ReplaceEnd(const wchar_t* wszSearch, const wchar_t* wszReplace)
	{
		const size_t searchLen = ::wcslen(wszSearch);
		if (m_path.size() > searchLen)
		{
			const size_t searchIndex = m_path.size()-searchLen;
			if (::wcscmp(&m_path[searchIndex], wszSearch) == 0)
			{
				m_path.erase(searchIndex);
				m_path.append(wszReplace);
				return true;
			}
		}
		return false;
	}

private:
	std::wstring m_path;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <size_t Size>
bool ReadAsciiFile(const wchar_t* wszFileName, wchar_t(&buffer)[Size])
{
	bool success = false;
	FILE* fp;
	if (::_wfopen_s(&fp, wszFileName, L"r") == 0)
	{
		char readBuffer[Size];
		::fseek(fp, 0, SEEK_END);
		const size_t fileSize = ::ftell(fp);
		::fseek(fp, 0, SEEK_SET);
		const size_t readSize = ::fread(readBuffer, sizeof(char), fileSize >= Size ? Size-1 : fileSize, fp);
		if (readSize != 0)
		{
			::mbstowcs(buffer, readBuffer, readSize);
			buffer[readSize] = 0;
			success = true;
		}
		::fclose(fp);
	}
	return success;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ReadAsciiFile(const wchar_t* wszFileName, std::wstring& buffer)
{
	bool success = false;
	FILE* fp;
	if (::_wfopen_s(&fp, wszFileName, L"r") == 0)
	{
		char readBuffer[4096];
		::fseek(fp, 0, SEEK_END);
		const size_t fileSize = (size_t)::ftell(fp);
		::fseek(fp, 0, SEEK_SET);

		buffer.resize(fileSize);
		size_t writeSize = 0;

		for (;;)
		{
			const size_t readSize = ::fread(readBuffer, sizeof(char), sizeof(readBuffer), fp);
			if (readSize == 0)
				break;
			::mbstowcs(&buffer[writeSize], readBuffer, readSize);
			writeSize += readSize;
		}
		success = writeSize > 0;
		::fclose(fp);
	}
	return success;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool FileExists(const wchar_t* fileName)
{
	return 0 == ::_waccess_s(fileName, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsAtleastWindowsVista()
{
	OSVERSIONINFO osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&osvi);
	return osvi.dwMajorVersion >= 6;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Welcome()
{
#if USE_SHELLEXECUTE
	::printf("Generals Proxy Launcher (Shell) v2.0 by xezon\n");
	::printf("---------------------------------------------\n\n");
#else
	::printf("Generals Proxy Launcher v2.0 by xezon\n");
	::printf("-------------------------------------\n\n");
#endif
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Shutdown()
{
	::printf("\n");
	::printf("Press any key to continue . . .\n");
	::FlushConsoleInputBuffer(::GetStdHandle(STD_INPUT_HANDLE));
	::getchar();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring FindGeneralsExe(const wchar_t* wszLauncherDir, const wchar_t* wszGameDir)
{
	std::wstring applicationExe;
	std::wstring txtContent;
	wchar_t wszTxt[MAX_PATH];
	::wcslcpy_t(wszTxt, wszLauncherDir);
	::wcslcat_t(wszTxt, L"\\launch.txt");

	if (ReadAsciiFile(wszTxt, txtContent))
	{
		CTokenExtractor tokenExtractor(txtContent.c_str());
		while (const wchar_t* wszFileName = tokenExtractor.GetNext(L"\n"))
		{
			applicationExe.assign(wszLauncherDir);
			applicationExe.append(L"\\");
			applicationExe.append(wszFileName);

			if (FileExists(applicationExe.c_str()))
			{
				break;
			}
			else
			{
				applicationExe.clear();
			}
		}
	}

	if (applicationExe.empty())
	{
		applicationExe.assign(wszGameDir);
		applicationExe.append(L"\\generals.exe");
	}

	return applicationExe;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool LaunchGeneralsExe(const wchar_t* wszLauncherDir, const wchar_t* wszGameDir, int argc, _TCHAR* argv[])
{
	const std::wstring applicationExe = FindGeneralsExe(wszLauncherDir, wszGameDir);

	wchar_t wszCommandline[32767];
	wszCommandline[0] = 0;

	{
		wchar_t wszTxt[MAX_PATH];
		::wcslcpy_t(wszTxt, wszLauncherDir);
		::wcslcat_t(wszTxt, L"\\commandline.txt");

		if (!ReadAsciiFile(wszTxt, wszCommandline))
		{
			::wcslcpy_t(wszCommandline, COMMANDLINE);
		}
	}

	for (int i = 1; i < argc; ++i)
	{
		::wcslcat_t(wszCommandline, L" ");
		::wcslcat_t(wszCommandline, argv[i]);
	}

	::printf("Launching %ls %ls\n", applicationExe.c_str(), wszCommandline);

#if USE_SHELLEXECUTE
	SHELLEXECUTEINFOW ShExecInfo = {0};
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS|SEE_MASK_NOASYNC;
    ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = L"open";
    ShExecInfo.lpFile = applicationExe.c_str();
    ShExecInfo.lpParameters = wszCommandline;
    ShExecInfo.lpDirectory = wszGameDir;
    ShExecInfo.nShow = SW_SHOWNORMAL;
    ShExecInfo.hInstApp = NULL;

	if (FALSE != ::ShellExecuteExW(&ShExecInfo))
	{
		::printf("Game launch successful\nDo not close this box. Please wait...\n");
		::WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	}
	else
	{
		char szMessage[256];
		DWORD error = ::GetLastError();
		::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, error, 0, szMessage, sizeof(szMessage), NULL);
		::printf("Game launch failed (Code:%u Error:%s)\n", error, szMessage);
		return false;
	}
#else
	STARTUPINFO startupInfo = {0};
    PROCESS_INFORMATION processInfo = {0};
    startupInfo.cb = sizeof(startupInfo);
	LPSECURITY_ATTRIBUTES lpProcessAttributes = NULL;
	LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
	BOOL bInheritHandles = FALSE;
	DWORD dwCreationFlags = 0;
	LPVOID lpEnvironment = NULL;

    if (0 != ::CreateProcessW(
		applicationExe.c_str(),
        wszCommandline,
        lpProcessAttributes,
		lpThreadAttributes,
		bInheritHandles,
		dwCreationFlags,
		lpEnvironment,
        wszGameDir,
        &startupInfo,
		&processInfo)
    )
    {
		::printf("Game launch successful.\nDo not close this box. Please wait . . .\n");
		::WaitForSingleObject(processInfo.hProcess, INFINITE);
		::CloseHandle(processInfo.hProcess);
		::CloseHandle(processInfo.hThread);
    }
	else
	{
		char szMessage[256];
		DWORD error = ::GetLastError();
		::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, error, 0, szMessage, sizeof(szMessage), NULL);
		::printf("Game launch failed (Code:%u Error:%s)\n", error, szMessage);
        return false;
	}
#endif

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MoveFilesW(const std::vector<std::wstring>& sourceFiles, const std::vector<std::wstring>& targetFiles)
{
	assert(sourceFiles.size() == targetFiles.size());

	for (size_t i = 0; i < sourceFiles.size(); ++i)
	{
		::printf("Rename %ls\n", sourceFiles[i].c_str());
		::printf("    To %ls\n", targetFiles[i].c_str());

#if 0
		// Delete file we want to rename too just in case it exists.
		::DeleteFileW(targetFiles[i].c_str());
#endif

		// Move file.
		const bool success = ::MoveFileW(sourceFiles[i].c_str(), targetFiles[i].c_str()) != FALSE;

		if (!success)
		{
			char szMessage[256];
			DWORD error = ::GetLastError();
			::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, error, 0, szMessage, sizeof(szMessage), NULL);
			::printf("Rename failed (Code:%u Error:%s)\n", error, szMessage);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool InitiateLaunch(int argc, _TCHAR* argv[])
{
	wchar_t wszLauncherDir[MAX_PATH];
	::GetModuleFileNameW(NULL, wszLauncherDir, MAX_PATH);
	::PathRemoveFileSpecW(wszLauncherDir);

	wchar_t wszGameDir[MAX_PATH];
	::wcslcpy_t(wszGameDir, wszLauncherDir);
	::PathRemoveFileSpecW(wszGameDir);

	wchar_t wszBigTxtFileName[MAX_PATH];
	::wcslcpy_t(wszBigTxtFileName, wszLauncherDir);
	::wcslcat_t(wszBigTxtFileName, L"\\big.txt");

	std::wstring bigTxtContent;
	const bool hasMods = ReadAsciiFile(wszBigTxtFileName, bigTxtContent);

	std::vector<std::wstring> originalModNames;
	std::vector<std::wstring> replacementModNames;
	originalModNames.reserve(32);
	replacementModNames.reserve(32);

	if (hasMods)
	{
		CTokenExtractor tokenExtractor(bigTxtContent.c_str());
		while (const wchar_t* wszOriginalFileName = tokenExtractor.GetNext(L"\n"))
		{
			originalModNames.push_back(std::wstring());
			replacementModNames.push_back(std::wstring());
			std::wstring& originalModName = originalModNames.back();
			std::wstring& replacementModName = replacementModNames.back();
			
			CPathStringW replacementName(wszOriginalFileName);
			replacementName.ReplaceEnd(L".big", L".big.bak");
			originalModName.assign(wszLauncherDir).append(L"\\").append(wszOriginalFileName);
			replacementModName.assign(wszLauncherDir).append(L"\\").append(replacementName.GetCStr());
		}

		MoveFilesW(replacementModNames, originalModNames);
	}

	const bool launchSuccess = LaunchGeneralsExe(wszLauncherDir, wszGameDir, argc, argv);

	if (hasMods)
	{
		MoveFilesW(originalModNames, replacementModNames);
	}

	return launchSuccess;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	bool launchSuccess = false;

	Welcome();

	CSingleInstance instance;
	instance.InitMutex("PKshHqJMLqJyKcK3YlWgpau9V1Y78Cx2H8f4nuFl");

	if (instance.IsAnotherInstanceRunning())
	{
		::printf("Another instance of this program is already running.");
	}
	else
	{
		launchSuccess = InitiateLaunch(argc, argv);
	}

	if (!launchSuccess)
	{
		Shutdown();
	}

	return launchSuccess ? 0 : 1;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
