// ImageRecognition.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <cstdint>
#include <iostream>

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


void __stdcall callback_func(const result_t& _t, const void* _s);
typedef void(__stdcall *set_config)(const config_t & con);
typedef void(__stdcall *result_callback_func)(const result_t &, const void*);
typedef void(__stdcall *set_callback)(result_callback_func func, const void*);
typedef void(__stdcall *install)();
int _tmain(int argc, _TCHAR* argv[])
{

	wchar_t _currPath[MAX_PATH] = { 0 };
	::GetModuleFileNameW(NULL, _currPath, MAX_PATH);

	auto hModule = LoadLibrary("ImageRecognitionDll.dll");
	if (!hModule){
		return -1;
	}

	//��ȡ�����������ļ�����
	set_config pSetConfig = (set_config)GetProcAddress(hModule, "set_config");
	if (!pSetConfig){
		std::cout << "��ȡģ�麯��set_configʧ�ܣ�" << std::endl;
		return -1;
	}


	config_t _config;
	_config.duration_seconds_ = 1000;
	pSetConfig(_config);

	//��ȡ�����ûص�����
	set_callback pSetCallback = (set_callback)GetProcAddress(hModule, "set_callback");
	if (!pSetCallback){
		std::cout << "��ȡģ�麯��set_callbackʧ�ܣ�" << std::endl;
		return -1;
	}

	pSetCallback(callback_func, nullptr);

	//��ʼִ��
	install _inst = (install)GetProcAddress(hModule, "install");
	if (!_inst){
		std::cout << "��ȡģ�麯��installʧ�ܣ�" << std::endl;
		return -1;
	}

	_inst();

	getchar();
	FreeLibrary(hModule);


	return 0;
}

void __stdcall callback_func(const result_t& _t, const void* _s)
{
	std::cout << "���֣�" << _t.user_name_ << " �Ƿ���ӣ�" << _t.is_team_ << " ������" << _t.order_ << " ɱ������" << _t.kill_ << " ������" << _t.scores_  << "ͼƬ·��: "<< _t.currPath << std::endl;
}

