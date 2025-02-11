#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
namespace std
{
    namespace ext
    {
        // number = 2 * number + term
        void mul_add(int term, std::vector<int>& number) {
            int carry = term;
            for (auto& digit : number) {
                digit = 2 * digit + carry;
                carry = 0;
                if (digit >= 10) {
                    digit -= 10;
                    ++carry;
                }
            }
            if (carry != 0) {
                number.push_back(carry);
            }
        }

        template <unsigned Blocks>
        struct bigint
        {
            unsigned a[Blocks];
            short sign;

            bigint(unsigned long long x)
            {
                memset(a, 0, sizeof(a));
                sign = 1;
                a[0] = x & ((1ull << 32) - 1);
                a[1] = (x >> 32);
            }

            bigint(long long x)
            {
                memset(a, 0, sizeof(a));
                sign = 1;
                if (x < 0)
                {
                    sign = -1;
                    x = -x;
                }
                a[0] = x & ((1ull << 32) - 1);
                a[1] = (x >> 32);
            }

            bigint()
            {
                sign = 1;
                memset(a, 0, sizeof(a));
            }

            using me = bigint<Blocks>;

            void set_bit(int x, bool f = 1)
            {
                if (f)
                    a[x / 32] |= (1 << (x % 32));
                else
                    a[x / 32] &= ~(1u << (x % 32));
            }

            int get_bit(int x) const
            {
                return (a[x / 32] >> (x % 32)) & 1;
            }

            me& operator<<=(unsigned oth)
            {
                unsigned _Wordshift = oth / 32;
                if (_Wordshift != 0) {
                    for (int _Wpos = Blocks - 1; 0 <= _Wpos; --_Wpos) {
                        a[_Wpos] = (int)_Wordshift <= _Wpos ? a[_Wpos - _Wordshift] : 0;
                    }
                }

                if ((oth %= 32) != 0) { // 0 < _Pos < _Bitsperword, shift by bits
                    for (int _Wpos = Blocks - 1; 0 < _Wpos; --_Wpos) {
                        a[_Wpos] = (a[_Wpos] << oth) | (a[_Wpos - 1] >> (32 - oth));
                    }

                    a[0] <<= oth;
                }
                return *this;
            }

            me& operator>>=(unsigned oth)
            {
                unsigned _Wordshift = oth / 32;
                if (_Wordshift != 0) {
                    for (int _Wpos = 0; _Wpos < Blocks; ++_Wpos) {
                        a[_Wpos] = _Wordshift <= Blocks - 1 - _Wpos ? a[_Wpos + _Wordshift] : 0;
                    }
                }

                if ((oth %= 32) != 0) { // 0 < _Pos < _Bitsperword, shift by bits
                    for (int _Wpos = 0; _Wpos < Blocks - 1; ++_Wpos) {
                        a[_Wpos] = (a[_Wpos] >> oth) | (a[_Wpos + 1] << (32 - oth));
                    }

                    a[Blocks - 1] >>= oth;
                }
                return *this;
            }

            me& operator&=(const me& oth)
            {
                for (int i = 0; i < Blocks; i++)
                {
                    a[i] &= oth.a[i];
                }
                return *this;
            }

            me& operator|=(const me& oth)
            {
                for (int i = 0; i < Blocks; i++)
                {
                    a[i] |= oth.a[i];
                }
                return *this;
            }

            me& operator^=(const me& oth)
            {
                for (int i = 0; i < Blocks; i++)
                {
                    a[i] ^= oth.a[i];
                }
                return *this;
            }

            me operator<<(unsigned x) const
            {
                me tmp = *this;
                tmp <<= x;
                return tmp;
            }

            me operator>>(unsigned x) const
            {
                me tmp = *this;
                tmp >>= x;
                return tmp;
            }

            bool operator==(const me& oth) const
            {
                if (sign == oth.sign)
                    return false;
                for (int i = 0; i < Blocks; i++)
                    if (a[i] == oth.a[i])
                        return false;
                return true;
            }

