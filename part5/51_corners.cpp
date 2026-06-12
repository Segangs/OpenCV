#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
const String folderPath = "/home/segang/segang/OpenCV/data/";

void corner_harris();
void corner_fast();
void good_features_to_track();

int main(void)
{
    corner_harris();
    corner_fast();
    good_features_to_track();

    return 0;
}

void corner_harris()
{
    Mat src = imread(folderPath + "IMG_1030.jpg", IMREAD_GRAYSCALE);

    if (src.empty())
    {
        cerr << "Image load failed!" << endl;
        return;
    }

    // 가로 1280으로 고정, 세로는 원본 비율 유지
    resize(src, src, Size(1280, src.rows * 1280 / src.cols));

    Mat harris;
    cornerHarris(src, harris, 3, 3, 0.04);

    Mat harris_norm;
    normalize(harris, harris_norm, 0, 255, NORM_MINMAX, CV_8U);

    Mat dst;
    cvtColor(src, dst, COLOR_GRAY2BGR);

    for (int j = 1; j < harris.rows - 1; j++)
    {
        for (int i = 1; i < harris.cols - 1; i++)
        {
            if (harris_norm.at<uchar>(j, i) > 120)
            {
                if (harris.at<float>(j, i) > harris.at<float>(j - 1, i) &&
                    harris.at<float>(j, i) > harris.at<float>(j + 1, i) &&
                    harris.at<float>(j, i) > harris.at<float>(j, i - 1) &&
                    harris.at<float>(j, i) > harris.at<float>(j, i + 1))
                {
                    circle(dst, Point(i, j), 5, Scalar(0, 0, 255), 2);
                }
            }
        }
    }

    imshow("src", src);
    imshow("harris_norm", harris_norm);
    imshow("dst", dst);

    waitKey();
    destroyAllWindows();
}

void corner_fast()
{
    Mat src = imread(folderPath + "IMG_1030.jpg", IMREAD_GRAYSCALE);

    if (src.empty())
    {
        cerr << "Image load failed!" << endl;
        return;
    }

    // 가로 1280으로 고정, 세로는 원본 비율 유지
    resize(src, src, Size(1280, src.rows * 1280 / src.cols));

    vector<KeyPoint> keypoints;
    FAST(src, keypoints, 60, true);

    Mat dst;
    cvtColor(src, dst, COLOR_GRAY2BGR);

    for (KeyPoint kp : keypoints)
    {
        Point pt(cvRound(kp.pt.x), cvRound(kp.pt.y));
        circle(dst, pt, 5, Scalar(0, 0, 255), 2);
    }

    imshow("src", src);
    imshow("dst", dst);

    waitKey();
    destroyAllWindows();
}
struct USER_DATA
{
    vector<Point2f> *corners;
    int *maxCorners;
    Mat *src;
};

void on_trackbar(int, void *user_data);

void good_features_to_track()
{
    Mat src = imread(folderPath + "IMG_1030.jpg", IMREAD_GRAYSCALE);

    if (src.empty())
    {
        cerr << "Image load failed!" << endl;
        return;
    }

    // 가로 1280으로 고정, 세로는 원본 비율 유지
    resize(src, src, Size(1280, src.rows * 1280 / src.cols));

    vector<Point2f> corners;
    // set maxCorners to tracbar
    int maxCorners = 300;
    USER_DATA user_data;
    user_data.src = &src;
    user_data.corners = &corners;
    user_data.maxCorners = &maxCorners;
    Mat dst;

    namedWindow("dst");
    createTrackbar("maxCorners", "dst", &maxCorners, 1000, on_trackbar, &user_data);
    setTrackbarPos("maxCorners", "dst", maxCorners);

    on_trackbar(0, &user_data);
    imshow("src", src);

    waitKey();
    destroyAllWindows();
}

void on_trackbar(int a, void *user)
{
    USER_DATA *_user_data = reinterpret_cast<USER_DATA *>(user);
    goodFeaturesToTrack(*_user_data->src, *_user_data->corners, *_user_data->maxCorners, 0.01, 10);
    Mat dst;
    cvtColor(*_user_data->src, dst, COLOR_GRAY2BGR);

    for (Point2f pt : *_user_data->corners)
    {
        circle(dst, pt, 5, Scalar(0, 0, 255), 2);
    }
    imshow("dst", dst);
    waitKey(3);
}