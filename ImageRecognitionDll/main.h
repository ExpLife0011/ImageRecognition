#pragma once
#include <windows.h>
#include <cstdint>




typedef struct _config_t
{
	std::uint32_t duration_seconds_;	// ���ʱ��(��)
}config_t;

typedef struct _result_t
{
	bool is_team_;						// �Ƿ����
	char user_name_[128];				// �����(UTF8����)
	std::uint32_t order_;				// ����
	std::uint32_t kill_;				// ɱ����
	std::uint32_t scores_;				// ����
	char currPath[MAX_PATH];            //ͼƬȫ·��
}result_t;


void __stdcall  set_config(const config_t & con);
typedef void(__stdcall *result_callback_func)(const result_t &, const void *);
void __stdcall set_callback(const result_callback_func func, const void* _t);
void __stdcall install();

DWORD WINAPI ThreadProcMain(_In_ LPVOID lpParameter);
int retrunLparamDown(int key);