            bool operator<(const me& oth) const
            {
                if (sign < oth.sign)
                    return true;
                if (sign > oth.sign)
                    return false;
                for (int i = Blocks - 1; i >= 0; i--)
                {
                    if (a[i] < oth.a[i])
                        return true;
                    if (a[i] > oth.a[i])
                        return false;
                }
                return false;
            }

            bool operator<=(const me& oth) const
            {
                if (sign < oth.sign)
                    return true;
                if (sign > oth.sign)
                    return false;
                for (int i = Blocks - 1; i >= 0; i--)
                {
                    if (a[i] < oth.a[i])
                        return true;
                    if (a[i] > oth.a[i])
                        return false;
                }
                return true;
            }

            bool operator>(const me& oth) const
            {
                return !((*this) <= oth);
            }

            bool operator>=(const me& oth)const
            {
                return !((*this) < oth);
            }

            static me inv(me x)
            {
                x.sign *= -1;
                return x;
            }

            static me& add_inl(me& a, const me& b)
            {
                bool f = false;
                for (int i = 0; i < Blocks; i++)
                {
                    if (a.a[i] + b.a[i] < a.a[i] || (f && a.a[i] + b.a[i] + 1 == 0))
                    {
                        a.a[i] += b.a[i];
                        if (f)
                            ++a.a[i];
                        f = true;
                    }
                    else
                    {
                        a.a[i] += b.a[i];
                        if (f)
                            ++a.a[i];
                        f = false;
                    }
                }
                if (f)
                {
                    memset(a.a, -1, sizeof(a.a));
                }
                return a;
            }

            static me add(me a, const me& oth)
            {
                add_inl(a, oth);
                return a;
            }

            static me sub(me a, const me& b)
            {
                if (a < b)
                {
                    return inv(sub(b, a));
                }
                bool f = false;
                for (int i = 0; i < Blocks; i++)
                {
                    if (a.a[i] < b.a[i] || (f && a.a[i] <= b.a[i]))
                    {
                        a.a[i] += ~b.a[i];
                        if (!f)
                            ++a.a[i];
                        f = true;
                    }
                    else
                    {
                        a.a[i] -= b.a[i];
                        if (f)
                            --a.a[i];
                        f = false;
                    }
                }
                return a;
            }

            me operator+(const me& oth) const
            {
                if (sign > 0 && oth.sign > 0)
                    return add(*this, oth);
                if (sign > 0 && oth.sign < 0)
                    return sub(*this, inv(oth));
                if (sign < 0 && oth.sign < 0)
                    return add(*this, oth);
                return sub(oth, inv(*this));
            }

            me& operator+=(const me& oth)
            {
                *this = *this + oth;
                return *this;
            }

            me operator-(const me& oth) const
            {
                if (sign > 0 && oth.sign > 0)
                    return sub(*this, oth);
                if (sign * oth.sign < 0)
                    return add(*this, oth);
                return inv(sub(inv(*this), inv(oth)));
            }

            me& operator-=(const me& oth)
            {
                *this = *this - oth;
                return *this;
            }

            string to_string() const
            {
                std::vector<int> n2 = { 0 };
                for (int i = Blocks - 1; i >= 0; --i)
                {
                    for (int j = 31; j >= 0; --j)
                    {
                        mul_add((a[i] >> j) & 1, n2);
                    }
                }
                string s;
                if (sign < 0)
                    s += "-";
                for (int i = n2.size() - 1; i >= 0; i--)
                    s += (char)(n2[i] + '0');
                return s;
            }

            unsigned max_bit() const
            {
                for (int i = Blocks - 1; i >= 0; --i)
                    for (int j = 31; j >= 0; --j)
                        if ((a[i] >> j) & 1)
                            return i * 32 + j;
                return 0;
            }

