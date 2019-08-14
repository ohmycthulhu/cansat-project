#ifndef __KALMAN__
#define __KALMAN__

/*
    It is Kalman Filter implementation for 1 dimensional variable
    It will be used in order to prevent value jumps (e.g. 1 => 10)
*/

// Type can be either float or double
template<typename T>
class KalmanFilter {
private:
    T value = 0;
    double p = 1;
    const double r = 64, q = 9;
public:
    KalmanFilter () {}
    KalmanFilter (const T& value) : value(value) {}

    T update(const T& newValue) {
        double _p = p + q;
        double k = _p / (_p + r);
        if (value) {
            value = (value + k * (newValue - value)); // x + alpha * dx
            p = _p * (1 - k);
        } else {
            value = newValue;
        }
        return value;
    }
    T getValue() const { return value; }
};

#endif