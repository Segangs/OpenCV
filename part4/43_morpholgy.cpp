#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
  const String folderPath = "/home/segang/segang/OpenCV/data/";
    Mat img = imread(folderPath + "milkdrop.bmp", IMREAD_GRAYSCALE);
    Mat img2, img3, img4, img5;
    threshold(img, img2, 0, 255, THRESH_OTSU);
    erode(img2, img3, Mat(), Point(-1, -1), 5);
    dilate(img2, img4, Mat());
    morphologyEx(img2, img5, MORPH_OPEN, Mat(), Point(-1, -1), 5);

    imshow("sudoku", img);
    imshow("thresh_binary", img2);
    imshow("erode", img3);
    imshow("dialate", img4);
    imshow("open", img5);
    waitKey();

    return 0;
}