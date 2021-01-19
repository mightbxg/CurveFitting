#include "white_board.h"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    WhiteBoard wb("white_board", 800, 600);
    wb.show();

    waitKey(0);
    return 0;
}
