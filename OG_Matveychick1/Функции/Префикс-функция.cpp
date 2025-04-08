vector<long long> p_function(string s)
{
    long long n = (long long)s.length();
    vector<long long> p(n);
    for (long long i = 1; i < n; ++i)
    {
        long long j = p[i - 1];
        while (j > 0 && s[i] != s[j])
            j = p[j - 1];
        if (s[i] == s[j])  ++j;
        p[i] = j;
    }
    ret p;
}