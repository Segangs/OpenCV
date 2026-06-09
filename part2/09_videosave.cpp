#include <iostream>
#include <opencv2/opencv.hpp>
#include "../part1/colors.hpp"

using namespace std;
using namespace cv;

String folderPath = "/home/segang/segang/OpenCV/data/";

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

    // 출력 설정
    int w = 1280;
    int h = 720;
    double fps = 30.0;
    int fourcc = VideoWriter::fourcc('D', 'I', 'V', 'X');
    VideoWriter outVideo(folderPath + "Flip_roi_inverse.avi", fourcc, fps, Size(w, h));

    Mat frame;
    int x = (1280 - 400) / 2;
    int y = (720 - 400) / 2;
    int move_x = 0;

    for (int i = 0; i < 1000; ++i)
    {
        cap >> frame;
        flip(frame, frame, 1);
        Mat roi = frame(Rect(move_x, y, 400, 400));
        roi = ~roi;
        rectangle(frame, Rect(move_x, y, 400, 400), Color::Red,2);
        if (waitKey(30) == 27)
            break;
        outVideo << frame;
        imshow("frame", frame);
        move_x += 1;
        if (move_x > 500){
            move_x = 0;
        }
    }
    cap.release();
    outVideo.release();
    destroyAllWindows();
    return 0;
}