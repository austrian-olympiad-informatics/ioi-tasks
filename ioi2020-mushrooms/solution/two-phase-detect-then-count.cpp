/*
 * Near-optimal solution for task mushrooms
 *
 * Author: Ali Behjati
 */
#include "mushrooms.h"

using namespace std;

int count_mushrooms(int n) {
    vector<int> detected[2];

    detected[0].push_back(0);

    auto sz = [&] {return max(detected[0].size(), detected[1].size());};

    int i;
    for(i = 1; i < n && sz() < 2; i++) {
        int res = use_machine({0, i});
        detected[res].push_back(i); 
    }


    int k = 130;
    int t = (detected[1].size() == 2);
    for(; i < n && sz() < k; i += 2) {
        int res;
        if(i+1 < n)
            res = use_machine({detected[t][0], i, detected[t][1], i+1});
        else
            res = use_machine({detected[t][0], i, detected[t][1]});

        int not_t = (res & 2) ? 1 : 0;
        detected[t ^ not_t].push_back(i);

        if(i+1 < n) {
            not_t = res & 1; 
            detected[t ^ not_t].push_back(i+1);
        }
    }

    int more_zeros = 0;

    for(; i < n; ) {
        int size = sz();
        int t = (detected[1].size() == sz()); 
        int rem = min(size, n - i); 

        vector<int> tmp;
        for(int j = 0; j < rem; j++) {
            tmp.push_back(detected[t][j]);
            tmp.push_back(i+j); 
        }

        int res = use_machine(tmp);
		if (res % 2) res++;

        more_zeros += (t ? res / 2 : rem - res / 2);

        i += rem; 
    }

    return detected[0].size() + more_zeros;
}
