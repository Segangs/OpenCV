#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

    const String folderPath = "/home/segang/segang/OpenCV/data/";

int main()
{
    VideoCapture cap(0, CAP_V4L2);
    if (!cap.isOpened())
    {
        cerr << "Camera open failed!" << endl;
        return -1;
    }

    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(CAP_PROP_FRAME_WIDTH, 1080);
    cap.set(CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(CAP_PROP_FPS, 30);

    Mat book = imread(folderPath + "opencv_book.jpg");
    if (book.empty())
    {
        cerr << "Image load failed!" << endl;
        return -1;
    }
    Mat book_gray, desc1, desc2;
    cvtColor(book, book_gray, COLOR_BGR2GRAY);

    // 변수 준비
    Ptr<Feature2D> feature = ORB::create();
    Ptr<DescriptorMatcher> matcher = BFMatcher::create(NORM_HAMMING);
    vector<KeyPoint> keypoints1, keypoints2;
    vector<DMatch> matches;
    vector<Point2f> pts1, pts2, corners1, corners2;
    vector<Point> corners2_int;
    corners1.push_back(Point2f(0, 0));
    corners1.push_back(Point2f(book_gray.cols - 1.f, 0));
    corners1.push_back(Point2f(book_gray.cols - 1.f, book_gray.rows - 1.f));
    corners1.push_back(Point2f(0, book_gray.rows - 1.f));

    // book 조사
    feature->detectAndCompute(book_gray, Mat(), keypoints1, desc1);

    Mat img, img_gray, dst, M;
    while (true)
    {
        cap >> img;
        if (img.empty())
        {
            cerr << "Frame load failed!" << endl;
            break;
        }
        
        cvtColor(img, img_gray, COLOR_BGR2GRAY);
        feature->detectAndCompute(img_gray, Mat(), keypoints2, desc2);
        matcher->match(desc1, desc2, matches); // 매칭 수행

        sort(matches.begin(), matches.end());
        vector<DMatch> good_matches(matches.begin(), matches.begin() + 50);
        drawMatches(book, keypoints1, img, keypoints2, good_matches, dst, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

        for (size_t i = 0; i < good_matches.size(); ++i)
        {
            pts1.push_back(keypoints1[good_matches[i].queryIdx].pt);
            pts2.push_back(keypoints2[good_matches[i].trainIdx].pt);
        }
        M = findHomography(pts1, pts2, RANSAC); // getperspectiveTransform -> M 점 4+4
        perspectiveTransform(corners1, corners2, M);
        for (auto &p : corners2)
            corners2_int.push_back(Point(cvRound(p.x), cvRound(p.y)));
        polylines(img, corners2_int, true, Color::Red, 2, LINE_AA);
        corners2_int.clear();
        pts1.clear();
        pts2.clear();

        imshow("dst", dst);
        imshow("img", img);
        if (waitKey(33) == 27)
            break;
    }
    destroyAllWindows();
    return 0;
}