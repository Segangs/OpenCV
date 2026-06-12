#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

String folderPath = "/home/segang/segang/OpenCV/data/";

int main()
{
    VideoCapture cap(0, CAP_V4L2);
    if (!cap.isOpened())
    {
        cerr << "카메라를 열 수 없습니다." <<endl;
    }

    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(CAP_PROP_FPS, 30);

    Mat frame, edge;
    int low_th, high_th;
    namedWindow("frame");
    createTrackbar("lowthreshould", "frame", &low_th, 255);
    createTrackbar("highthreshould", "frame", &high_th, 255);

    while(true)
    {
        cap>>frame;
        Canny(frame, edge, low_th, high_th);
        if(waitKey(30) == 27)
            break;
        imshow("frame_ori", frame);
        imshow("frame", edge);
    }
    cap.release();
    return 0;

}