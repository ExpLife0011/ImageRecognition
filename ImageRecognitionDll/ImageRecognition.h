#pragma once

/*
// ʶ���
*/
#include <vector>
#include "opencv2/opencv.hpp"

//�ֿ�ṹ��
typedef struct _EM_FONTS_PRO
{
	cv::Mat _mat;  //ͼƬ
	std::string _text; //ͼƬ��Ӧ����
}EM_FONTS_PRO, *PEM_FONTS_PRO;


//���ƶȽṹ��
typedef struct _EM_IMAGE_FONTS_PRO
{
	int _samilar; //���ƶ�
	std::string _text; //����
}EM_IMAGE_FONTS_PRO,*PEM_IMAGE_FONTS_PRO;

//ͼƬ�ṹ��
typedef struct _EM_IMAGE_PRO
{
	cv::Rect _rc;
	EM_IMAGE_FONTS_PRO _eip;
}EM_IMAGE_PRO, *PEM_IMAGE_PRO;


class ImageRecognition
{

public:
	ImageRecognition();
	~ImageRecognition();
	//��ʼ���ֿ�
	bool InitFonts();
	//ʶ��ͼ�񣬲������ַ�������
	void Recognition(cv::Mat _mat, std::string &_ret);
private:
	//���غ��ֿ�������˼����
	void RecognitionFonts(cv::Mat _mat, EM_IMAGE_FONTS_PRO & _eip);
	//�ж�����ͼƬ�Ƿ�����
	int GetImageSemblance(cv::Mat _mat1, cv::Mat _mat2);
	//��ͼƬת��Ϊ32*32��ͼƬ
	cv::Mat ConvertImgTo32Pix(cv::Mat& _mat);
private:
	std::vector<EM_FONTS_PRO> m_fontsList; //�����
	int m_bltPixel;
};
