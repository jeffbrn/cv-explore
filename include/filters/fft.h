#pragma once

#include <complex>
#include <Eigen/Dense>
#include "image/image.h"

namespace jeffb::cv::filters {

class FftFilters {
public:
    FftFilters(image::Image img);
    ~FftFilters();

private:
    Eigen::ArrayXXcd dft_ = Eigen::ArrayXXcd::Zero(0, 0);
};

}
