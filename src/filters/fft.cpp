#include "filters/fft.h"
#include <fftw3.h>
#include <iostream>

using namespace std;

namespace jeffb::cv::filters {

FftFilters::FftFilters(image::Image img) {
    auto d = img.get_dims();
    auto w = d[1];
    auto h = d[0];
    auto N = w * h;
    auto in = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * N));
    auto dft = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * N));
    auto plan = fftw_plan_dft_2d(w, h, in, dft, FFTW_FORWARD, FFTW_ESTIMATE);
}

FftFilters::~FftFilters() {}

}
