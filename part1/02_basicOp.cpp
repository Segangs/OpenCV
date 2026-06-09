#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


cv::String folderPath = "/home/segang/segang/OpenCV/img/";

int main()
{
    Point_<int> p1(1, 3);
    Point_<float> p2(3.14, 4.31);
    Point_<int> p3(5, 8);

    cout << p1 << p2 << endl; // 연산자 오버로딩
    cout << p1 + p3 << endl;

    Point p4(3, 4);   // Point_ <int>
    Point2i p5(3, 4); // Point <int>
    Point2f p6(3.11, 2.11);

    cout << p4 << p5 << p6 << endl;

    Size sz1;
    Size sz2(10, 20);

    sz1.width = 10;
    sz1.height = 20;

    Size2f sz3(3.14, 6.78);
    cout << "sz1.area(): " << sz1.area() << " sz1.aspectRatio():" << sz1.aspectRatio() << endl;

    Rect rc1;
    Rect rc2(10, 10, 20, 20);
    Rect rc3 = rc1 + Size(50, 40);  // 원래 사각형에서 크기(가로, 세로)만 각각 50, 40만큼 키웁니다.
    Rect rc4 = rc1 + Point(10, 10); // 사각형의 크기는 그대로 두고, 위치(x, y)만 우측 하단으로 (10,10) 이동시킵니다.
    Rect rc5 = rc3 & rc4;           // 두 사각형이 겹치는 영역만 찾아서 새로운 사각형으로 반환합니다.
    Rect rc6 = rc3 | rc4;           // 두 사각형을 모두 포함하는 가장 작은 하나의 거대한 사각형을 만듭니다.

    cout << "rc3:" << rc3 << endl;
    cout << "rc4:" << rc4 << endl;
    cout << "rc5:" << rc5 << endl;
    cout << "rc6:" << rc6 << endl;
    cout << rc6.contains(p1); // 이 사각형 안에 이 점이 들어있니?

    Mat img = Mat::zeros(Size(100, 100), CV_8UC3);
    rectangle(img, rc3, (255, 255, 255));
    rectangle(img, rc4, (255, 255, 255));
    rectangle(img, rc5, (255, 0, 0));
    rectangle(img, rc6, (0, 255, 255));

    imshow("img", img);
    waitKey();

    return 0;
}
