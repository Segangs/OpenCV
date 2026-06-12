#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

const String folderPath = "/home/segang/segang/OpenCV/data/";

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

    CascadeClassifier face_classifier(folderPath + "haarcascade_frontalface_default.xml");
    CascadeClassifier eye_classifier(folderPath + "haarcascade_eye.xml");

    if (face_classifier.empty() || eye_classifier.empty())
    {
        cerr << "XML load failed!" << endl;
        return -1;
    }

    Mat frame;

    while (true)
    {
        cap >> frame;

        if (frame.empty())
        {
            cerr << "프레임을 읽을 수 없습니다." << endl;
            break;
        }

        vector<Rect> faces;
        face_classifier.detectMultiScale(frame, faces);

        for (Rect face : faces)
        {
            rectangle(frame, face, Scalar(255, 0, 255), 2);

            Mat faceROI = frame(face);

            vector<Rect> eyes;
            eye_classifier.detectMultiScale(faceROI, eyes);

            for (Rect eye : eyes)
            {
                Point center(
                    eye.x + eye.width / 2,
                    eye.y + eye.height / 2);

                circle(
                    faceROI,
                    center,
                    eye.width / 2,
                    Scalar(255, 0, 0),
                    2,
                    LINE_AA);
            }
        }

        imshow("frame", frame);

        if (waitKey(30) == 27)
            break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}