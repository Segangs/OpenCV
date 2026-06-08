#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

String folderPath = "/home/segang/segang/OpenCV/img/";

int main()
{
    cout << "hello world" << endl;
    Mat img = imread(folderPath + "IMG_1745.jpg");
    //Mat img= Mat::zeros(480, 640, CV_8UC3);
    Size size (1024, 768);
    resize(img, img, size);
    imshow ("lenna", img);
    waitKey();
    return 0;
}
