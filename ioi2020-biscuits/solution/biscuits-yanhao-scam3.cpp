#include "biscuits.h"
#include <vector>

long long count_tastiness(long long x, std::vector<long long> a) {
    long long sum = 0;
    for(int i=0; i<(int) a.size(); i++) {
        sum += (a[i]<<i);
    }
    return sum/x+1;
}
