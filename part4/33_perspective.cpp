#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

int main()
{
    const String folderPath = "/home/segang/segang/OpenCV/data/";
    Mat img = imread(folderPath + "jy.jpg");

    if (img.empty()) {
        cout << "이미지를 로드할 수 없습니다!" << endl;
        return -1;
    }

    // 결과물과 매핑 정보를 담을 행렬 생성
    Mat dst;
    Mat map_x(img.size(), CV_32FC1);
    Mat map_y(img.size(), CV_32FC1);

    // 물결 효과의 속성을 조절하는 변수들
    float amplitude = 10.0f; // 물결의 높이 (진폭)
    float frequency = 0.05f; // 물결의 주기 (주파수)
    float offset = 0.0f;     // 물결을 움직이게 할 오프셋 (시간 흐름)

    while (true) {
        // 이미지의 모든 픽셀을 돌며 새로운 좌표(물결) 계산
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                // X축 방향으로 Sine 함수를 적용해 Y축 좌표를 흔듬 (좌우 물결은 반대로)
                map_x.at<float>(i, j) = (float)j;
                map_y.at<float>(i, j) = (float)i + amplitude * sin((float)j * frequency + offset);
            }
        }

        // 재매핑 함수로 물결 효과 적용
        remap(img, dst, map_x, map_y, INTER_LINEAR, BORDER_REFLECT_101);

        // 화면 출력
        imshow("Wave Effect", dst);

        // 오프셋을 증가시켜 다음 프레임에서 물결이 이동하도록 함
        offset += 0.2f; 

        // 'q' 키를 누르거나 ESC(27)를 누르면 종료 (30ms 대기로 초당 약 33프레임 연출)
        int key = waitKey(30);
        if (key == 27 || key == 'q' || key == 'Q') {
            break;
        }
    }

    destroyAllWindows();
    return 0;
}