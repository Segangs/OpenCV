#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

String folderPath = "/home/segang/segang/OpenCV/data/";

void on_mouse(int event, int x, int y , int flags, void *data);

int main()
{
    Mat img = imread(folderPath + "lenna.bmp");
    namedWindow("img");
    setMouseCallback("img", on_mouse, (void *)&img);

    while (true)
    {
        imshow("img", img);
        if (waitKey(30) == 27)
            break;
    }
    destroyAllWindows();
    return 0;
}

void on_mouse(int event, int x, int y, int flags, void *data)
{
    Mat *img = (Mat *)data;
    static int number;
    cout << " mouse event 발생!! " << number++ << endl;
    static Point ptOld;
    static bool pushed;
    switch (event)
    {
        case EVENT_LBUTTONDOWN:
            ptOld = Point(x,y);
            pushed = true;
            break;
        case EVENT_LBUTTONUP:
            pushed = false;
            break;
        case EVENT_MOUSEMOVE:
            if (pushed)
            {
                line(*img, ptOld, Point(x, y), Color::Red, 2);
                ptOld = Point(x, y);
            }
            break;
    }
}