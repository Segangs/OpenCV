#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

// 이미지 파일이 있는 폴더 경로
const String folderPath = "/home/segang/segang/OpenCV/data/";

// contour 정보를 이용해 이미지에 라벨과 보조 도형을 그리는 함수
void setLabel(Mat &img, const vector<Point> &pts, const String &label)
{
    // 컨투어를 감싸는 축 정렬 직사각형
    Rect rc = boundingRect(pts);

    // 컨투어를 감싸는 최소 회전 사각형
    RotatedRect rc2 = minAreaRect(pts);

    // 컨투어를 감싸는 최소 원의 중심과 반지름 계산
    Point2f center;
    float radius;
    minEnclosingCircle(pts, center, radius);

    // 원래 축에 맞춰진 직사각형을 빨간색으로 그림
    rectangle(img, rc, Scalar(0, 0, 255), 1);

    // 최소 회전 사각형을 초록색으로 그림
    Point2f vertices[4];
    rc2.points(vertices);
    for (int i = 0; i < 4; i++)
        line(img, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0), 3);

    // 최소 외접 원을 파란색으로 그림
    circle(img, center, cvRound(radius), Scalar(255, 0, 0), 2);

    // 분류된 라벨을 직사각형의 왼쪽 위 위치에 표시
    putText(img, label, rc.tl(), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255));
}

int main(int argc, char *argv[])
{
    Mat img = imread(folderPath + "polygon.bmp", IMREAD_COLOR);

    if (img.empty())
    {
        cerr << "Image load failed!" << endl;
        return -1;
    }

    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    // 이진화를 통해 객체와 배경을 분리
    Mat bin;
    threshold(gray, bin, 200, 255, THRESH_BINARY_INV | THRESH_OTSU);

    // 컨투어를 저장할 벡터
    vector<vector<Point>> contours;
    findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    // 검출된 모든 컨투어를 순회
    for (vector<Point> pts : contours)
    {
        // 노이즈로 판단되는 작은 컨투어는 제외
        if (contourArea(pts) < 400)
            continue;

        // 컨투어를 근사한 다각형으로 단순화
        vector<Point> approx;
        approxPolyDP(pts, approx, arcLength(pts, true) * 0.02, true);

        int vtc = (int)approx.size();

        if (vtc == 3)
        {
            // 세 꼭짓점: 삼각형
            setLabel(img, pts, "TRI");
        }
        else if (vtc == 4)
        {
            // 네 꼭짓점: 사각형
            setLabel(img, pts, "RECT");
        }
        else
        {
            // 원 형태 여부 판단을 위해 형태 유지 비율 계산
            double len = arcLength(pts, true);
            double area = contourArea(pts);
            double ratio = 4. * CV_PI * area / (len * len);

            if (ratio > 0.85)
            {
                // 비율이 충분히 크면 원으로 분류
                setLabel(img, pts, "CIR");
            }
        }
    }

    imshow("img", img);

    waitKey();
    return 0;
}