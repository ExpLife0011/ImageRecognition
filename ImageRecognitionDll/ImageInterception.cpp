#include "ImageInterception.h"


bool ImageInterception::ImageCapture(CImage& image)
{
	//1.���Ҿ������������Ƿ����
	auto hwnd = ::FindWindow("UnrealWindow", "�������� ");
	if (!hwnd){
		return false;
	}

	//2. �жϾ��������Ƿ��Ƕ��㴰��
	if (GetForegroundWindow() != hwnd){
		return false;
	}

	RECT rc;
	GetWindowRect(hwnd, &rc);

	HDC hdcWindow = GetDC(NULL);
	int nBitPerPixel = GetDeviceCaps(hdcWindow, BITSPIXEL);

	// ����ͼ�����ÿ�ߣ�����
	image.Create((rc.right - rc.left) / 2, (rc.bottom - rc.top) / 3 -40, 24);

	//�жϴ����Ƿ���ȫ����С�Ĵ��ڣ�����Ǵ���ģʽ�����ȥ���ڱ߿��С��
	int with = GetSystemMetrics(SM_CXFULLSCREEN);
	int heigh = GetSystemMetrics(SM_CYFULLSCREEN);

	LONG _x = 0;
	LONG _y = 0;
	if (with > (rc.right - rc.left)){
		_x = rc.left + 20;
		_y = rc.top + 40;
	}
	else{
		_x = rc.left;
		_y = rc.top;
	}


	// ��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת��
	BitBlt(
		image.GetDC(),  // ���浽��Ŀ�� ͼƬ���� ������
		0, 0,     // ��ʼ x, y ����
		(rc.right - rc.left) / 2, (rc.bottom - rc.top) /3 - 40,  // ��ͼ���
		hdcWindow,      // ��ȡ����� �����ľ��
		_x, _y,           // ָ��Դ�����������Ͻǵ� X, y �߼�����
		SRCCOPY);

	// �ͷ� DC���
	ReleaseDC(NULL, hdcWindow);
	// �ͷ�ͼƬ������
	image.ReleaseDC();

	return true;
}
