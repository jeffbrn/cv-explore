#include "image/image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "common/stb_image.h"
#include "common/stb_image_write.h"

using namespace Eigen;

namespace jeffb::cv::image {

Image::Image(ArrayXXf pixels) {
    auto min = pixels.minCoeff();
    pixels -= min;
    auto max = pixels.maxCoeff();
    
    pixels -= pixels.minCoeff();
    auto scale = 255.f / pixels.maxCoeff();
    pixels *= scale;
    img_ = pixels.cast<uint8_t>();
    dim_x_ = img_.cols();
    dim_y_ = img_.rows();
}

bool Image::load(std::string filepathname) {
    int chans;
    auto buff = stbi_load(filepathname.c_str(), &dim_x_, &dim_y_, &chans, 1);
    if (buff == nullptr) {
        return false;
    }
    auto buff_len = dim_x_ * dim_y_ * 1;
    auto data = static_cast<uint8_t *>(buff);
    img_ = Map<Array<uint8_t, Dynamic, Dynamic>, RowMajor>(data, dim_y_, dim_x_);
    stbi_image_free(buff);
    return true;
}

void Image::save_png(std::string filepathname) {
    Array<uint8_t, Dynamic, Dynamic, RowMajor> buff = img_;
    stbi_write_png(filepathname.c_str(), dim_x_, dim_y_, 1, buff.data(), dim_x_);
}

}
