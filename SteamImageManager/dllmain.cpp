// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"

#include "SteamManager.h"
#include "Tools.h"
#include "ShareMemoryClient.h"


#include <atlbase.h>

HANDLE g_hThread = { 0 };
DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
	);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		g_hThread = ::CreateThread(NULL, NULL, ThreadProc, NULL, NULL, NULL);

	}
	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;
	case DLL_PROCESS_DETACH:
	{
		if (g_hThread)
		{
			CloseHandle(g_hThread);
		}
		break;
	}
	}
	return TRUE;
}

DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
{
	//1. �ж�steamclient.dll�Ƿ���أ�ȷ���Ƿ��stream
	ULONG steamclient_Base = 0;
	while (TRUE)
	{
		steamclient_Base = Tools::GetCurrentProcessModuleBase(L"steamclient.dll");
		if (steamclient_Base != 0)
		{
			Tools::message("HXL: steam����򿪣�\n");
			break;
		}
		Tools::message("HXL: �ȴ�steam���档\n");
		Sleep(1000);
	}

	//2. �ȴ��û���ɵ�¼
	CRegKey creg;
	while (TRUE)
	{
		Tools::message("HXL: �ȴ���¼steam!\n");

		HKEY hOpen = { 0 };
		if (ERROR_SUCCESS == creg.Open(HKEY_CURRENT_USER, L"Software\\Valve\\Steam\\ActiveProcess"))
		{
			DWORD account = { 0 };
			creg.QueryDWORDValue(L"ActiveUser", account);
			if (account > 0)
			{
				Tools::message("HXL: ��¼��ɣ��ȴ����������\n");
				break;
			}
		}

		Sleep(1000);
	}

	if (!SteamManager::GetInstance()->RestoreGameSet())
	{
		Tools::message("HXL: �޸���Ϸ����ʧ�ܣ� \n");
		return false;
	}


	if (!ShareMemoryClient::GetInstance()->Initialize())
	{
		Tools::message("HXL: ��ʼ�������ڴ�ͻ���ʧ�ܣ� \n");
		return false;
	}

	if (!SteamManager::GetInstance()->Initialize())
	{
		Tools::message("HXL: ��ʼ��Steam��ͼ��ز���ʧ�ܣ� \n");
		return false;
	}

	return 1;
}

