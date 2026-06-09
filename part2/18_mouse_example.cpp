// 마우스를 따라다니는 사각형 박스 만들기
// 마우스 오른쪽 버튼을 누르면 사각형 박스의 색깔이 랜덤으로 변경되도록 구현

#include "../part1/colors.hpp"
#include <opencv2/opencv.hpp>
#include <random>

using namespace cv;
using namespace std;

static Point mousePos(100,100);
static Scalar boxColor = Color::Blue;
static std::mt19937 rng((unsigned)time(nullptr));

void onMouse(int event, int x, int y, int flags, void* userdata)
{
	// 항상 마우스 위치를 저장해서 박스가 따라오게 함
	mousePos = Point(x, y);

	if (event == EVENT_RBUTTONDOWN)
	{
		// 오른쪽 버튼 클릭 -> 랜덤 색상으로 변경
		uniform_int_distribution<int> dist(0, 255);
		boxColor = Scalar(dist(rng), dist(rng), dist(rng));
	}
}

int main()
{
	const int width = 800;
	const int height = 600;
	Mat canvas(height, width, CV_8UC3, Color::White);

	namedWindow("mouse_box", WINDOW_AUTOSIZE);
	setMouseCallback("mouse_box", onMouse, nullptr);

	const Size boxSize(140, 90);

	while (true)
	{
		// 배경 초기화
		canvas.setTo(Color::White);

		// 박스의 좌상단 계산 (마우스가 박스 중앙에 위치하도록)
		Point tl(mousePos.x - boxSize.width/2, mousePos.y - boxSize.height/2);
		Point br = tl + Point(boxSize.width, boxSize.height);

		// 화면 밖으로 나가지 않게 클램프
		if (tl.x < 0) { tl.x = 0; br.x = boxSize.width; }
		if (tl.y < 0) { tl.y = 0; br.y = boxSize.height; }
		if (br.x > canvas.cols) { br.x = canvas.cols; tl.x = canvas.cols - boxSize.width; }
		if (br.y > canvas.rows) { br.y = canvas.rows; tl.y = canvas.rows - boxSize.height; }

		// 채워진 사각형(테두리 포함)
		rectangle(canvas, Rect(tl, br), boxColor, FILLED, LINE_AA);
		rectangle(canvas, Rect(tl, br), Scalar(200,200,200), 3, LINE_AA);

		imshow("mouse_box", canvas);
		int key = waitKey(16);
		if (key == 27) break; // ESC 종료
	}

	destroyAllWindows();
	return 0;
}
