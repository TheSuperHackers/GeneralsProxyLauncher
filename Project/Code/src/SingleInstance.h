
#ifndef __SINGLE_INSTANCE_H__
#define __SINGLE_INSTANCE_H__

#pragma once

//////////////////////////////////////////////////////////////////////////////////////

class CSingleInstance
{
public:
	CSingleInstance();
	~CSingleInstance();
	void InitMutex(const char* strMutexName);
	void ClearMutex();
	bool IsAnotherInstanceRunning();

protected:
	DWORD  m_dwLastError;
	HANDLE m_hMutex;
};

//////////////////////////////////////////////////////////////////////////////////////

class CSingleInstanceCheck
{
public:
	bool IsAnotherInstanceRunning(const char* strMutexName);
private:
	CSingleInstance m_instance;
};

//////////////////////////////////////////////////////////////////////////////////////

#endif // __SINGLE_INSTANCE_H__