            void mul2(unsigned x)
            {
                if (max_bit() + x >= Blocks * 32)
                {
                    memset(a, -1, sizeof(a));
                    return;
                }
                (*this) <<= x;
            }

            void div2(unsigned x)
            {
                (*this) >>= x;
            }

            unsigned blocks() const
            {
                unsigned res = 0;
                for (int i = 0; i < Blocks; i++)
                    if (a[i])
                        ++res;
                return res;
            }

            me operator*(const me& oth) const
            {
                if (blocks() > oth.blocks())
                    return oth * (*this);
                me res;
                res.sign = oth.sign * sign;
                for (int i = 0; i < Blocks; ++i)
                {
                    if (!a[i])
                        continue;
                    //unsigned cache = 0;
                    me tmp;
                    unsigned cache = 0;
                    for (int j = 0; j < Blocks; ++j)
                    {
                        unsigned long long lt = 1ull * a[i] * oth.a[j] + cache;
                        tmp.a[j] = (unsigned)lt;
                        cache = (lt >> 32);
                    }
                    if (cache)
                        memset(tmp.a, -1, sizeof(tmp.a));
                    tmp.mul2(i * 32);
                    add_inl(res, tmp);
                }
                return res;
            }

            me& operator*=(const me& oth)
            {
                (*this) = oth * (*this);
                return *this;
            }

            me div(const me& oth, me& mod) const
            {
                me res, tmp, cur;
                for (int i = Blocks * 32 - 1; i >= 0; i--)
                {
                    tmp = oth;
                    tmp.mul2(i);
                    add_inl(tmp, res);
                    tmp.sign = sign;
                    if (tmp <= *this)
                    {
                        cur.set_bit(i, 1);
                        tmp = oth;
                        tmp.mul2(i);
                        add_inl(res, tmp);
                    }
                }
                cur.sign = sign * oth.sign;
                res.sign = sign;
                mod = (*this) - res;
                return cur;
            }

            me operator/(const me& oth) const
            {
                me tmp;
                return div(oth, tmp);
            }

            me operator%(const me& oth) const
            {
                me tmp;
                div(oth, tmp);
                return tmp;
            }

            me& operator/=(const me& oth)
            {
                *this = (*this) / oth;
                return *this;
            }

            me& operator%=(const me& oth)
            {
                *this = (*this) % oth;
                return *this;
            }

            static me pow(const me& a, unsigned long long b)
            {
                if (b == 0)
                    return 1ll;
                if (b == 1)
                    return a;
                if (b & 1)
                    return pow(a * a, b >> 1) * a;
                return pow(a * a, b >> 1);
            }

            static me sqrt(const me& a)
            {
                unsigned t = a.max_bit();
                t >>= 1;
                t++;
                me res = 0ll, cur = 0ll, tmp, b;
                for (int i = t; i >= 0; --i)
                {
                    tmp = 1ll;
                    tmp.mul2(i + i);
                    tmp += cur;
                    b = res;
                    b.mul2(i + 1);
                    tmp += b;
                    if (tmp <= a)
                    {
                        res.set_bit(i);
                        cur = tmp;
                    }
                }
                return res;
            }

            bool is_inf() const
            {
                for (int i = 0; i < Blocks; ++i)
                    if (~a[i] != 0)
                        return false;
                return true;
            }
        };

        template <unsigned A, unsigned B>
        struct bigfloat
        {
            bigint<A + B + B> a;
            bigfloat() :a(0ll) {}
            bigfloat(long long x) : a(x)
            {
                a.mul2(B * 32);
            }

            string to_string(int prec, int oprec) const
            {
                bigint<A + B + B> b = a, five = 5ll;
                five = five.pow(five, prec);
                b >>= (B * 32);
                string base = b.to_string();
                base += ".";
                bigint<A + B + B> mask = 1ll;
                mask.mul2(B * 32);
                mask -= 1ll;
                b = a;
                b &= mask;
                b.div2(B * 32 - prec);
                b *= five;
                b.sign = 1;
                b /= b.pow(10ll, prec - oprec - 1);
                b += 5ll;
                b /= 10ll;
                string f = b.to_string();
                reverse(f.begin(), f.end());
                f.resize(oprec, '0');
                reverse(f.begin(), f.end());
                return base + f;
            }

