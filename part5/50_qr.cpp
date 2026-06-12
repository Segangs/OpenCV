// sudo apt install -y libzbar-dev

#include "opencv2/opencv.hpp"
#include <iostream>
#include <zbar.h>

using namespace cv;
using namespace std;
using namespace zbar;

const String folderPath = "/home/segang/segang/OpenCV/data/";
void decode_qrcode();

int main(void)
{
    decode_qrcode();

    return 0;
}

void decode_qrcode()
{
    VideoCapture cap(0, CAP_V4L2);

    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(CAP_PROP_FPS, 30);

    if (!cap.isOpened())
    {
        cerr << "Camera open failed!" << endl;
        return;
    }

    ImageScanner scanner;

    // QR 코드만 스캔하도록 설정
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 0);
    scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);

    Mat frame, gray;
    while (true)
    {
        cap >> frame;

        if (frame.empty())
        {
            cerr << "Frame load failed!" << endl;
            break;
        }

        cvtColor(frame, gray, COLOR_BGR2GRAY);

        Image zbarImage(
            gray.cols,
            gray.rows,
            "Y800",
            gray.data,
            gray.cols * gray.rows);

        int n = scanner.scan(zbarImage);

        if (n > 0)
        {
            for (Image::SymbolIterator symbol = zbarImage.symbol_begin();
                 symbol != zbarImage.symbol_end();
                 ++symbol)
            {
                string type = symbol->get_type_name();
                string data = symbol->get_data();

                vector<Point> points;

                for (int i = 0; i < symbol->get_location_size(); i++)
                {
                    points.push_back(Point(
                        symbol->get_location_x(i),
                        symbol->get_location_y(i)));
                }

                if (!points.empty())
                {
                    polylines(frame, points, true, Scalar(0, 0, 255), 2);
                }

                putText(frame,
                        type + ": " + data,
                        Point(10, 30),
                        FONT_HERSHEY_DUPLEX,
                        0.8,
                        Scalar(0, 0, 255),
                        2);

                cout << "Type: " << type << endl;
                cout << "Data: " << data << endl;
            }
        }

        // ZBar가 gray.data를 소유하지 않도록 데이터 해제 연결 끊기
        zbarImage.set_data(nullptr, 0);

        imshow("frame", frame);

        if (waitKey(1) == 27)
            break;
    }

    cap.release();
    destroyAllWindows();
}