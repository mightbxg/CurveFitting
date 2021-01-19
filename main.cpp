#include "curve.hpp"
#include "white_board.h"
#include <ceres/ceres.h>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    WhiteBoard wb("white_board", 800, 600);
    using CurveType = Line;
    CurveType c(1.0, 0.0);
    wb.setupAdditionalDrawing(bind(&CurveType::draw, &c, placeholders::_1));
    wb.show();

    while (true) {
        wb.updateView();
        int ckey = waitKey(0);

        if (ckey == 'q')
            break;
        else if (ckey == 'c')
            wb.clear();
        else if (ckey == 'f')
            c.fit(wb.pts());
    }
    return 0;
}
