#include "../part1/colors.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

String folderPath = "/home/segang/segang/OpenCV/data/";

void on_level_change(int pos, void *data);

int main()
{
    const String folderPath = "/home/segang/segang/OpenCV/data/";
    String name = "Segang";
    int age = 18;
    Point pt1(100, 200);
    vector<float> scores = {3.14, 6.66, 9.14};
    Mat mat1 = (Mat_<float>(2,2) << 1.0f, 1.5f, 2.0f, 3.2f);
    
FileStorage fs;
fs.open(folderPath + "mydata.xml", FileStorage::WRITE);
fs << "name" << name;
fs << "age" << age;
fs << "Point" << scores;
fs << "data" << mat1;

fs.release();

    return 0;
}
