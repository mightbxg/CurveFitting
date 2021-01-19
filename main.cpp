#include "curve.h"
#include "white_board.h"
#include <ceres/ceres.h>
#include <iostream>

using namespace std;
using namespace cv;

struct ExponentialResidual {
    ExponentialResidual(double x, double y)
        : x_(x)
        , y_(y)
    {
    }
    template <typename T>
    bool operator()(const T* const p, T* residual) const
    {
        residual[0] = y_ - exp(p[0] * x_ + p[1]) - p[2];
        return true;
    }

private:
    const double x_;
    const double y_;
};

void fitCurve(const vector<Point>& pts, double p[3])
{
    using namespace ceres;
    Problem problem;
    for (const auto& pt : pts) {
        CostFunction* cost_function = new AutoDiffCostFunction<ExponentialResidual, 1, 3>(
            new ExponentialResidual(pt.x, pt.y));
        problem.AddResidualBlock(cost_function, nullptr, p);
    }

    Solver::Options options;
    options.max_num_iterations = 25;
    options.linear_solver_type = ceres::DENSE_QR;

    Solver::Summary summary;
    Solve(options, &problem, &summary);
    cout << summary.BriefReport() << endl;
    cout << "curve params: " << p[0] << " " << p[1] << " " << p[2] << endl;
}

int main(int argc, char* argv[])
{
    WhiteBoard wb("white_board", 800, 600);

    double p[3] = { 0.01, 0, 0 };
    auto getY = [&](double x) -> double {
        return exp(p[0] * x + p[1]) + p[2];
    };
    auto draw = [&](cv::Mat& img) {
        constexpr double y_thresh = 1e5;
        double prev_y = getY(-1);
        for (int i = 0; i < 800; ++i) {
            double crt_y = getY(i);
            if (abs(prev_y) < y_thresh && abs(crt_y) < y_thresh)
                line(img, { i - 1, cvRound(prev_y) }, { i, cvRound(crt_y) }, { 255, 0, 0 }, 1);
            prev_y = crt_y;
        }
    };

    wb.setupAdditionalDrawing(draw);
    wb.show();

    while (true) {
        wb.updateView();
        int ckey = waitKey(0);

        if (ckey == 'q')
            break;
        else if (ckey == 'c')
            wb.clear();
        else if (ckey == 'f')
            fitCurve(wb.pts(), p);
    }
    return 0;
}
