long long fpow(long long b, long long m, long long mod)
{
    long long ans = 1 % mod;
    long long x = b;
    while (m != 0)
    {
        if (m % 2 == 1) ans *= x;
        ans %= mod;
        x *= x;
        x %= mod;
        m /= 2;
    }
    return ans;
}