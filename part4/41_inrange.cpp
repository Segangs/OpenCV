#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

typedef struct
{
    int lower_hue = 0;
    int upper_hue = 179;
} TrackbarData;

void on_hue_changed(int pos, void* data) {}

int main()
{
    VideoCapture cap(0, CAP_V4L2);
    if (!cap.isOpened())
    {
        cerr << "카메라를 열 수 없습니다." << endl;
        return -1;
    }

    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(CAP_PROP_FPS, 30);

    TrackbarData mydata;
    namedWindow("Color Tuning");
    createTrackbar("Lower Hue", "Color Tuning", &mydata.lower_hue, 179, on_hue_changed, nullptr);
    createTrackbar("Upper Hue", "Color Tuning", &mydata.upper_hue, 179, on_hue_changed, nullptr);

    Mat frame, hsv, mask;
    Mat labels, stats, centroids;

    // 모폴로지 연산에 사용할 구조 원소 (3x3 크기의 사각형)
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

    while (true)
    {
        cap >> frame;
        if (frame.empty()) break;

        Mat displayImg = frame.clone();
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // 기본 이진화 마스크 생성
        Scalar lowerb(mydata.lower_hue, 70, 70); // 채도/명도 하한선을 70으로 올려서 무채색 노이즈 차단
        Scalar upperb(mydata.upper_hue, 255, 255);
        inRange(hsv, lowerb, upperb, mask);

        // [해결책 1] 모폴로지 열기(Opening) 연산 적용 -> 잔먼지 노이즈 완전 제거
        morphologyEx(mask, mask, MORPH_OPEN, kernel);

        int cnt = connectedComponentsWithStats(mask, labels, stats, centroids);

        // 가장 큰 객체를 찾기 위한 변수들
        int maxArea = 0;
        int maxIdx = -1;

        for (int i = 1; i < cnt; i++) 
        {
            int area = stats.at<int>(i, CC_STAT_AREA);

            // [해결책 2] 최소 크기 제한을 3000~5000 픽셀 이상으로 대폭 상향
            if (area > 4000) 
            {
                if (area > maxArea) {
                    maxArea = area;
                    maxIdx = i; // 가장 큰 덩어리의 인덱스를 기억
                }
            }
        }

        // [해결책 3] 검출된 후보 중 가장 큰 '단 하나'의 물체만 사각형 표시
        if (maxIdx != -1) 
        {
            int x = stats.at<int>(maxIdx, CC_STAT_LEFT);
            int y = stats.at<int>(maxIdx, CC_STAT_TOP);
            int width = stats.at<int>(maxIdx, CC_STAT_WIDTH);
            int height = stats.at<int>(maxIdx, CC_STAT_HEIGHT);

            // 타겟 물체에만 깔끔하게 사각형 그리기
            rectangle(displayImg, Rect(x, y, width, height), Scalar(0, 255, 0), 3);
            
            string text = "Target Deteted (Area: " + to_string(maxArea) + ")";
            putText(displayImg, text, Point(x, y - 10), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);
        }

        imshow("Color Tuning", displayImg);
        imshow("Cleaned Mask", mask); // 잔먼지가 지워진 마스크 영상 확인 가능

        if (waitKey(30) == 27)
            break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}