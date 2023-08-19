#include <iostream>
#include <Eigen/Dense>
#include "image/image.h"

using namespace std;
using namespace Eigen;
using namespace jeffb::cv::image;

int main() {
    Image img1;
    cout << "Uninitialized Image :-" << endl;
    auto i = img1.get_image();
    auto d = img1.get_dims();
    cout << "Dims: " << d[0] << " x " << d[1] << ", image dimensions: " << i.rows() << " x " << i.cols() << endl << endl;

    cout << "Loading image :-" << endl;
    if (!img1.load("./Square.png")) {
        cout << "failed to load image" << endl;
        return 1;
    }
    Array<uint8_t, Dynamic, Dynamic> b = img1.get_image();
    d = img1.get_dims();
    cout << "Dims: " << d[0] << " x " << d[1] << ", image dimensions: " << b.rows() << " x " << b.cols() << endl;
    cout << b << endl;

    return 0;
}