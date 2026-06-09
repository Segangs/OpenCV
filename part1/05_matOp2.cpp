#include "colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

String folderPath = "/home/segang/segang/OpenCV/data/";

void printMat(InputArray _mat, OutputArray _output);

int main()
{
    Mat img1 = imread(folderPath + "dog.bmp");

    Mat img2 = img1;
    Mat img3;
    img3 = img1;

    Mat img4 = img1.clone();
    Mat img5;
    img1.copyTo(img5);
    //Vec3b &pixel = img5.at<Vec3b>(0, 0);
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
        Vec3b &pixel = img5.at<Vec3b>(i, j);
        cout << pixel[0] << pixel[1] << pixel[2] << endl;
        pixel[0] = 0;
        pixel[1] = 0;
        pixel[2] = 0;
        }
    }
    Vec

    img1.setTo(Color::Yellow);
    Rect roi(220, 120, 200, 200);
    Mat img6 = img4(roi); // 얕은 복사
    img6.setTo(Color::Black);

    imshow("img1", img1);
    imshow("img2", img2);
    imshow("img3", img3);
    imshow("img4", img4);
    imshow("img5", img5);
    imshow("img6", img6);
    waitKey();
    destroyAllWindows();
    return 0;
}

void printMat(InputArray _mat, OutputArray _output)
{
    Mat img = _mat.getMat();
    cout << img << endl;
    Mat img2 = img + 3;
    img2.copyTo(_output);
}