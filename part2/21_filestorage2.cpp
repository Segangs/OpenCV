#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void on_level_change(int pos, void *data);

int main()
{
    const String folderPath = "/home/segang/segang/OpenCV/data/";
    String name;
    int age;
    Point pt1;
    vector<float> scores;
    Mat mat1;

    FileStorage fs;
    fs.open(folderPath + "mydata.xml", FileStorage::READ);
    fs["name"] >> name;
    fs["age"] >> age;
    fs["Point"] >> pt1;
    fs["scores"] >> scores;
    fs["data"] >> mat1;

    cout << name << age << pt1 << Mat(scores).t() << mat1 << endl;

    fs.release();

    return 0;
}
