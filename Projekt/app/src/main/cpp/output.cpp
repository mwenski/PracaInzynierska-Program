//
// Moduł wyjściowy
//

#include "output.h"

Filter::Filter() {
    const int AVERAGE_BUFFER = 10;
    m_arr[AVERAGE_BUFFER];
    m_idx = 0;
}

Filter::~Filter() {
    delete[] m_arr;
}


float Filter::append(float val) {
        m_arr[m_idx] = val;
        m_idx++;
        if (m_idx == AVERAGE_BUFFER)
            m_idx = 0;
        return avg();
    }

float Filter::avg() {
        float sum = 0;
        for (float x : m_arr)
            sum += x;
        return sum / AVERAGE_BUFFER;
    }


Filter m_filter;

float ComputeOrientation(float orientation){
    float output = 0.f;
    /** 1 radian = 57.2957795 stopnia */
    float x = orientation * 57.2957795f;
    output = m_filter.append(x);
    return output;
}

int ComputeSignal(int mode, float input){
    int signal = 50;
    int output = 0;
    switch (mode){
        case 1:
            if (input >= 0) {
                signal = (int) (50 + (180 - input));
            } else {
                // signal = (int) (50 - (180 + input));
                signal = 0;
            }
            output = 500 + (signal*10) * 2 ;

        case 2:
            if (input > 100) {
                signal = 100;
            } else if (input >= 0) {
                signal = (int) input;
            } else {
               signal = 0;
            }
            output = 500 + (signal*10) * 2 ;

        case 3:
            output = (input * 10)+1000;
    }
    return output;
}



