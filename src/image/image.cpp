#include "image/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "common/stb_image.h"

using namespace Eigen;

namespace jeffb::cv::image {

bool Image::load(std::string_view) {
    int chans;
    auto buff = stbi_load("../imgs/Chapter2/Input/Square.png", &dim_x_, &dim_y_, &chans, 1);
    if (buff == nullptr) {
        return false;
    }
    auto buff_len = dim_x_ * dim_y_ * 1;
    auto data = static_cast<uint8_t*>(buff);
    img_ = Map<Array<uint8_t, Dynamic, Dynamic>, RowMajor>(data, dim_y_, dim_x_);
    stbi_image_free(buff);
    return true;
}

}
