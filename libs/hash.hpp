// Hash library
// TEST OK
// (c) Mark_Pr 2025
#pragma once
#include <cstring>
#include <vector>
namespace std
{
    const unsigned RTHH_MODL[] = { 0, (int)1e9 + 7, (int)1e9 + 9, 999935977 };

    struct RTHH
    {
        unsigned data[4];

        RTHH(unsigned x)
        {
            data[0] = x;
            data[1] = x;
            data[2] = x;
            data[3] = x;
        }

        RTHH(char c)
        {
            data[0] = c - 'a';
            data[1] = c - 'a';
            data[2] = c - 'a';
            data[3] = c - 'a';
        }

        RTHH()
        {
            memset(data, 0, sizeof(data));
        }

        inline void add_mod(unsigned& a, unsigned b, unsigned MOD)
        {
            a += b;
            if (a >= MOD)
                a -= MOD;
        }

        inline void mul_mod(unsigned& a, unsigned b, unsigned MOD)
        {
            a = (1ll * a * b % MOD);
        }

        inline void sub_mod(unsigned& a, unsigned b, unsigned MOD)
        {
            if (a < b)
                a += MOD;
            a -= b;
        }

        void operator+=(const RTHH& oth)
        {
            data[0] += oth.data[0];
            add_mod(data[1], oth.data[1], RTHH_MODL[1]);
            add_mod(data[2], oth.data[2], RTHH_MODL[2]);
            add_mod(data[3], oth.data[3], RTHH_MODL[3]);
        }

        void operator-=(const RTHH& oth)
        {
            data[0] -= oth.data[0];
            sub_mod(data[1], oth.data[1], RTHH_MODL[1]);
            sub_mod(data[2], oth.data[2], RTHH_MODL[2]);
            sub_mod(data[3], oth.data[3], RTHH_MODL[3]);
        }

        void operator*=(const RTHH& oth)
        {
            data[0] *= oth.data[0];
            mul_mod(data[1], oth.data[1], RTHH_MODL[1]);
            mul_mod(data[2], oth.data[2], RTHH_MODL[2]);
            mul_mod(data[3], oth.data[3], RTHH_MODL[3]);
        }
    };

    RTHH operator+(RTHH a, const RTHH& b)
    {
        a += b;
        return a;
    }

    RTHH operator-(RTHH a, const RTHH& b)
    {
        a -= b;
        return a;
    }

    RTHH operator*(RTHH a, const RTHH& b)
    {
        a *= b;
        return a;
    }

    bool operator==(const RTHH& a, const RTHH& b)
    {
        return memcmp(a.data, b.data, sizeof(a.data)) == 0;
    }

    bool operator<(const RTHH& a, const RTHH& b)
    {
        return memcmp(a.data, b.data, sizeof(a.data)) < 0;
    }

    struct StaticHashSegmentTree
    {
    protected:
        vector <RTHH> l, P;

    public:
        void build(const vector<RTHH>& lst)
        {
            int n = lst.size();
            l.resize(n + 1);
            P.resize(n + 1);
            P[0] = 1u;
            for (size_t i = 0; i < n; i++)
            {
                l[i + 1] = l[i] * 27u + lst[i];
                P[i + 1] = P[i] * 27u;
            }
        }
        template <typename T>
        void build(T first, T last)
        {
            vector <RTHH> tmp(first, last);
            build(tmp);
        }

        unsigned size()
        {
            return l.size() + 1;
        }

        RTHH segment(int l, int r)
        {
            if (l < 0)
                l = 0;
            if (r > size())
                r = size();
            if (l >= r)
                return 0u;
            return this->l[r] - this->l[l] * this->P[r - l];
        }
    };
}
