struct DST
{
private:
    struct node;
    ll n;
    vector<vector<node>> table;
    ll maxlev;
    struct node
    {
        ll x;
        node() {}
        node(ll x) :x(x) {}
    };
    node neutral = 0;
    friend node operator + (node a, node b)
    {
        ret node(min(a.x, b.x));
    }
public:
    DST(ll n1, vll& a)
    {
        n = pow(2, ceil(log2(n1)));
        maxlev = __builtin_clz(n) ^ 31;
        table.rs(log2(n) + 1);
        for0(i, sz(table))
        {
            table[i].rs(n);
        }
        while (sz(a) < n)
        {
            a.pb(neutral.x);
        }
        build(0, 0, n, a);
    }
private:
    void build(ll level, ll l, ll r, vll& a)
    {
        ll m = (l + r) / 2;
        table[level][m] = a[m];
        for (ll i = m - 1; i >= l; i--)
        {
            table[level][i] = table[level][i + 1] + node(a[i]);
        }
        if (m + 1 < r)
        {
            table[level][m + 1] = a[m + 1];
            rep(i, m + 2, r)
            {
                table[level][i] = table[level][i - 1] + node(a[i]);
            }
        }
        if (l + 1 < r)
        {
            build(level + 1, l, m, a);
            build(level + 1, m, r, a);
        }
    }
public:
    ll query(ll l, ll r)
    {
        r--;
        if (l == r)
        {
            ret table[sz(table) - 1][l].x;
        }
        ll k = __builtin_clz(l ^ r) ^ 31;
        ll lev = maxlev - 1 - k;
        node ans = table[lev][l];
        if (r & ((1 << k) - 1))
            ans = ans + table[lev][r];
        ret ans.x;
    }
};