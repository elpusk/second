#pragma once

#include <windows.h>
#include <Userenv.h>

class Cns_WarpHandle
{
public:
	Cns_WarpHandle() : m_hHandle(NULL)
	{

	}

	explicit Cns_WarpHandle(HANDLE hHandle) : m_hHandle(hHandle)
	{
		
	}
	~Cns_WarpHandle()
	{
		if( m_hHandle )
			::CloseHandle( m_hHandle );

	}

	HANDLE GetHandle() const
	{
		return m_hHandle;
	}
	void SetHandle(HANDLE hHandle)
	{
		m_hHandle = hHandle;
	}

private:
	HANDLE m_hHandle;
private://don't call these method
	
	Cns_WarpHandle( const Cns_WarpHandle &);
	Cns_WarpHandle & operator=(const Cns_WarpHandle &);

};

class Cns_WarpEvtLogHandle{
public:
	Cns_WarpEvtLogHandle() : m_hHandle(NULL)
	{
	}

	explicit Cns_WarpEvtLogHandle(HANDLE hHandle) : m_hHandle(hHandle)
	{
	}
	~Cns_WarpEvtLogHandle()
	{
		if( m_hHandle )
			::DeregisterEventSource( m_hHandle );
	}

	HANDLE GetHandle() const
	{
		return m_hHandle;
	}

	void SetHandle(HANDLE hHandle)
	{
		m_hHandle = hHandle;
	}

private:
	HANDLE m_hHandle;
private://don't call these method
	Cns_WarpEvtLogHandle( const Cns_WarpEvtLogHandle &);
	Cns_WarpEvtLogHandle & operator=(const Cns_WarpEvtLogHandle &);
};

class Cns_WarpRegKeyHandle
{
public:
	explicit Cns_WarpRegKeyHandle( const HKEY & hKey )
	{
		::memcpy( &m_hKey, &hKey, sizeof(m_hKey) );
	}

	~Cns_WarpRegKeyHandle()
	{
		::RegCloseKey( m_hKey );
	}

	HKEY & GetHandle()
	{
		return m_hKey;
	}


private:
	HKEY m_hKey;
private://don't call these method
	Cns_WarpRegKeyHandle();
	Cns_WarpRegKeyHandle( const Cns_WarpRegKeyHandle& );
	Cns_WarpRegKeyHandle & operator=(const Cns_WarpRegKeyHandle&);
};

class Cns_WarpEnvBlock
{
public:
	explicit Cns_WarpEnvBlock( LPVOID lpEnv )
	{
		m_lpEnv= lpEnv;
	}

	~Cns_WarpEnvBlock()
	{
		if( m_lpEnv )
			::DestroyEnvironmentBlock( m_lpEnv );
	}

	LPVOID GetBlock()
	{
		return m_lpEnv;
	}


private:
	LPVOID m_lpEnv;
private://don't call these method
	Cns_WarpEnvBlock();
	Cns_WarpEnvBlock( const Cns_WarpEnvBlock& );
	Cns_WarpEnvBlock & operator=(const Cns_WarpEnvBlock&);
};

#include <winsvc.h>

class Cns_WarpServiceHandle
{
public:
	explicit Cns_WarpServiceHandle( SC_HANDLE hHandle )
	{
		m_hHandle = hHandle;
	}
	~Cns_WarpServiceHandle()
	{
		if( m_hHandle )
			::CloseServiceHandle( m_hHandle );
	}

	SC_HANDLE GetHandle()
	{
		return m_hHandle;
	}

private:
	SC_HANDLE m_hHandle;
private://don't call these method
	Cns_WarpServiceHandle();
	Cns_WarpServiceHandle( const Cns_WarpServiceHandle &);
	Cns_WarpServiceHandle & operator=(const Cns_WarpServiceHandle &);

};

#include <WtsApi32.h>

class Cns_WarpWTSMem
{
public:
	explicit Cns_WarpWTSMem( PWTS_SESSION_INFO pSessionInfo )
	{
		m_pSessionInfo= pSessionInfo;
	}

	~Cns_WarpWTSMem()
	{
		if( m_pSessionInfo )
			::WTSFreeMemory( m_pSessionInfo );
	}

	PWTS_SESSION_INFO GetSessionInfo()
	{
		return m_pSessionInfo;
	}


private:
	PWTS_SESSION_INFO m_pSessionInfo;
private://don't call these method
	Cns_WarpWTSMem();
	Cns_WarpWTSMem( const Cns_WarpWTSMem& );
	Cns_WarpWTSMem & operator=(const Cns_WarpWTSMem&);
};
