#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

String folderPath = "/home/segang/segang/OpenCV/data/";

void blur_ex(Mat &img);

int main()
{
    VideoCapture cap(0, CAP_V4L2);
    if (!cap.isOpened())
    {
        cerr << "카메라를 열 수 없습니다." << endl;
    }

    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(CAP_PROP_FPS, 30);
    int pos = 1;
    namedWindow("frame");
    createTrackbar("alpha", "frame", &pos, 30);

    Mat frame;
    Mat blurM, dst;
    for (int i = 0; i < 1000; ++i)
    {
        cap >> frame;
        if (waitKey(30) == 27)
            break;
        //blur_ex(frame);
        //blur(frame, frame, Size(pos*2+1,pos*2+1));  // 블러 함수
        GaussianBlur(frame, blurM, Size(0,0), double(pos));  // 가우시안 블러 함수
        dst = (1 + (float)pos * 0.1)*frame - (float)pos  * 0.1 *blurM;
        imshow("frame", dst);
    }
    cap.release();
    return 0;
}

void blur_ex(Mat &img)
{
    float data[] = {1,1,1,1,1,1,1,1,1}; // 커널의 데이터 중요!
    Mat blur(3, 3, CV_32FC1, data);
    blur = blur/9.0;

    // Mat dst = Mat::zeros(img.size(), img.type());
    filter2D(img, img, -1, blur, Point(-1, -1), 0, BORDER_REPLICATE);
}