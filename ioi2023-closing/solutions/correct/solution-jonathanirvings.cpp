#include "closing.h"

#ifndef ATCODER_INTERNAL_TYPE_TRAITS_HPP
#define ATCODER_INTERNAL_TYPE_TRAITS_HPP 1

#include <cassert>
#include <numeric>
#include <type_traits>

namespace atcoder
{

    namespace internal
    {

#ifndef _MSC_VER
        template <class T>
        using is_signed_int128 =
            typename std::conditional<std::is_same<T, __int128_t>::value ||
                                          std::is_same<T, __int128>::value,
                                      std::true_type,
                                      std::false_type>::type;

        template <class T>
        using is_unsigned_int128 =
            typename std::conditional<std::is_same<T, __uint128_t>::value ||
                                          std::is_same<T, unsigned __int128>::value,
                                      std::true_type,
                                      std::false_type>::type;

        template <class T>
        using make_unsigned_int128 =
            typename std::conditional<std::is_same<T, __int128_t>::value,
                                      __uint128_t,
                                      unsigned __int128>;

        template <class T>
        using is_integral = typename std::conditional<std::is_integral<T>::value ||
                                                          is_signed_int128<T>::value ||
                                                          is_unsigned_int128<T>::value,
                                                      std::true_type,
                                                      std::false_type>::type;

        template <class T>
        using is_signed_int = typename std::conditional<(is_integral<T>::value &&
                                                         std::is_signed<T>::value) ||
                                                            is_signed_int128<T>::value,
                                                        std::true_type,
                                                        std::false_type>::type;

        template <class T>
        using is_unsigned_int =
            typename std::conditional<(is_integral<T>::value &&
                                       std::is_unsigned<T>::value) ||
                                          is_unsigned_int128<T>::value,
                                      std::true_type,
                                      std::false_type>::type;

        template <class T>
        using to_unsigned = typename std::conditional<
            is_signed_int128<T>::value,
            make_unsigned_int128<T>,
            typename std::conditional<std::is_signed<T>::value,
                                      std::make_unsigned<T>,
                                      std::common_type<T>>::type>::type;

#else

        template <class T>
        using is_integral = typename std::is_integral<T>;

        template <class T>
        using is_signed_int =
            typename std::conditional<is_integral<T>::value && std::is_signed<T>::value,
                                      std::true_type,
                                      std::false_type>::type;

        template <class T>
        using is_unsigned_int =
            typename std::conditional<is_integral<T>::value &&
                                          std::is_unsigned<T>::value,
                                      std::true_type,
                                      std::false_type>::type;

        template <class T>
        using to_unsigned = typename std::conditional<is_signed_int<T>::value,
                                                      std::make_unsigned<T>,
                                                      std::common_type<T>>::type;

#endif

        template <class T>
        using is_signed_int_t = std::enable_if_t<is_signed_int<T>::value>;

        template <class T>
        using is_unsigned_int_t = std::enable_if_t<is_unsigned_int<T>::value>;

        template <class T>
        using to_unsigned_t = typename to_unsigned<T>::type;

    } // namespace internal

} // namespace atcoder

#endif // ATCODER_INTERNAL_TYPE_TRAITS_HPP

#ifndef ATCODER_FENWICKTREE_HPP
#define ATCODER_FENWICKTREE_HPP 1

#include <cassert>
#include <vector>

namespace atcoder
{

    // Reference: https://en.wikipedia.org/wiki/Fenwick_tree
    template <class T>
    struct fenwick_tree
    {
        using U = internal::to_unsigned_t<T>;

    public:
        fenwick_tree() : _n(0) {}
        explicit fenwick_tree(int n) : _n(n), data(n) {}

        void add(int p, T x)
        {
            assert(0 <= p && p < _n);
            p++;
            while (p <= _n)
            {
                data[p - 1] += U(x);
                p += p & -p;
            }
        }

