// Inject.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Inject.h"
#include <Tlhelp32.h>

DWORD gPID = 0;

VOID OutInfo(WCHAR *formatstring, ...)
{
	WCHAR buff[MAX_PATH] = { 0 };
	WCHAR buffA[MAX_PATH] = { 0 };
	va_list arglist;
	va_start(arglist, formatstring);
	_vsnwprintf_s(buff, MAX_PATH, _TRUNCATE, formatstring, arglist);//ע��ڶ�������������sizeof
	va_end(arglist);
	swprintf_s(buffA, MAX_PATH, L"YY-%s\n", buff);//ע��ڶ�������������sizeof
	OutputDebugStringW(buffA);
}

DWORD GetProcessID(WCHAR* ProcessName)
{
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);//���̿���
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		if (Process32First(hSnapshot, &pe))
		{
			while (Process32Next(hSnapshot, &pe))
			{
				if (wcscmp(ProcessName, pe.szExeFile) == 0)
				{
					CloseHandle(hSnapshot);
					return pe.th32ProcessID;
				}
			}
		}
		CloseHandle(hSnapshot);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL GetSelfPath(WCHAR* OutBuffer)//��ȡ��������·�������������������û��\����
{
	size_t len;
	size_t i;

	if (GetModuleFileNameW(
		NULL,//��������ΪNULL�����Լ�����
		OutBuffer,//ָ��һ���ִ������������ڴ�����·��
		MAX_PATH//ǰ��ָ��������������Ĵ�С
		)//��ȡһ��ģ�������·��������ģ������ʧ�ܷ���0
		== 0)
	{
		return FALSE;
	}

	len = wcslen(OutBuffer);
	for (i = len; i >= 0; i -- )
	{
		if (OutBuffer[i] == '\\')
		{
			OutBuffer[i+1] = 0;
			return TRUE;
		}
	}
	return FALSE;
}
BOOL RemoteThreadInject(
	DWORD PID,
	WCHAR* DLLPath
	)
{
	BOOL bRet = FALSE;
	HANDLE hProcess = NULL;
	HMODULE hMoude = NULL;
	HANDLE hThread = NULL;
	PVOID pDllName = NULL;
	size_t dwStrLen = 0;

	//�򿪽���
	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,PID);
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		OutInfo(L"��Զ�̽���ʧ��");
		bRet = FALSE;
		goto Exit;
	}
	//��Զ�̽���������ռ�
	pDllName = VirtualAllocEx(
		hProcess, //Զ�̽��̾��
		NULL,		//���鿪ʼ��ַ
		0x1000,		//����ռ��С
		MEM_COMMIT,	//�ռ��ʼ��ȫ0
		PAGE_EXECUTE_READWRITE
		);	//�ռ�Ȩ��
	if (pDllName == NULL)
	{
		OutInfo(L"��Զ�̽����������ڴ�ʧ��");
		bRet = FALSE;
		goto Exit;
	}
	dwStrLen = wcslen(DLLPath) * 2 + 2;
	//дDLL·��
	if (WriteProcessMemory(hProcess, pDllName, DLLPath, dwStrLen, NULL) == 0)
	{
		OutInfo(L"��Զ�̽�����д��DLL·��ʧ��");
		bRet = FALSE;
		goto Exit;
	}
	//��ȡLoadLibraryW��ַ
	hMoude = GetModuleHandleW(L"Kernel32.dll");
	LPTHREAD_START_ROUTINE pfnLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(hMoude, "LoadLibraryW");
	if (pfnLoadLibrary == NULL)
	{
		OutInfo(L"��ȡLoadLibraryW��ַʧ��");
		bRet = FALSE;
		goto Exit;
	}
	//����Զ���߳�
	hThread = CreateRemoteThread(hProcess, NULL, 0, pfnLoadLibrary, pDllName, 0, NULL);
	if (hThread == NULL)
	{
		OutInfo(L"����Զ���߳�ʧ��");
		//�ͷ�Զ�̿ռ�
		VirtualFreeEx(hProcess, pDllName, dwStrLen, MEM_DECOMMIT);
		bRet = FALSE;
		goto Exit;
	}
	WaitForSingleObject(hThread, INFINITE);
	VirtualFreeEx(hProcess, pDllName, dwStrLen, MEM_DECOMMIT);
	bRet = TRUE;
Exit:
	if (hThread != NULL)
	{
		CloseHandle(hThread);
		hThread = NULL;
	}
	if (hMoude != NULL)
	{
		FreeLibrary(hMoude);
		hMoude = NULL;
	}
	if (hProcess > NULL)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}

	return bRet;
}
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	DWORD PID = 0;
	WCHAR DLLPath[MAX_PATH] = { 0 };
	HWND GameWndHandle = NULL;

	if (GetSelfPath(DLLPath) == FALSE)
	{
		OutInfo(L"��ȡ����·��ʧ��");
		return 0;
	}
	wcscat_s(DLLPath, MAX_PATH, L"SteamImageManager.dll");
	//OutInfo(L"%s", DLLPath);
	
	while (TRUE)
	{
		PID = GetProcessID(L"Steam.exe");
		GameWndHandle = FindWindow(
			TEXT("vguiPopupWindow"),
			NULL
			);

		//auto hwnd = ::FindWindow(L"UnrealWindow", L"�������� ");

		if (/*hwnd &&*/
			GameWndHandle != NULL &&
			PID > 4 &&
			gPID != PID
			)
		{
			//ע��
			OutInfo(L"��ʼע��");
			if (RemoteThreadInject(PID, DLLPath) == FALSE)
			{
				OutInfo(L"ע��ʧ��");
			}
			else
			{
				OutInfo(L"ע��ɹ�");
				gPID = PID;
			}
		}

		Sleep(5000);
	}

	return 0;
}