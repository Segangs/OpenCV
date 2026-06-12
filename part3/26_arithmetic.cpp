#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    const String folderPath = "/home/segang/segang/OpenCV/data/";
    Mat img = imread(folderPath + "lenna256.bmp", IMREAD_GRAYSCALE);
    Mat img2 = imread(folderPath + "square.bmp", IMREAD_GRAYSCALE);
    Mat dst1, dst2, dst3, dst4;

    if (img.empty() || img2.empty()) {
        cerr << "ERROR: Could not load image files from " << folderPath << "\n";
        return -1;
    }

    bitwise_and(img, img2, dst1);
    bitwise_or(img, img2, dst2);
    bitwise_xor(img, img2, dst3);
    bitwise_not(img, dst4);

    namedWindow("lenna", WINDOW_NORMAL);
    namedWindow("square", WINDOW_NORMAL);
    namedWindow("and", WINDOW_NORMAL);
    namedWindow("or", WINDOW_NORMAL);
    namedWindow("xor", WINDOW_NORMAL);
    namedWindow("not", WINDOW_NORMAL);

    imshow("lenna", img);
    imshow("square", img2);
    imshow("and", dst1);
    imshow("or", dst2);
    imshow("xor", dst3);
    imshow("not", dst4);
    waitKey(0);

    return 0;
}