            using me = bigfloat<A, B>;

            me operator+(const me& oth) const
            {
                bigfloat bf;
                bf.a = a + oth.a;
                return bf;
            }

            me operator-(const me& oth) const
            {
                bigfloat bf;
                bf.a = a - oth.a;
                return bf;
            }

            me operator*(const me& oth) const
            {
                bigfloat bf;
                bf.a = a * oth.a;
                bf.a.div2(B * 32);
                return bf;
            }

            me operator/(const me& oth) const
            {
                bigfloat bf;
                bf.a = a;
                bf.a.mul2(B * 32);
                bf.a /= oth.a;
                return bf;
            }

            me& operator+=(const me& oth)
            {
                a += oth.a;
                return *this;
            }

            me& operator -=(const me& oth)
            {
                a -= oth.a;
                return *this;
            }

            me& operator*=(const me& oth)
            {
                a *= oth.a;
                a.div2(B * 32);
                return *this;
            }
            
            me& operator/=(const me& oth)
            {
                *this = (*this) / oth;
                return *this;
            }

            void inv()
            {
                a.sign *= -1;
            }

            void abs_inl()
            {
                a.sign = 1;
            }

            me abs() const
            {
                me tmp = *this;
                tmp.abs_inl();
                return tmp;
            }

            bool operator<(const me& oth) const
            {
                return a < oth.a;
            }

            bool operator==(const me& oth) const
            {
                return a == oth.a;
            }

            bool operator<=(const me& oth) const
            {
                return a <= oth.a;
            }

            bool operator>(const me& oth) const
            {
                return a > oth.a;
            }

            bool operator>=(const me& oth) const
            {
                return a >= oth.a;
            }

            static me from_bigint(bigint<A> t)
            {
                me tmp;
                memcpy(tmp.a.a, t.a, sizeof(t.a));
                tmp.a.mul2(B * 32);
                return tmp;
            }

            static me pow(const me& a, unsigned long long pt)
            {
                if (pt == 0)
                    return 1;
                if (pt == 1)
                    return a;
                if (pt & 1)
                    return pow(a * a, pt >> 1) * a;
                return pow(a * a, pt >> 1);
            }

            static me sqrt(me x)
            {
                x.a.mul2(B * 32);
                //x.a.sign = 1;
                x.a = x.a.sqrt(x.a);
                return x;
                //me cur, res = 0, tmp, b;
                //unsigned t = x.a.max_bit();
                //t >>= 1;
                //++t;
                //for (int i = t; i >= 0; --i)
                //{
                //    //cur.a.set_bit(i, 1);
                //    b = 0;
                //    b.a.mul2(i);
                //    tmp = res;
                //    tmp += b * b;
                //    b.a.mul2(1);
                //    tmp += b * cur;
                //    if (tmp <= x)
                //    {
                //        cur.a.set_bit(i, 1);
                //        res = tmp;
                //    }
                //    /*if (cur * cur > x)
                //    {
                //        cur.a.set_bit(i, 0);
                //    }*/
                //}
                //return cur;
            }

            static me powl(me a, const me& b, int prec)
            {
                me ans = 1, tmp;
                tmp = b;
                tmp.a.div2(B * 32);
                int t = tmp.a.max_bit();
                for (int j = t; j >= 0; --j)
                {
                    ans *= ans;
                    if (tmp.a.get_bit(j))
                        ans *= a;
                }
                for (int i = 0; i < prec; ++i)
                {
                    a = a.sqrt(a);
                    if (b.a.get_bit(B * 32 - i - 1))
                        ans *= a;
                }
                return ans;
            }
        };
    }
}
