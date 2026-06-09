#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/freetype.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


String folderPath = "/home/segang/segang/OpenCV/data/";

int main()
{
    Mat img = imread(folderPath + "lenna.bmp");
    namedWindow("img");
    auto i = getTickCount();

    int keycode;
    while (true)
    {
        i = getTickCount();
        keycode = waitKey(1000);
        imshow("img", img);
        if(keycode == 27)
            break;
        if(keycode == 'v' || keycode == 'V' )
            img = ~img;
        if(keycode != -1){
        cout << "keycode : " << keycode << endl;
        cout << "i:" << i << endl;
        }
        //cout << "frequency : " << getTickFrequency() << endl;
        cout << "fps : " << getTickFrequency() / (getTickCount() - i) << endl;

    }
    destroyAllWindows();
    return 0;
}