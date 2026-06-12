#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;
const String folderPath = "/home/segang/segang/OpenCV/data/";
int main()
{
    VideoCapture cap(folderPath + "IMG_6906.MOV");
    int fps = cap.get(CAP_PROP_FPS);
    if (!cap.isOpened())
    {
        cerr << "Video open failed!" << endl;
        return -1;
    }

    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    Mat frame;
    int frame_cnt = 0;

    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;

        frame_cnt++;
        // 60fps 영상을 30fps로 처리하기 위해 프레임 하나씩 건너뛰기
        if (frame_cnt % 2 != 0)
            continue;

        // 연산 속도 향상을 위해 해상도를 1280x720으로 조절
        resize(frame, frame, Size(1280, 720));

        vector<Rect> detected;
        hog.detectMultiScale(frame, detected);

        for (Rect r : detected)
        {
            Scalar c = Scalar(rand() % 256, rand() % 256, rand() % 256);
            rectangle(frame, r, c, 3);
        }

        imshow("frame", frame);

        // 30fps 기준 대기 시간 (1000 / 30 = 약 33ms)
        if (waitKey(33) == 27)
            break;
    }

    return 0;
}