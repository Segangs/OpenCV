#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
  const String folderPath = "/home/segang/segang/OpenCV/data/";
    Mat img = imread(folderPath + "sudoku.jpg", IMREAD_GRAYSCALE);
    Mat img2, img3;
    threshold(img, img2, 0, 255, THRESH_OTSU);
    adaptiveThreshold(img, img3, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 101, 5);
    imshow("sudoku", img);
    imshow("thresh_binary", img2);
    imshow("adaptive", img3);
    waitKey();

    return 0;
}