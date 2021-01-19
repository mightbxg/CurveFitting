#include "white_board.h"

using namespace cv;

static void onMouse(int event, int x, int y, int flags, void* board)
{
    auto bd = static_cast<WhiteBoard*>(board);
    if (event == EVENT_LBUTTONDOWN) {
        bd->addPoint(x, y);
    }
    bd->updateView();
}

WhiteBoard::WhiteBoard(const std::string& name, int width, int height, Scalar bg)
    : name_(name)
{
    CV_Assert(!name.empty());
    CV_Assert(width > 10 && height > 10);
    background_ = Mat(height, width, CV_8UC3, bg);
}

Mat WhiteBoard::image() const
{
    Mat image = background_.clone();
    for (const auto& pt : pts_)
        circle(image, pt, 2, { 0, 0, 255 }, cv::FILLED);
    additional_drawing_(image);
    return image;
}

void WhiteBoard::show()
{
    updateView();
    setMouseCallback(name_, onMouse, this);
}

void WhiteBoard::updateView() const
{
    imshow(name_, image());
}
