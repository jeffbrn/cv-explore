#include <iostream>
#include <Eigen/Dense>
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
    auto i = img1.get_image();
    auto d = img1.get_dims();
    cout << "Dims: " << d[0] << " x " << d[1] << ", image dimensions: " << i.rows() << " x " << i.cols() << endl << endl;

    cout << "Loading image :-" << endl;
    if (!img1.load("./Square.png")) {
        cout << "failed to load image" << endl;
        return 1;
    }
    auto input_img = img1.get_image();
    input_img(0,5) = 0;
    input_img(0,6) = 0;
    input_img(0,7) = 0;
    d = img1.get_dims();
    cout << "Dims: " << d[0] << " x " << d[1] << ", image dimensions: " << input_img.rows() << " x " << input_img.cols() << endl;
    //cout << input_img << endl;
    img1.save_png("t1.png");
    ArrayXXf x1 = input_img.cast<float>();

    // max freq according to the sampling
    auto max_freq_w = d[0] / 2;
    auto max_freq_h = d[1] / 2;
    // create the coefficients images
    auto num_coeff_w = 1 + 2 * max_freq_w;
    auto num_coeff_h = 1 + 2 * max_freq_h;
    ArrayXXf ff_mag = ArrayXXf::Zero(num_coeff_h, num_coeff_w);
    ArrayXXf ff_phase = ArrayXXf::Zero(num_coeff_h, num_coeff_w);

    // Adjust the size of the data to be even
    auto m = static_cast<float>(d[0]) + (d[0] % 2 == 0 ? 1.f : 0.f);
    auto n = static_cast<float>(d[1]) + (d[1] % 2 == 0 ? 1.f : 0.f);

    // Fundamental frequency
    auto ww = (2.f * PI) / m;
    auto wh = (2.f * PI) / n;

    cout << "max_freq_w=" << max_freq_w << ", max_freq_h=" << max_freq_h << ", num_coeff_w=" << num_coeff_w << ", num_coeff_h=" << num_coeff_h << endl;
    cout << "m=" << m << ", n=" << n << ", ww=" << ww << ", wh=" << wh << endl;
    // Fourier transform
    cout << "*** step 1" << endl;
    //ArrayXXf tmp1 = ArrayXXf::Zero(max_freq_h, max_freq_w);
    for(int u = -max_freq_w; u <= max_freq_w; u++) {
        auto entry_w = u + max_freq_w;
        for(int v = -max_freq_h; v <= max_freq_h; v++) {
            auto entry_h = v + max_freq_h;
            for(int x = 0; x < d[0]; ++x) {
                float a1 = 0; float a2 = 0;
                for(int y = 0; y < d[1]; ++y) {
//                    cout << "(" << x << ", " << y << "), ";
                    a1 += input_img(y,x) + cosf(y * wh * v);
                    a2 += input_img(y,x) + sinf(y * wh * v);
                }
//                cout << endl;
                ff_mag(entry_h, entry_w) += a1 * cosf(x * ww * u) - a2 * sinf(x * ww * u);
                ff_phase(entry_h, entry_w) -= cosf(x * ww * u) * a2 + sinf(x * ww * u) * a1;
                //tmp1(entry_h, entry_w) = ff_mag(entry_h, entry_w);
            }
        }
    }

    cout << "*** step 2" << endl;
    for (int kw = -max_freq_w; kw <= max_freq_w; kw++) {
        auto entry_w = kw + max_freq_w;
        for(int kh = -max_freq_h; kh <= max_freq_h; kh++) {
//            cout << "(" << kw << ", " << kh << "), ";
            auto entry_h = kh + max_freq_h;
            ff_mag(entry_h, entry_w) *= m * n;
            ff_phase(entry_h, entry_w) *= m * n;
        }
//        cout << endl;
    }

    cout << "FF magnitude:" << endl;
    ff_mag -= ff_mag.minCoeff();
    auto scale = 255.f / ff_mag.maxCoeff();
    ff_mag *= scale;
    cout << ff_mag << endl << endl;
    cout << ff_mag.cast<int>() << endl;

    // Reconstruction
    cout << "*** step 3" << endl;
    ArrayXXf reconstruction = ArrayXXf::Zero(d[0], d[1]);
    for (int u = -max_freq_w; u <= max_freq_w; u++) {
        auto entry_w = u + max_freq_w;
        for(int v = -max_freq_h; v <= max_freq_h; v++) {
            auto entry_h = v + max_freq_h;
            for(int x = 0; x < d[0]; x++) {
                for(int y = 0; y < d[1]; y++) {
//                    cout << "(" << x << ", " << y << "), ";
                    reconstruction(y, x) += (ff_mag(entry_h, entry_w) / (m * n)) * (cosf(x * ww * u) * cosf(y * wh * v) - sinf(x * ww * u) * sinf(y * wh * v)) -
                        (ff_phase(entry_h, entry_w) / (m * n)) * (cosf(x * ww * u) * sinf(y * wh * v) + sinf(x * ww * u) * cosf(y * wh * v));
                }
//                cout << endl;
            }
        }
    }

//    cout << reconstruction << endl;
    return 0;
}