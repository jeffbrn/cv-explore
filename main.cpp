#include <iostream>
#include <Eigen/Dense>
#include <cstdint>
#include "image/image.h"

// #define STB_IMAGE_IMPLEMENTATION
// #include "common/stb_image.h"

using namespace std;
using namespace Eigen;
using namespace jeffb::cv::image;

int main() {
  // int x, y, chans;
  // auto buff = stbi_load("../imgs/Chapter2/Input/Square.png", &x, &y, &chans, 1);
  // if (buff == nullptr) {
  //   cout << "Unable to load image" << endl;
  //   return 0;
  // }
  // auto buff_len = x*y*1;
  // cout << "x=" << x << ", y=" << y << ", chans=" << chans << endl;
  // auto data = static_cast<uint8_t*>(buff);
  // // for(int i = 0; i < buff_len; i++) {
  // //   cout << (int)data[i] << ", ";
  // // }
  // cout << "data[4]=" << (int)data[5] << endl;
  // Array<uint8_t, Dynamic, Dynamic> img = Map<Array<uint8_t, Dynamic, Dynamic>, RowMajor>(data, y, x);
  // cout << endl << img << endl;
  // stbi_image_free(buff);
  // cout << (int)img(0, 5) << endl;

  Image img1;
  auto i = img1.get_image();
  cout << i.rows() << " x " << i.cols() << endl;
  if (!img1.load("../imgs/Chapter2/Input/Square.png")) {
    cout << "failed to load image" << endl;
    return 1;
  }
  Array<uint8_t,Dynamic,Dynamic> b = img1.get_image();
  cout << b << endl;
  b(0,0) = 0;
  cout << b << endl;
  auto c = img1.get_image();
  cout << endl << c << endl;

  return 0;
}