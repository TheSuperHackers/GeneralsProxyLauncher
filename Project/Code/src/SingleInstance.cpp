
#include "stdafx.h"
#include "SingleInstance.h"

//////////////////////////////////////////////////////////////////////////////////////

CSingleInstance::CSingleInstance()
: m_dwLastError(0)
, m_hMutex(NULL)
{}

//////////////////////////////////////////////////////////////////////////////////////

CSingleInstance::~CSingleInstance() 
{
	ClearMutex();
}

//////////////////////////////////////////////////////////////////////////////////////

void CSingleInstance::InitMutex(const char* strMutexName)
{
	m_hMutex = ::CreateMutexA(NULL, TRUE, strMutexName);
	m_dwLastError = ::GetLastError();
}

//////////////////////////////////////////////////////////////////////////////////////

void CSingleInstance::ClearMutex()
{
	if (m_hMutex)
	{
		if (!IsAnotherInstanceRunning()) {
			::ReleaseMutex(m_hMutex);
		}
		::CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
	m_dwLastError = 0;
}

//////////////////////////////////////////////////////////////////////////////////////

bool CSingleInstance::IsAnotherInstanceRunning() 
{
	return m_dwLastError==ERROR_ALREADY_EXISTS
		|| m_dwLastError==ERROR_ACCESS_DENIED;
}

//////////////////////////////////////////////////////////////////////////////////////

bool CSingleInstanceCheck::IsAnotherInstanceRunning(const char* strMutexName)
{
	m_instance.InitMutex(strMutexName);
	bool isRunning = m_instance.IsAnotherInstanceRunning();
	m_instance.ClearMutex();
	return isRunning;
}

//////////////////////////////////////////////////////////////////////////////////////