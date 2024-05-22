#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<cv::Point2f> control_points;

void mouse_handler(int event, int x, int y, int flags, void *userdata)
{
    if (event == cv::EVENT_LBUTTONDOWN && control_points.size() < 4)
    {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", "
                  << y << ")" << '\n';
        control_points.emplace_back(x, y);
    }
}

void naive_bezier(const std::vector<cv::Point2f> &points, cv::Mat &window)
{
    auto &p_0 = points[0];
    auto &p_1 = points[1];
    auto &p_2 = points[2];
    auto &p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001)
    {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                     3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
    }
}

cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points, float t)
{
    // TODO: Implement de Casteljau's algorithm
    std::vector<cv::Point2f> tmp_points;
    for (auto k : control_points)
    {
        tmp_points.push_back(k);
    }

    while (tmp_points.size() != 1)
    {
        for (int i = 0; i < tmp_points.size() - 1; i++)
        {
            auto a = tmp_points[i];
            auto b = tmp_points[i + 1];
            auto x = a.x + t * (b.x - a.x);
            auto y = a.y + t * (b.y - a.y);
            cv::Point2f c(x, y);
            tmp_points[i] = c;
            if (i == tmp_points.size() - 2)
            {
                tmp_points.pop_back();
            }
        }
    }
    return tmp_points[0];
}

void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window)
{
    // TODO: Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's
    // recursive Bezier algorithm.

    for (double t = 0; t <= 1.0; t += 0.0001)
    {
        auto p = recursive_bezier(control_points, t);
        window.at<cv::Vec3b>(p.y, p.x)[1] = 255;
    }
}

void bezier2(const std::vector<cv::Point2f> &control_points, cv::Mat &window)
{
    // TODO: Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's
    // recursive Bezier algorithm.

    // 反走样
    for (double t = 0; t <= 1.0; t += 0.0001)
    {
        auto p = recursive_bezier(control_points, t);
        window.at<cv::Vec3b>(p.y, p.x)[1] = 255;
        float dx = p.x - std::floor(p.x);
        float dy = p.y - std::floor(p.y);
        float kx = dx < 0.5f ? -1.0 : 1.0;
        float ky = dy < 0.5f ? -1.0 : 1.0;
        auto p0 = cv::Point2f(std::floor(p.x) + 0.5f, std::floor(p.y) + 0.5f);
        auto p1 = cv::Point2f(std::floor(p.x + kx * 1.0f) + 0.5f, std::floor(p.y) + 0.5f);
        auto p2 = cv::Point2f(std::floor(p.x) + 0.5f, std::floor(p.y + ky * 1.0) + 0.5f);
        auto p3 = cv::Point2f(std::floor(p.x + kx * 1.0f) + 0.5f, std::floor(p.y + ky * 1.0) + 0.5f);

        std::vector<cv::Point2f> points;
        // points.push_back(p0);
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);

        float d0 = std::sqrt(std::pow(p.x - p0.x, 2) + std::pow(p.y - p0.y, 2));
        for (auto &point : points)
        {
            auto d = std::sqrt(std::pow(point.x - p.x, 2) + std::pow(point.y - p.y, 2));
            auto ratio = d0 / d;
            auto color = window.at<cv::Vec3b>(point.y, point.x)[1];
            color = std::fmax(color, ratio * 255.0);
            window.at<cv::Vec3b>(point.y, point.x)[1] = (int)color;
        }
    }
}

int main()
{
    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);

    int key = -1;
    while (key != 27)
    {
        for (auto &point : control_points)
        {
            cv::circle(window, point, 3, {255, 255, 255}, 3);
        }

        if (control_points.size() == 4)
        {
            // naive_bezier(control_points, window);
            bezier2(control_points, window);

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

    return 0;
}
