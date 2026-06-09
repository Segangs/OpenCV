#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/freetype.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


String folderPath = "/home/segang/segang/OpenCV/data/";

Ptr<cv::freetype::FreeType2> rapperFreeTypeCenterSetup(const String &fontpath)
{
    Ptr<cv::freetype::FreeType2> ft2 = freetype::createFreeType2();
    ft2->loadFontData(fontpath, 0);
    return ft2;
}

void rapperFreeTypeCenter(Mat &img, Ptr<cv::freetype::FreeType2> ft2, const String &text, int textHeight, int thickness, int line_type, const Scalar &color, const Point &textOrg, bool withRect)
{
    int baseline = 0;
    Size textSize = ft2->getTextSize(text, textHeight, thickness, &baseline);
    Point top_left(textOrg.x, textOrg.y - textSize.height);
    Rect textRect(top_left, textSize + Size(0, 20));
    if (withRect)
    {
        rectangle(img, textRect, color, 3, line_type); // 텍스트 영역 사각형 그리기
    }

    ft2->putText(img, text, top_left, textHeight, color, thickness, line_type, false);
}

int main()
{
    Mat img(1000, 1200, CV_8UC3, Color::White);

    int a = -800, b = -1400, c = -1900;
    auto ft2 = rapperFreeTypeCenterSetup("/home/segang/segang/OpenCV/data/MonaS10.ttf");

    String text = "이것은 IOT 수업 입니다.";
    String text2 = "The quick brown fox jumps over the lazy dog.";
    String text3 = "텍스트입니다.";

    while (true)
    {
        img.setTo(Color::White);                         // 배경 그리기
        circle(img, Point((a + 300) % img.cols, 300), 6, Color::Red, -1); // 빨간색 원 그리기
        rapperFreeTypeCenter(img, ft2, text, 100, -1, LINE_AA, Color::Blue, Point(a, 300), true);
        rapperFreeTypeCenter(img, ft2, text2, 70, -1, LINE_AA, Scalar(30, 30, 30), Point(b, 430), true);
        rapperFreeTypeCenter(img, ft2, text3, 70, -1, LINE_AA, Scalar(0, 140, 220), Point(c, 200), false);

        imshow("img", img);
        if (waitKey(33) == 27)
            break;
        a += 14;
        b += 20;
        c += 26;

        if (a > img.cols + 500) a = -800;
        if (b > img.cols + 500) b = -800;
        if (c > img.cols + 500) c = -800;
    }
    destroyAllWindows();
    return 0;
}