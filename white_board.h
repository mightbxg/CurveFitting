#ifndef ___WHITE_BOARD_H
#define ___WHITE_BOARD_H

#include <opencv2/opencv.hpp>

class WhiteBoard {
public:
    WhiteBoard(const std::string& name, int width, int height, cv::Scalar bg = cv::Scalar::all(255));
    void addPoint(int x, int y)
    {
        pts_.emplace_back(x, y);
    }
    cv::Mat image() const;
    void show();
    void updateView() const;
    void clear()
    {
        pts_.clear();
        show();
    }
    const std::vector<cv::Point>& pts() const { return pts_; }

private:
    std::string name_;
    cv::Mat background_;
    std::vector<cv::Point> pts_;
};

#endif // ___WHITE_BOARD_H
