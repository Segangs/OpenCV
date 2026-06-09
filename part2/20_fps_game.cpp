#include "../part1/colors.hpp"
#include <opencv2/opencv.hpp>
#include <random>
#include <vector>
#include <string>
#include <chrono>

using namespace cv;
using namespace std;

struct Target {
    Point2f pos;
    Point2f vel;
    float radius;
    bool alive;
    int respawnTimer; // frames until respawn
    int type; // 0 = smiley, 1 = person, 2 = alien
    Scalar color;
    int hitEffect;
};

static vector<Target> targets;
static Point mousePos(0,0);
static int score = 0;
static bool fired = false;

static mt19937 rng((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());

void spawnTarget(int w, int h, Target &t) {
    uniform_real_distribution<float> xd(100.0f, w-100.0f);
    uniform_real_distribution<float> yd(100.0f, h-220.0f);
    uniform_real_distribution<float> vd(-3.0f, 3.0f);
    uniform_real_distribution<float> rd(24.0f, 40.0f);
    uniform_int_distribution<int> typeDist(0, 2);
    uniform_int_distribution<int> colDist(70, 255);
    t.pos = Point2f(xd(rng), yd(rng));
    t.vel = Point2f(vd(rng), vd(rng));
    if (abs(t.vel.x) < 0.5f) t.vel.x = 1.0f;
    if (abs(t.vel.y) < 0.5f) t.vel.y = 1.2f;
    t.radius = rd(rng);
    t.type = typeDist(rng);
    if (t.type == 0) {
        t.color = Scalar(colDist(rng), colDist(rng), colDist(rng));
    } else if (t.type == 1) {
        t.color = Scalar(20, 20, 20);
    } else {
        t.color = Scalar(30, 200, 50);
    }
    t.alive = true;
    t.respawnTimer = 0;
    t.hitEffect = 0;
}

void onMouse(int event, int x, int y, int flags, void* userdata) {
    mousePos = Point(x,y);
    if (event == EVENT_LBUTTONDOWN) {
        fired = true;
    }
}

int main(){
    const int WIDTH = 1280;
    const int HEIGHT = 960;
    Mat canvas(HEIGHT, WIDTH, CV_8UC3, Color::Black);

    namedWindow("FPS Shooter", WINDOW_AUTOSIZE);
    setMouseCallback("FPS Shooter", onMouse, nullptr);

    // initialize targets
    const int NUM = 6;
    targets.resize(NUM);
    for (int i=0;i<NUM;i++) spawnTarget(WIDTH, HEIGHT, targets[i]);

    int frames = 0;
    const int FPS = 60;
    const int GAME_TIME = 60; // seconds
    int totalFrames = GAME_TIME * FPS;

    auto start = chrono::steady_clock::now();

    while (true) {
        // clear
        canvas.setTo(Color::Black);

        // update targets
        for (auto &t : targets) {
            if (!t.alive) {
                if (t.respawnTimer > 0) t.respawnTimer--;
                if (t.respawnTimer == 0) spawnTarget(WIDTH, HEIGHT, t);
                continue;
            }
            t.pos += t.vel;
            // bounce
            if (t.pos.x - t.radius < 0 || t.pos.x + t.radius > WIDTH) t.vel.x *= -1;
            if (t.pos.y - t.radius < 0 || t.pos.y + t.radius > HEIGHT) t.vel.y *= -1;

            float headR = t.radius * 0.5f;
            Point2f headCenter = Point2f(t.pos.x, t.pos.y);
            if (t.type == 0) {
                circle(canvas, headCenter, int(headR), t.color, FILLED, LINE_AA);
                circle(canvas, Point(int(headCenter.x - headR*0.28f), int(headCenter.y - headR*0.15f)), int(max(2.0f, headR*0.12f)), Scalar(0,0,0), FILLED, LINE_AA);
                circle(canvas, Point(int(headCenter.x + headR*0.28f), int(headCenter.y - headR*0.15f)), int(max(2.0f, headR*0.12f)), Scalar(0,0,0), FILLED, LINE_AA);
                ellipse(canvas, headCenter + Point2f(0.0f, headR*0.12f), Size(int(headR*0.45f), int(headR*0.35f)), 0.0, 200.0, 340.0, Scalar(0,0,0), int(max(2.0f, headR*0.08f)), LINE_AA);
                circle(canvas, headCenter, int(headR), Scalar(220,220,220), 1, LINE_AA);
            } else if (t.type == 1) {
                circle(canvas, headCenter, int(headR), Scalar(20,20,20), FILLED, LINE_AA);
                Point shoulderL(int(t.pos.x - headR*0.7f), int(headCenter.y + headR*0.5f));
                Point shoulderR(int(t.pos.x + headR*0.7f), int(headCenter.y + headR*0.5f));
                Point waistL(int(t.pos.x - headR*0.25f), int(headCenter.y + headR*1.2f));
                Point waistR(int(t.pos.x + headR*0.25f), int(headCenter.y + headR*1.2f));
                Point hipL(int(t.pos.x - headR*0.38f), int(headCenter.y + headR*1.7f));
                Point hipR(int(t.pos.x + headR*0.38f), int(headCenter.y + headR*1.7f));
                vector<Point> body = {shoulderL, waistL, hipL, hipR, waistR, shoulderR};
                fillConvexPoly(canvas, body.data(), int(body.size()), Scalar(30,30,30), LINE_AA);
                line(canvas, shoulderL, Point(int(t.pos.x - headR*1.0f), int(headCenter.y + headR*0.8f)), Scalar(30,30,30), int(max(4.0f, headR*0.15f)), LINE_AA);
                line(canvas, shoulderR, Point(int(t.pos.x + headR*1.0f), int(headCenter.y + headR*0.8f)), Scalar(30,30,30), int(max(4.0f, headR*0.15f)), LINE_AA);
                line(canvas, waistL, Point(int(t.pos.x - headR*0.4f), int(headCenter.y + headR*2.4f)), Scalar(30,30,30), int(max(4.0f, headR*0.15f)), LINE_AA);
                line(canvas, waistR, Point(int(t.pos.x + headR*0.4f), int(headCenter.y + headR*2.4f)), Scalar(30,30,30), int(max(4.0f, headR*0.15f)), LINE_AA);
                circle(canvas, headCenter, int(headR), Scalar(200,200,200), 2, LINE_AA);
            } else {
                circle(canvas, headCenter, int(headR), t.color, FILLED, LINE_AA);
                Point eyeL(int(headCenter.x - headR*0.25f), int(headCenter.y - headR*0.2f));
                Point eyeR(int(headCenter.x + headR*0.25f), int(headCenter.y - headR*0.2f));
                circle(canvas, eyeL, int(max(2.0f, headR*0.1f)), Scalar(255,255,255), FILLED, LINE_AA);
                circle(canvas, eyeR, int(max(2.0f, headR*0.1f)), Scalar(255,255,255), FILLED, LINE_AA);
                circle(canvas, eyeL, int(max(1.0f, headR*0.05f)), Scalar(0,0,0), FILLED, LINE_AA);
                circle(canvas, eyeR, int(max(1.0f, headR*0.05f)), Scalar(0,0,0), FILLED, LINE_AA);
                ellipse(canvas, headCenter + Point2f(0.0f, headR*0.2f), Size(int(headR*0.35f), int(headR*0.25f)), 0.0, 0.0, 360.0, Scalar(255,255,255), int(max(3.0f, headR*0.12f)), LINE_AA);
                line(canvas, Point(int(headCenter.x - headR*0.28f), int(headCenter.y + headR*0.3f)), Point(int(headCenter.x + headR*0.28f), int(headCenter.y + headR*0.3f)), Scalar(0,0,0), int(max(3.0f, headR*0.12f)), LINE_AA);
            }
            if (t.hitEffect > 0) {
                int effect = t.hitEffect;
                float ratio = float(effect) / 12.0f;
                circle(canvas, headCenter, int(headR + ratio * 30.0f), Scalar(255,255,255), int(max(1.0f, 4.0f - ratio*3.0f)), LINE_AA);
                t.hitEffect--;
            }
        }

        // show crosshair at mouse
        Scalar chColor(0,220,220);
        int cx = mousePos.x, cy = mousePos.y;
        line(canvas, Point(cx-18, cy), Point(cx+18, cy), chColor, 2, LINE_AA);
        line(canvas, Point(cx, cy-18), Point(cx, cy+18), chColor, 2, LINE_AA);
        circle(canvas, Point(cx,cy), 6, chColor, 1, LINE_AA);

        // handle firing
        if (fired) {
            // check hits
            for (auto &t : targets) {
                if (!t.alive) continue;
                Point2f headCenter = Point2f(t.pos.x, t.pos.y);
                float headR = t.radius * 0.5f;
                float dx = headCenter.x - mousePos.x;
                float dy = headCenter.y - mousePos.y;
                float dist2 = dx*dx + dy*dy;
                if (dist2 <= (headR * 1.05f) * (headR * 1.05f)) {
                    int bonus = 5;
                    if (t.type == 1) bonus = 8;
                    if (t.type == 2) bonus = 7;
                    score += bonus;
                    t.alive = false;
                    t.respawnTimer = FPS * (2 + (rng()%3)); // 2-4 seconds
                    t.hitEffect = 12;
                }
            }
            // simple muzzle flash
            circle(canvas, Point(cx,cy), 18, Scalar(0,255,255), 2, LINE_AA);
            fired = false;
        }

        // HUD: score and time
        int elapsed = int( chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start).count() );
        int remaining = max(0, GAME_TIME - elapsed);
        string scoreText = "Score: " + to_string(score);
        string timeText = "Time: " + to_string(remaining) + "s";
        putText(canvas, scoreText, Point(20,40), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(230,230,230), 2, LINE_AA);
        putText(canvas, timeText, Point(WIDTH-200,40), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(230,230,230), 2, LINE_AA);

        imshow("FPS Shooter", canvas);
        int key = waitKey(1000 / FPS);
        if (key == 27) break;
        if (remaining == 0) break;
        frames++;
    }

    // end screen
    Mat end(360, 640, CV_8UC3, Scalar(12,12,24));
    string s = "Game Over";
    putText(end, s, Point(120,80), FONT_HERSHEY_SIMPLEX, 2.2, Scalar(240,240,255), 3, LINE_AA);
    string sc = "Your Score: " + to_string(score);
    putText(end, sc, Point(60,220), FONT_HERSHEY_SIMPLEX, 1.1, Scalar(200,220,255), 2, LINE_AA);
    imshow("FPS Shooter", end);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
