#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
    const String folderPath = "/home/segang/segang/OpenCV/data/";
    Mat img = imread(folderPath + "lenna.bmp");
    vector<Mat> images;

    Mat dx, dy;
    Sobel(img, dx, CV_32FC1, 1, 0);
    Sobel(img, dx, CV_32FC1, 1, 0);

    Mat magnitudeM, phaseM;
    magnitude(dx, dy, magnitudeM);
    phase(dx, dy, phaseM, true);

    dx.convertTo(dx, CV_8UC1);
    images.push_back(dx);
    dx.convertTo(dy, CV_8UC1);
    images.push_back(dy);
    magnitudeM.convertTo(magnitudeM, CV_8UC1);
    images.push_back(magnitudeM);
    phaseM.convertTo(phaseM, CV_8UC1);
    images.push_back(phaseM);
    Mat edge = magnitudeM > 150;
    images.push_back(edge);

    int i = 1;
    for (auto img : images)

    imshow("img", img);
    imshow("dst", dst);

    waitKey();
    destroyAllWindows();
    return 0;
}