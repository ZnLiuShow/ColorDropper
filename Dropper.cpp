// Dropper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Rect imageRect; // 图像窗口的矩形区域;
// 鼠标回调函数
void onMouse(int event, int x, int y, int flags, void* param)
{
    Mat* image = (Mat*)param;

    if (event == EVENT_MOUSEMOVE || event == EVENT_LBUTTONDOWN) // 检查是否是鼠标移动或左键单击
    {
        // 检查鼠标是否在图像窗口内
        bool isInside = (x >= imageRect.x && x < imageRect.x + imageRect.width &&
            y >= imageRect.y && y < imageRect.y + imageRect.height);
        if (!isInside) {
            return;
        }
        // 定义5x5的区域
        int halfSize = 5;
        int roi_size = 11;
        Rect roi(x - halfSize, y - halfSize, roi_size, roi_size);

        // 创建一个5x5的黑色背景图像
        Mat smallRegion = Mat::zeros(roi_size, roi_size, image->type());

        // 计算有效区域的边界
        int startX = max(0, x - halfSize);
        int startY = max(0, y - halfSize);
        int endX = min(image->cols, x + halfSize + 1);
        int endY = min(image->rows, y + halfSize + 1);

        // 计算在smallRegion中的偏移量
        int offsetX = startX - (x - halfSize);
        int offsetY = startY - (y - halfSize);

        // 复制有效区域到smallRegion
        Mat validRegion = (*image)(Rect(startX, startY, endX - startX, endY - startY));
        validRegion.copyTo(smallRegion(Rect(offsetX, offsetY, validRegion.cols, validRegion.rows)));

        // 放大4倍
        Mat enlargedRegion;
        resize(smallRegion, enlargedRegion, Size(), 8, 8, INTER_LINEAR);

        // 绘制十字准星
        int centerX = enlargedRegion.cols / 2;
        int centerY = enlargedRegion.rows / 2;
        line(enlargedRegion, Point(centerX, 0), Point(centerX, enlargedRegion.rows - 1), Scalar(0, 0, 255), 1);
        line(enlargedRegion, Point(0, centerY), Point(enlargedRegion.cols - 1, centerY), Scalar(0, 0, 255), 1);

        // 显示放大后的区域
        imshow("Enlarged Region", enlargedRegion);

        // 如果是左键单击，输出HSV值
        if (event == EVENT_LBUTTONDOWN)
        {
            Vec3b color = (*image).at<Vec3b>(Point(x, y)); // 获取点击位置的颜色
            Mat hsvColor;
            cvtColor(Mat(1, 1, image->type(), &color), hsvColor, COLOR_BGR2HSV); // 转换为HSV颜色空间
            Vec3b hsv = hsvColor.at<Vec3b>(0, 0);
            cout << "HSV: H=" << (int)hsv[0] << " S=" << (int)hsv[1] << " V=" << (int)hsv[2] << endl;
        }
    }
   
}

int main(int argc, char** argv)
{
    Mat img;

    // 检查启动参数
    if (argc > 1)
    {
        // 拖拽文件启动
        string imagePath = argv[1];
        img = imread(imagePath);
    }
    else
    {
        // 固定名字的照片启动
        img = imread("2210.jpg");
    }

    if (img.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }
    // 获取图像窗口的矩形区域
    imageRect = Rect(0, 0, img.cols, img.rows);

    // 创建窗口
    namedWindow("Image", WINDOW_AUTOSIZE);
    //namedWindow("Enlarged Region", WINDOW_NORMAL);

    // 设置鼠标回调
    setMouseCallback("Image", onMouse, &img);

    // 显示图像
    imshow("Image", img);

    // 等待按键
    waitKey(0);

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