        T sum(int l, int r)
        {
            assert(0 <= l && l <= r && r <= _n);
            return sum(r) - sum(l);
        }

    private:
        int _n;
        std::vector<U> data;

        U sum(int r)
        {
            U s = 0;
            while (r > 0)
            {
                s += data[r - 1];
                r -= r & -r;
            }
            return s;
        }
    };

} // namespace atcoder

#endif // ATCODER_FENWICKTREE_HPP

#include <bits/stdc++.h>
using namespace std;

vector<vector<pair<int, int>>> adj;

struct SumCntFenwickTree
{
    SumCntFenwickTree(vector<pair<long long, int>> num)
        : _bit_cnt(num.size()), _bit_sum(num.size())
    {
        for (auto [a, _] : num)
        {
            _num.push_back(a);
        }
    }

    void Add(int x)
    {
        _bit_cnt.add(x, 1);
        _bit_sum.add(x, _num[x]);
    }

    void Remove(int x)
    {
        _bit_cnt.add(x, -1);
        _bit_sum.add(x, -_num[x]);
    }

    int Cnt(int x)
    {
        return _bit_cnt.sum(0, x);
    }

    long long Sum(int x)
    {
        return _bit_sum.sum(0, x);
    }

    vector<long long> _num;
    atcoder::fenwick_tree<int> _bit_cnt;
    atcoder::fenwick_tree<long long> _bit_sum;
};

void fill_distance(int u, int from, vector<long long> &distance)
{
    for (auto [v, w] : adj[u])
    {
        if (v != from)
        {
            distance[v] = distance[u] + w;
            fill_distance(v, u, distance);
        }
    }
}

int non_intersecting(
    int N, long long K,
    const vector<long long> &fromX, const vector<long long> &fromY)
{
    vector<long long> smaller(N);
    for (int i = 0; i < N; ++i)
    {
        smaller[i] = min(fromX[i], fromY[i]);
    }
    sort(smaller.begin(), smaller.end());
    for (int i = 0; i < N; ++i)
    {
        if (smaller[i] <= K)
        {
            K -= smaller[i];
        }
        else
        {
            return i;
        }
    }
    return N;
}

