#ifndef ___CURVE_H
#define ___CURVE_H

#include <ceres/ceres.h>
#include <opencv2/opencv.hpp>

template <typename CurveType>
struct CurveResidual {
    CurveResidual(double x, double y)
        : x_(x)
        , y_(y)
    {
    }
    template <typename T>
    bool operator()(const T* const p, T* residual) const
    {
        residual[0] = T(y_) - CurveType::template getYImpl<T>(p, T(x_));
        return true;
    }

private:
    const double x_, y_;
};

template <unsigned P_Num, typename Derived>
class Curve {
public:
    template <typename... Ts>
    Curve(Ts... ps)
        : p { ps... }
    {
    }
    void setDrawingRange(double x_min, double x_max)
    {
        x_draw_min = x_min;
        x_draw_max = x_max;
    }
    void setDrawingColor(const cv::Scalar& color)
    {
        color_draw = color;
    }
    double getY(double x) const
    {
        return Derived::template getYImpl<double>(p, x);
    }
    void draw(cv::Mat& img) const
    {
        constexpr double y_thresh = 1e5;
        double prev_y = getY(x_draw_min - 1.);
        for (double x = x_draw_min; x < x_draw_max; x += 1.) {
            double crt_y = getY(x);
            if (abs(prev_y) < y_thresh && abs(crt_y) < y_thresh)
                cv::line(img, { cvRound(x - 1.), cvRound(prev_y) },
                    { cvRound(x), cvRound(crt_y) }, color_draw, 1);
            prev_y = crt_y;
        }
    }
    void fit(const std::vector<cv::Point>& pts)
    {
        using namespace ceres;
        Problem problem;
        for (const auto& pt : pts)
            problem.AddResidualBlock(createCostFunction(pt.x, pt.y), nullptr, p);
        Solver::Options options;
        options.max_num_iterations = 100;
        options.linear_solver_type = ceres::DENSE_QR;
        Solver::Summary summary;
        ceres::Solve(options, &problem, &summary);
        std::cout << summary.BriefReport() << "\n";
        std::cout << "curve params:";
        for (const auto& v : p)
            std::cout << " " << v;
        std::cout << std::endl;
    }

private:
    ceres::CostFunction* createCostFunction(double x, double y) const
    {
        using ResidualType = CurveResidual<Derived>;
        return new ceres::AutoDiffCostFunction<ResidualType, 1, P_Num>(
            new ResidualType(x, y));
    }

protected:
    double p[P_Num]; // params

    double x_draw_min { 0. };
    double x_draw_max { 1000. };
    cv::Scalar color_draw { 255, 0, 0 };
};

class ExpCurve : public Curve<3, ExpCurve> {
public:
    using Curve::Curve;
    template <typename T>
    static T getYImpl(const T* p, T x)
    {
        return ceres::exp(x * p[0] + p[1]) + p[2];
    }
};

#endif // ___CURVE_H
