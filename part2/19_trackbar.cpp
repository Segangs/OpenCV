#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

String folderPath = "/home/segang/segang/OpenCV/data/";

void on_level_change(int pos, void *data);

int main()
{
    Mat img = imread(folderPath + "lenna.bmp");
    namedWindow("img");
	int pos = 0;
    createTrackbar("level","img",&pos,255, on_level_change, (void *)&img);

    while (true)
    {
        imshow("img", img);
        if (waitKey(30) == 27)
            break;
    }
    destroyAllWindows();
    return 0;
}

void on_level_change(int pos, void *data)
{
	Mat *img = (Mat *)data;
	cout << pos << endl;
	img -> setTo(Scalar(pos, 0, 0));

}