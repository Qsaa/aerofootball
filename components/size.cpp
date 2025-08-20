#include "size.hpp"

Size::Size(float w, float h) : w_(w), h_(h) {}

Size::Size(int w, int h): w_(w), h_(h){}

Size::Size(const Size& size) : w_(size.w_), h_(size.h_) {}