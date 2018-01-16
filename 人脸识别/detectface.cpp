#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

#include <opencv.hpp>
#include "facedetect-dll.h"
#pragma comment(lib,"libfacedetect.lib")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )//����ʾ����̨


using namespace cv;
using namespace std;
//��������
int detectAndDisplay(Mat frame);
extern bool stop = false;//����ȫ�ֱ���
extern string mode = "manual";
extern unsigned _int64 time = 1000;

static void help()
{
    cout <<
	"\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"
    "\n@This program shows image from camera.\n"
	"\n@And Detect Face!!!\n"
	"\n@Version:1.15.11.0\n"
    "\n@Call:./detectface.exe /auto 1000\n"
	"\n@Call:./detectface.exe /manual 5000\n"
	"\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"<< endl;
}

int main(int argc, char** argv )
{
	help();
	
	//������ͷ�豸
	VideoCapture cap(0);

	if(!cap.isOpened())
	{
		cout<<"---Device Can Not Open---Break";
		waitKey(1000);
		return 1;
	}

	Mat frame;
	if(argc > 1 )
	{
		mode = argv[1];
	}

	if(argc > 2 )
	{
		mode = argv[1];
		time = _strtoui64(argv[2],NULL,10);
	}

		while(!stop)
		{
			//�鿴ͼ���Ƿ�ץȡ����ץȡ���͵��ü��face����ʾ��δ��⵽����������˳���
			cap>>frame;
			if(!frame.empty())
				{
					detectAndDisplay(frame);
				}
			else
				{
					cout<<"---No Capture Frame---Break";
					waitKey(1000);
					return 2;
				}

			if(waitKey(20) > 0) //����֡��50Hz
			{
				cout<<"This is FR/HZ";
			}
			
			}
		return 0;
	}

int detectAndDisplay(Mat frame)
{

	Mat edges;
	Mat gray;

	//GaussianBlur(edges,edges,Size(7,7),1.5,1.5);//��˹�˲�
	//Canny(edges,edges,0,30,3);//��Ե���
	flip(frame,edges,1);

	//ÿһ֡ת��Ϊ�Ҷ�
	cvtColor(edges,gray,CV_BGR2GRAY);  //��frameͼ����BGRת��ΪGRAY��Ŀ��ͼ��edges
	if(gray.empty())
	{
		fprintf(stderr, "Can not load the image file.\n");
	    cin.get();
		return 2;
	}

	int *pResults = NULL; 
	///////////////////////////////////////////
	// ��Ƕ�������� facedetect_multiview()
	// ���Լ�ⲿ�����������������������Ҫ�������溯��Ϊfacedetect_frontal()
	//////////////////////////////////////////
	//!!! �����źű���Ϊת�Ҷȵ�ͼ�񣨵�ͨ���źţ�
	//!!! ��Ҫ�ͷű���pResults !!!
	pResults = facedetect_frontal((unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, gray.step,
											   1.2f, 4, 24);
	printf("%d faces detected.\n", (pResults ? *pResults : 0));

	//print the detection results
	for(int i = 0; i < (pResults ? *pResults : 0); i++)
	{ 
        short * p = ((short*)(pResults+1))+6*i;
		int x = p[0];
		int y = p[1];
		int w = p[2];
		int h = p[3];
		int neighbors = p[4];
		int angle = p[5];

		//printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x,y,w,h,neighbors, angle);
		rectangle(edges, cvPoint(x,y), cvPoint(x+w, y+h), CV_RGB(255,0,0), 2);//�������ο�
		if(neighbors > 5)
		{
			imshow("Camera-˵����ץȡ��������ᶨ��ͼ�����룬��ȷ�Ϻ�������˳���",edges);
			if(mode == "/auto")
			{
				stop = true;
			}

			if(waitKey(time) > 0)
			{
				cout<<"Detect Face Pass!!!"<<endl;	
				stop = true;
			}
		}
	}

	     imshow("Camera-˵����ץȡ��������ᶨ��ͼ�����룬��ȷ�Ϻ�������˳���",edges);
		 return -1;
}
