//
// Moduł wyjściowy
//

#ifndef PROJEKT_OUTPUT_H
#define PROJEKT_OUTPUT_H
class Filter{
    static const int AVERAGE_BUFFER = 10;
    float m_arr[AVERAGE_BUFFER];
    int m_idx = 0;
public:
    Filter();
    ~Filter();
    float append(float val);
    float avg();
};

float ComputeDegrees(float input);
int ComputeSignal(int mode, float input);

#endif //PROJEKT_OUTPUT_H
