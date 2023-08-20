#include <iostream>
#include <Eigen/Dense>
#include <fftw3.h>
#include "image/image.h"
#include <cmath>

using namespace std;
using namespace Eigen;
using namespace jeffb::cv::image;

const float PI = atanf(1.f) * 4.f;

int main() {
    cout << PI << endl;
    Image img1;
    cout << "Uninitialized Image :-" << endl;
    auto empty = img1.get_image();
    auto d = img1.get_dims();
    cout << "Dims: " << d[0] << " x " << d[1] << ", image dimensions: " << empty.rows() << " x " << empty.cols() << endl << endl;

    cout << "Loading image :-" << endl;
    if (!img1.load("./Square.png")) {
        cout << "failed to load image" << endl;
        return 1;
    }
    auto input_img = img1.get_image();
    d = img1.get_dims();
    cout << "Dims: " << d[0] << " x " << d[1] << ", image dimensions: " << input_img.rows() << " x " << input_img.cols() << endl;
    img1.save_png("t1.png");
    ArrayXXf x1 = input_img.cast<float>();

    auto w = d[0];
    auto h = d[1];
    auto N = w * h;

    auto in = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * N));
    auto dft = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * N));
    auto idft = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * N));
    auto plan_f = fftw_plan_dft_2d(w, h, in, dft, FFTW_FORWARD, FFTW_ESTIMATE);
    auto plan_b = fftw_plan_dft_2d(w, h, dft, idft, FFTW_BACKWARD, FFTW_ESTIMATE);

    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            auto k = i*w+j;
            in[k][0] = x1(i, j);
            in[k][1] = 0.0;
        }
    }

    fftw_execute(plan_f);
    fftw_execute(plan_b);

    ArrayXXd img2 = ArrayXXd::Zero(w, h);
    ArrayXXd img3 = ArrayXXd::Zero(w, h);
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            auto k = i*w+j;
            auto mag = sqrt(dft[k][0]*dft[k][0] + dft[k][1] * dft[k][1]);
            img2(i, j) = mag;
            img3(i,j) = atan(dft[k][1] / dft[k][0]);
            if (i == 6) {
                cout << dft[k][0] << " + i" << dft[k][1] << endl;
            }
        }
    }
    img2 *= (1.0/(w*h));
    cout << img3 << endl;

    ArrayXXd inverse = ArrayXXd::Zero(w, h);
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            auto k = i*w+j;
            auto mag = sqrt(idft[k][0]*idft[k][0] + idft[k][1] * idft[k][1]);
            inverse(i, j) = mag;
        }
    }
    inverse *= (1.0/(w*h));

    fftw_destroy_plan(plan_f);
    fftw_destroy_plan(plan_b);
    fftw_free(in);
    fftw_free(dft);
    fftw_free(idft);

    Image i2(img2.cast<float>());
    i2.save_png("t2.png");

    return 0;
}