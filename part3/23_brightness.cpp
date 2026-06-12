#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    const String folderPath = "/home/segang/segang/OpenCV/data/";
    cout << "hello, world" << endl;
    Mat img = imread(folderPath + "lenna.bmp", IMREAD_GRAYSCALE);
    Mat img2;
    Mat img3;
    Mat img4(img.rows, img.cols, img.type());

    add(img, 100, img2);
    img3 = img + 100;
    for (int j = 0; j < img.rows;j++){
        for (int i = 0; i < img.cols;i++){
            img4.at<uchar>(j, i) = img.at<uchar>(j, i) + 100;
        }
    }

    imshow("lenna", img);
    imshow("add", img2);
    imshow("plus", img3);
    imshow("pixel_add", img4);
    waitKey();

    return 0;
}