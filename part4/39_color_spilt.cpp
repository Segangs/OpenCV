#include <iostream>
#include <opencv2/opencv.hpp>
#include "../part1/colors.hpp"

// 컬러를 숫자로 치기 귀찮으니까.. colors.hpp만들어 놈

using namespace std;
using namespace cv;

int main()
{
    const String folderPath = "/home/segang/segang/OpenCV/data/";
    Mat img = imread(folderPath + "candies.png");
    vector<Mat> bgr_planes;
    Mat b_planes, g_planes, r_planes;
    Mat empty(img.size(), CV_8UC1, Scalar(0));

    split(img, bgr_planes);
    merge(vector<Mat>({bgr_planes[0], empty, empty}), b_planes);
    merge(vector<Mat>({empty, bgr_planes[1], empty}), g_planes);
    merge(vector<Mat>({empty, empty, bgr_planes[2]}), r_planes);
    
    imshow("candies", img);
    imshow("b_plane", b_planes);
    imshow("g_plane", g_planes);
    imshow("r_plane", r_planes);
    waitKey();
    return 0;
}
