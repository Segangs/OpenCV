#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

// 중심점을 기준으로 점들을 시계방향(좌상, 우상, 우하, 좌하)으로 정렬하는 함수
void sortCorners(vector<Point2f>& pts) {
    Point2f center(0, 0);
    for (auto& p : pts) center += p;
    center *= (1.0 / pts.size());
    
    sort(pts.begin(), pts.end(), [&center](const Point2f& a, const Point2f& b) {
        return atan2(a.y - center.y, a.x - center.x) < atan2(b.y - center.y, b.x - center.x);
    });
}

int main()
{
    // 카메라 열기
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
    
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    
    // 종이 임계값 범위를 수동으로 조절할 수 있도록 트랙바 추가
    namedWindow("Result");
    // OpenCV 트랙바는 정수형만 지원하므로, 범위를 10배로 늘려 float 수준(0.1 단위)의 정밀도로 조절합니다.
    int lower_paper_tb = 1500; 
    int upper_paper_tb = 2550; 
    createTrackbar("Paper Lower", "Result", &lower_paper_tb, 2550);
    createTrackbar("Paper Upper", "Result", &upper_paper_tb, 2550);

    while (true)
    {
        Mat frame;
        cap >> frame;
        if (frame.empty()) break;
        
        Mat displayImg = frame.clone();
        
        // 그레이스케일 변환 및 노이즈 제거 (테이프 주변 밝기 인식용으로 먼저 수행)
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, gray, Size(5, 5), 0);
        
        // 1. HSV 변환
        Mat hsv;
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        
        // 2. 노란색 테이프 threshold
        // OpenCV에서 Hue(색상) 범위는 0~179, 노란색은 대략 20~40 구간
        Mat mask_yellow;
        Scalar lower_yellow(20, 120, 120); // 채도(S)와 명도(V) 하한값을 120으로 올려 옅은 노란색 노이즈 차단
        Scalar upper_yellow(40, 255, 255);
        inRange(hsv, lower_yellow, upper_yellow, mask_yellow);
        
        // 3. morphology로 노이즈 제거
        morphologyEx(mask_yellow, mask_yellow, MORPH_OPEN, kernel);
        morphologyEx(mask_yellow, mask_yellow, MORPH_CLOSE, kernel);
        
        // 4. 테이프 contour 검출
        vector<vector<Point>> tape_contours;
        findContours(mask_yellow, tape_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        
        vector<Point> tape_approx;
        double max_tape_area = 0;
        
        // 5. approxPolyDP로 테이프 4점 검출
        for (const auto& pts : tape_contours)
        {
            double area = contourArea(pts);
            if (area > 1000 && area > max_tape_area) // 최소 크기 필터링
            {
                vector<Point> approx;
                approxPolyDP(pts, approx, arcLength(pts, true) * 0.05, true);
                if (approx.size() == 4)
                {
                    max_tape_area = area;
                    tape_approx = approx;
                }
            }
        }
        
        Mat H; // 테이프 기준 Homography 행렬
        Point2f tape_center(-1, -1); // 테이프 중심점
        
        // 6. 노란색 테이프 기준 homography 생성
        if (tape_approx.size() == 4)
        {
            vector<Point2f> src_pts(tape_approx.begin(), tape_approx.end());
            sortCorners(src_pts); // 좌상, 우상, 우하, 좌하 정렬
            
            tape_center = Point2f(0, 0);
            for (const auto& p : src_pts) tape_center += p;
            tape_center *= 0.25f;
            
            // 화면에 테이프 영역 그리기
            for (int i = 0; i < 4; i++)
                line(displayImg, src_pts[i], src_pts[(i + 1) % 4], Color::Blue, 3);
            putText(displayImg, "Yellow Tape", src_pts[0] - Point2f(0, 10), FONT_HERSHEY_SIMPLEX, 0.7, Color::Blue, 2);
            
            // 테이프를 100x100 픽셀의 단위 정사각형이라고 가상으로 매핑
            // 이 단위 크기를 기준으로 삼으면 계산하기 편해집니다.
            vector<Point2f> dst_pts = { Point2f(0, 0), Point2f(100, 0), Point2f(100, 100), Point2f(0, 100) };
            H = findHomography(src_pts, dst_pts);
        }
        
        // 7. 종이 영역 threshold
        // 트랙바의 정수 값을 10.0으로 나누어 float 형태의 임계값으로 변환
        float lower_paper = lower_paper_tb / 10.0f;
        float upper_paper = upper_paper_tb / 10.0f;
        
        Mat mask_paper;
        inRange(gray, Scalar(lower_paper), Scalar(upper_paper), mask_paper);
        
        // 현재 트랙바로 설정된 종이 인식 범위를 화면에 출력
        string rangeText = "Paper Range: " + format("%.1f", lower_paper) + " ~ " + format("%.1f", upper_paper);
        putText(displayImg, rangeText, 
                Point(30, 90), FONT_HERSHEY_SIMPLEX, 0.7, Color::Magenta, 2);
        
        // 이진화 시 종이 안의 테이프 부분이 까맣게 파이는 현상을 메우기 위해 CLOSE 연산 추가
        Mat close_kernel = getStructuringElement(MORPH_RECT, Size(11, 11));
        morphologyEx(mask_paper, mask_paper, MORPH_CLOSE, close_kernel);
        morphologyEx(mask_paper, mask_paper, MORPH_OPEN, kernel);
        
        // 8. 종이 contour / polygon 검출
        vector<vector<Point>> paper_contours;
        findContours(mask_paper, paper_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        
        vector<Point> best_paper_contour;
        double max_paper_area = 0;
        
        for (const auto& pts : paper_contours)
        {
            double area = contourArea(pts);
            if (area > 5000 && area > max_paper_area)
            {
                // [핵심 해결책] 검출된 종이 영역 내부에 노란 테이프 중심점이 있는지 검사
                if (tape_center.x >= 0 && tape_center.y >= 0)
                {
                    if (pointPolygonTest(pts, tape_center, false) < 0)
                        continue; // 노란 테이프를 포함하지 않는 엉뚱한 배경 사각형은 무시
                }
                
                // 4점 조건을 없애고, 테이프를 포함하는 가장 큰 윤곽선 자체를 저장
                max_paper_area = area;
                best_paper_contour = pts;
            }
        }
        
        if (!best_paper_contour.empty() && !H.empty())
        {
            // 비정사각형/찢어진 종이의 윤곽선 전체를 화면에 그림
            vector<vector<Point>> draw_contours = { best_paper_contour };
            drawContours(displayImg, draw_contours, -1, Color::Green, 3);
            putText(displayImg, "Paper", best_paper_contour[0] - Point(0, 10), FONT_HERSHEY_SIMPLEX, 0.7, Color::Green, 2);
            
            // 9. 종이 contour 전체를 homography로 변환하기 위해 타입 변환 (Point -> Point2f)
            vector<Point2f> paper_pts_2f;
            for (const auto& pt : best_paper_contour)
                paper_pts_2f.push_back(Point2f(pt.x, pt.y));
            
            // 9. 종이 contour를 homography로 변환
            vector<Point2f> paper_transformed;
            perspectiveTransform(paper_pts_2f, paper_transformed, H);
            
            // 10. contourArea로 비율 넓이 계산
            // 테이프의 투영 면적(100*100 = 10000)으로 종이의 투영 면적을 나누면 테이프 대비 종이 넓이가 됨.
            double transformed_area = contourArea(paper_transformed);
            double ratio = transformed_area / 10000.0;
            
            string text = "Paper / Tape Area Ratio : " + format("%.2f", ratio);
            putText(displayImg, text, Point(30, 50), FONT_HERSHEY_SIMPLEX, 1.0, Color::Red, 3);
        }
        
        imshow("Result", displayImg);
        
        if (waitKey(30) == 27) // ESC 누르면 종료
            break;
    }
    
    cap.release();
    destroyAllWindows();
    return 0;
}
