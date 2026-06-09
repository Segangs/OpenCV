#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

String folderPath = "/home/segang/segang/OpenCV/data/";

int main()
{
    Mat img = imread(folderPath + "lenna.bmp");
    namedWindow("img");

    auto start_tick = getTickCount();
    int keycode;
    TickMeter tm1;
    TickMeter tm2;
    int fps = 70;
    int needed_tick_ms;
    tm1.start();
    while (true)
    {
        tm2.start();
        // 필요한 코드 ( 오래걸리는)
        imshow("img", img);
        tm1.stop();
        double elapsed_ms = tm1.getTimeMilli();
        cout << elapsed_ms << endl;
        needed_tick_ms = cvRound(1000.0 / fps - elapsed_ms);
        cout << needed_tick_ms << endl;

        keycode = waitKey(needed_tick_ms);
        if (keycode == 27)
            break;
        if (keycode == 'v' || keycode == 'V')
            img = ~img;
        if (keycode != -1)
        {
            cout << "keycode: " << keycode << endl;
        }
        // cout << "frequency: " << getTickFrequency() << endl;
        tm2.stop();
        cout << "fps: " << tm2.getFPS() << endl;
        tm1.reset();
        tm2.reset();
    }
    destroyAllWindows();
    return 0;
}