int max_score(int N, int X, int Y, long long K,
              std::vector<int> U, std::vector<int> V, std::vector<int> W)
{
    adj.assign(N, vector<pair<int, int>>());
    for (int i = 0; i < N - 1; ++i)
    {
        adj[U[i]].emplace_back(V[i], W[i]);
        adj[V[i]].emplace_back(U[i], W[i]);
    }
    vector<long long> fromX(N), fromY(N);
    fill_distance(X, -1, fromX);
    fill_distance(Y, -1, fromY);
    vector<bool> is_in_path(N);
    vector<int> in_path;
    for (int i = 0; i < N; ++i)
    {
        is_in_path[i] = fromX[i] + fromY[i] == fromX[Y];
        if (is_in_path[i])
        {
            in_path.push_back(i);
        }
    }

    int answer = non_intersecting(N, K, fromX, fromY);
    for (int u : in_path)
    {
        K -= min(fromX[u], fromY[u]);
    }
    if (K >= 0)
    {
        vector<int> order(N);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b)
             { return make_pair(max(fromX[a], fromY[a]) - min(fromX[a], fromY[a]), a) < make_pair(max(fromX[b], fromY[b]) - min(fromX[b], fromY[b]), b); });

        vector<pair<long long, int>> cost_one, cost_two;
        for (int i = 0; i < N; ++i)
        {
            if (is_in_path[i])
            {
                cost_one.emplace_back(
                    max(fromX[i], fromY[i]) - min(fromX[i], fromY[i]), i);
            }
            else
            {
                cost_two.emplace_back(max(fromX[i], fromY[i]), i);
                cost_one.emplace_back(min(fromX[i], fromY[i]), i);
            }
        }
        sort(cost_one.begin(), cost_one.end());
        sort(cost_two.begin(), cost_two.end());

        vector<int> ix_cost_one(N), ix_cost_two(N);
        for (int i = 0; i < N; ++i)
        {
            ix_cost_one[cost_one[i].second] = i;
            if (i < static_cast<int>(cost_two.size()))
            {
                ix_cost_two[cost_two[i].second] = i;
            }
        }

        SumCntFenwickTree bit_cost_one(cost_one);
        SumCntFenwickTree bit_cost_two(cost_two);
        for (int i = 0; i < N; ++i)
        {
            if (!is_in_path[i])
            {
                bit_cost_one.Add(ix_cost_one[i]);
            }
        }

        for (int i = 0; i < N; ++i)
        {
            int u = order[i];
            if (is_in_path[u])
            {
                bit_cost_one.Add(ix_cost_one[u]);
            }
            else
            {
                bit_cost_one.Remove(ix_cost_one[u]);
                bit_cost_two.Add(ix_cost_two[u]);
            }

            auto check = [&](int mid)
            {
                int lowest_cannot_take = lower_bound(
                                             cost_one.begin(),
                                             cost_one.end(),
                                             make_pair((cost_two[mid].first + 1) >> 1, 0)) -
                                         cost_one.begin();
                return bit_cost_two.Sum(mid + 1) + bit_cost_one.Sum(lowest_cannot_take) <= K;
            };

            int take_until_two = -1;
            {
                int lo = 0;
                int hi = static_cast<int>(cost_two.size()) - 1;
                while (lo <= hi)
                {
                    int mid = (lo + hi) >> 1;
                    if (check(mid))
                    {
                        take_until_two = mid;
                        lo = mid + 1;
                    }
                    else
                    {
                        hi = mid - 1;
                    }
                }
            }

            int take_until_one = -1;
            if (false)
            {
                while (take_until_one + 1 < N &&
                       bit_cost_two.Sum(take_until_two + 1) + bit_cost_one.Sum(take_until_one + 2) <= K)
                    ++take_until_one;
            }
            else
            {
                int lo = 0;
                int hi = N - 1;
                while (lo <= hi)
                {
                    int mid = (lo + hi) >> 1;
                    if (bit_cost_two.Sum(take_until_two + 1) + bit_cost_one.Sum(mid + 1) <= K)
                    {
                        take_until_one = mid;
                        lo = mid + 1;
                    }
                    else
                    {
                        hi = mid - 1;
                    }
                }
            }
            answer = max(answer, static_cast<int>(in_path.size()) + 2 * bit_cost_two.Cnt(take_until_two + 1) + bit_cost_one.Cnt(take_until_one + 1));

            if (take_until_one >= 0)
            {
                long long target_cost = bit_cost_one.Sum(take_until_one + 1);
                {
                    take_until_one = -1;
                    int lo = 0;
                    int hi = N - 1;
                    while (lo <= hi)
                    {
                        int mid = (lo + hi) >> 1;
                        if (bit_cost_one.Sum(mid + 1) < target_cost)
                        {
                            take_until_one = mid;
                            lo = mid + 1;
                        }
                        else
                        {
                            hi = mid - 1;
                        }
                    }
                }
                int lo = 0;
                int hi = static_cast<int>(cost_two.size()) - 1;
                while (lo <= hi)
                {
                    int mid = (lo + hi) >> 1;
                    if (bit_cost_two.Sum(mid + 1) + bit_cost_one.Sum(take_until_one + 1) <= K)
                    {
                        take_until_two = mid;
                        lo = mid + 1;
                    }
                    else
                    {
                        hi = mid - 1;
                    }
                }
                answer = max(answer, static_cast<int>(in_path.size()) + 2 * bit_cost_two.Cnt(take_until_two + 1) + bit_cost_one.Cnt(take_until_one + 1));
            }
        }
    }
    return answer;
}
