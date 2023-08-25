#include <iostream>
#include <Eigen/Dense>
#include <fftw3.h>
#include "image/image.h"
#include "filters/fft.h"
#include <cmath>

using namespace std;
using namespace Eigen;
using namespace jeffb::cv;

const float PI = atanf(1.f) * 4.f;

int main() {
    cout << PI << endl;
    image::Image img1;
    filters::FftFilters fft(img1);

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
    cout << input_img << endl << endl;
    d = img1.get_dims();
    cout << "Dims: " << d[0] << " x " << d[1] << ", image dimensions: " << input_img.rows() << " x " << input_img.cols() << endl;
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
    
    ArrayXXd img2 = ArrayXXd::Zero(w, h);
    ArrayXXd img3 = ArrayXXd::Zero(w, h);
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            auto k = i*w+j;
            auto mag = sqrt(dft[k][0]*dft[k][0] + dft[k][1] * dft[k][1]);
            img2(i, j) = mag;
            img3(i,j) = dft[k][1] == 0 ? 0.0 : atan(dft[k][1] / dft[k][0]);
        }
    }
    img2 *= (1.0/(w*h));

    dft[0][0] = 0.0;
    dft[0][1] = 0.0;

    fftw_execute(plan_b);
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
    fftw_cleanup();

    cout << img2 << endl << endl;
    cout << inverse << endl;
    image::Image z1(img2.cast<float>());
    z1.save_png("t1.png");
    image::Image z2(img3.cast<float>());
    z2.save_png("t2.png");
    image::Image z3(inverse.cast<float>());
    z3.save_png("t3.png");

    return 0;
}