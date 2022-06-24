#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat loadImg();
vector<Vec3f> hough_trans(Mat img);

int main()
{
    Scalar white = Scalar(255, 255, 255),
           black = Scalar(0, 0, 0),
           red = Scalar(0, 0, 255);
    
    Mat img = loadImg(),
        wheel(img.size(), 0, white),
        mask(img.size(), 0, black);
    
    // MARK: 허프 변환을 사용하여 휠 검출
    // 허프 변환 사용 시 사진 상의 휠 크기에 따라 검출이 안되는 경우가 발생
    // 이미지 전처리를 통해 크기를 일정하게 해야함
    vector<Vec3f> circles;
    circles = hough_trans(img);
    
    Vec3i c = circles[0];
    Point center(c[0] - 2, c[1]);
    int circle_radius = c[2],
        rect_radius = 0;
    
    cout << c[0] << " " << c[1] << " " << c[2] << " " << c[3] << endl;

    for(int i = 0; i <= img.rows; ++i) {
        for(int j = 0; j <= img.cols; ++j) {
            rect_radius = sqrt(pow((i - center.x), 2) + pow((j - center.y), 2));

            // MARK: 허프 변환에서 검출한 휠의 반지름보다 큰 경우 white, 작은 경우 기존 휠의 Scalar값을 점으로 찍음
            if(circle_radius <= rect_radius) {
                line(wheel, Point(i, j), Point(i, j), white);
            }
            else if(circle_radius > rect_radius && rect_radius >= 0) {
                line(wheel, Point(i, j), Point(i, j), img.at<uchar>(i, j));
            }
        }
    }

    Rect bbox(c[0] - c[2] - 2, c[1] - c[2], 2 * c[2], 2 * c[2]);
    wheel = wheel(bbox);

    imwrite("wheel00002.png", wheel);

    imshow("Image", img);
    imshow("wheel", wheel);
    waitKey(0);
    
    return 0;
}

vector<Vec3f> hough_trans(Mat img)
{
    vector<Vec3f> circles;
    
    HoughCircles(img, circles, HOUGH_GRADIENT, 1, 150, 700, 100, 0, 220);
    
    return circles;
}

// TODO: 모든 이미지 불러오기
Mat loadImg()
{
    string fname = format("../Assets/wheel00002.PNG");
    
    Mat img = imread(fname, 0);
    
    return img;
}
