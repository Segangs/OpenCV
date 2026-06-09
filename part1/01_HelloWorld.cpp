#include <iostream>
#include <opencv2/opencv.hpp>
#include "colors.hpp"

// 컬러를 숫자로 치기 귀찮으니까.. colors.hpp만들어 놈

using namespace std;
using namespace cv;

String folderPath = "/home/segang/segang/OpenCV/img/";

int main()
{
    cout << "hello world" << endl;
    // Mat img = imread(folderPath + "IMG_1745.jpg");
    // Mat img = Mat::zeros(480, 640, CV_8UC3);
    // Size size(640, 480);
    // Mat img = Mat(480, 640, CV_8UC3, Scalar(255, 255, 255));
    Mat img = Mat(480, 640, CV_8UC3, Color::Red);
    Size size(1024, 768);
    resize(img, img, size);
    imshow("lenna", img);
    waitKey();
    return 0;
}
