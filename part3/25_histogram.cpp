#include "colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat calcGrayHist(const Mat &img)
{
    CV_Assert(img.type() == CV_8UC1);

    Mat hist;
    int channel[] = {0};
    int dims = 1;
    const int histSize[] = {256};
    float graylevel[] = {0, 256};
    const float *ranges[] = {graylevel};

    calcHist(&img, 1, channel, noArray(), hist, dims, histSize, ranges);

    return hist;
}

Mat getGrayHistImage(const Mat &hist)
{
    CV_Assert(hist.type() == CV_32FC1);     // gray 스케일 확인 에러 발생!
    CV_Assert(hist.size() == Size(1, 256)); // histogram 인지 확인!

    double histMax;
    minMaxLoc(hist, 0, &histMax);

    Mat imgHist(100, 256, CV_8UC1, Scalar(255));
    for (int i = 0; i < 256; ++i)
    {
        line(imgHist, Point(i, 100), Point(i, 100 - (cvRound(hist.at<float>(i, 0))) / histMax * 100), Color::Black);
    }
    return imgHist;
}

int main()
{
    const String folderPath = "/home/aa/kuBig2026/opencv_ex/data/";
    double min, max;

    cout << "hello, world" << endl;
    Mat img = imread(folderPath + "hawkes.bmp", IMREAD_GRAYSCALE);
    Mat img2 = 2.f * img;
    Mat img3 = img + (img - 128) * 1.f;
    minMaxLoc(img, &min, &max);
    Mat img4 = (img - min) * 255 / (max - min);
    Mat img5;
    equalizeHist(img, img5);

    imshow("lenna", img);
    imshow("multiple", img2);
    imshow("center contrast", img3);
    imshow("stretching", img4);
    imshow("equalize", img5);
    waitKey();

    return 0;
}