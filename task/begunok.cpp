#include <opencv2/opencv.hpp>
#include "cvDirectory.h"
using namespace cv;
using namespace std;


int th = 0;
Mat img, thimg;
void ProcImg(int,void* user_data){
	int* th_type = (int*)user_data;
	threshold(img, thimg, th, 255, *th_type);
	imshow("thimg", thimg);
}

int main(int argc, char** argv){
	string fn;
	if (argc>1) fn= argv[1];
	else fn = "img.jpg";
	img = imread(fn,0);
	imshow(fn, img);
	vector<string> fnms = Directory::GetListFiles("./","*.jpg",false);//�������� ��� ������ ���� jpg � ������� ����� ��� ���������� � ��� ����.

	int th_type = THRESH_BINARY;

	ProcImg(0,&th_type);
	createTrackbar("th", "thimg", &th, 255, ProcImg, &th_type);
	waitKey();
}