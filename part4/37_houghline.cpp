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
    int low_th = 50;
    int high_th = 150;
    int h_thresh = 50;
    namedWindow("frame", WINDOW_AUTOSIZE);
    namedWindow("frame_ori", WINDOW_AUTOSIZE);
    createTrackbar("lowthreshould", "frame", &low_th, 255);
    createTrackbar("highthreshould", "frame", &high_th, 255);
    createTrackbar("hough_thresh", "frame", &h_thresh, 255);

   vector<Vec4f> lines;

    while (true)
    {
        cap >> frame;
        Canny(frame, edge, low_th, high_th);
        HoughLinesP(edge, lines, 1, CV_PI / 180, h_thresh, 10, 20);
        for (int i = 0; i < 20; i++)
            line(frame, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Color::Red, 2, LINE_AA);
        if (waitKey(30) == 27)
            break;
        imshow("frame_ori", frame);
        imshow("frame", edge);
    }
    cap.release();
    destroyAllWindows();
    return 0;
}