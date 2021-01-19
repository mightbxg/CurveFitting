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
    bool operator()(const T* const m, const T* const c, const T* const d, T* residual) const
    {
        residual[0] = y_ - exp(m[0] * x_ + c[0]) - d[0];
        return true;
    }

private:
    const double x_;
    const double y_;
};

void fitCurve(const vector<Point>& pts, double& p1, double& p2, double& p3)
{
    using namespace ceres;
    Problem problem;
    for (const auto& pt : pts) {
        CostFunction* cost_function = new AutoDiffCostFunction<ExponentialResidual, 1, 1, 1, 1>(
            new ExponentialResidual(pt.x, pt.y));
        problem.AddResidualBlock(cost_function, nullptr, { &p1, &p2, &p3 });
    }

    Solver::Options options;
    options.max_num_iterations = 25;
    options.linear_solver_type = ceres::DENSE_QR;

    Solver::Summary summary;
    Solve(options, &problem, &summary);
    cout << summary.BriefReport() << endl;
    cout << "curve params: " << p1 << " " << p2 << " " << p3 << endl;
}

int main(int argc, char* argv[])
{
    WhiteBoard wb("white_board", 800, 600);

    double p1 = 0.01, p2 = 0, p3 = 0;
    auto getY = [&](double x) -> double {
        return exp(p1 * x + p2) + p3;
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
            fitCurve(wb.pts(), p1, p2, p3);
    }
    return 0;
}
