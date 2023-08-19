#pragma once

#include <array>
#include <string>
#include <Eigen/Dense>
#include <cstdint>

namespace jeffb::cv::image {

class Image {
public:
    bool load(std::string);

    [[nodiscard]]
    std::array<int, 2> get_dims() const { return {dim_x_, dim_y_}; }

    Eigen::Ref<Eigen::Array<uint8_t, Eigen::Dynamic, Eigen::Dynamic>> get_image() { return img_; }

private:
    int dim_x_{0};
    int dim_y_{0};
    Eigen::Array<uint8_t, Eigen::Dynamic, Eigen::Dynamic> img_ = Eigen::Array<uint8_t, Eigen::Dynamic, Eigen::Dynamic>::Zero(0, 0);
};

}
