#pragma once 

class Vec4 {
    public:
        Vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x_(x), y_(y), z_(z), w_(w) {}

        float x() const {
            return x_;
        }

        float y() const {
            return y_;
        }

        float z() const {
            return z_;
        }

        float w() const {
            return w_;
        }

    private:
        float x_;
        float y_;
        float z_;
        float w_;
};