#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void on_level_change(int pos, void *data);

int main()
{
    const String folderPath = "/home/segang/segang/OpenCV/data/";
     Mat img1 = imread(folderPath + "lenna.bmp");
    Mat img2 = imread(folderPath + "mask_smile.bmp", IMREAD_GRAYSCALE);

    img1.setTo(Color::Yellow, img2);

    imshow("img", img1);

    Mat airplane = imread(folderPath + "airplane.bmp");
    Mat maskAirPlane = imread(folderPath + "mask_plane.bmp", IMREAD_GRAYSCALE);
    Mat sky = imread(folderPath + "field.bmp");

    resize(airplane, airplane, Size(), 0.5, 0.5, INTER_AREA);
    resize(maskAirPlane, maskAirPlane, Size(), 0.5, 0.5, INTER_NEAREST);
    int x = 0;
    int y = 50;
    int dx = 1;

    while (true)
    {
        x += dx;
        Mat frame = sky.clone();
        Rect roiRect(x, y, airplane.cols, airplane.rows);

        Mat roi = frame(roiRect);
        airplane.copyTo(roi, maskAirPlane);
        imshow("sky", frame);
        if(waitKey(30) == 27)
            break;

        if ( x <= 0 || x + airplane.cols >= sky.cols)
        {
            dx = -dx;
            flip(airplane, airplane, 1);
            flip(maskAirPlane, maskAirPlane, 1);
        }
    }
    destroyAllWindows();
    return 0;
}