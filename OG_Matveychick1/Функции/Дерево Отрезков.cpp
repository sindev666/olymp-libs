struct ST
{
    struct node;
    ll n;
    vector<node> tree;
    vector<node> lazy;
    struct node
    {
        ll x;
        node() {}
        node(ll x) :x(x) {}
    };
    node neutral = 0;
    friend node operator + (node a, node b)
    {
        ret max(a.x, b.x);
    }
    friend void operator %= (node& a, node b)
    {
        a.x += b.x;
    }
    ST(ll n1)
    {
        n = n1;
        tree.rs(4 * n, neutral);
        lazy.rs(4 * n, neutral);
    }
    void update(ll l, ll r, ll val)
    {
        update(1, 0, n, l, r, val);
    }
    void update(ll pos, ll val)
    {
        update(1, 0, n, pos, val);
    }
    node query(ll l, ll r)
    {
        ret query(1, 0, n, l, r);
    }
    void update(ll v, ll l, ll r, ll pos, ll val)
    {
        tree[v] %= lazy[v / 2];
        lazy[v] %= lazy[v / 2];
        if (l > pos || r <= pos)
        {
            ret;
        }
        if (l == r - 1)
        {
            tree[v] %= val;
            ret;
        }
        ll m = (l + r) / 2;
        update(v * 2, l, m, pos, val);
        update(v * 2 + 1, m, r, pos, val);
        tree[v] = tree[v * 2] + tree[v * 2 + 1];
        lazy[v] = neutral;
    }
    void update(ll v, ll l, ll r, ll L, ll R, ll val)
    {
        tree[v] %= lazy[v / 2];
        lazy[v] %= lazy[v / 2];
        if (l >= R || r <= L)
        {
            ret;
        }
        if (l >= L && r <= R)
        {
            tree[v] %= val;
            lazy[v] %= val;
            ret;
        }
        ll m = (l + r) / 2;
        update(v * 2, l, m, L, R, val);
        update(v * 2 + 1, m, r, L, R, val);
        tree[v] = tree[v * 2] + tree[v * 2 + 1];
        lazy[v] = neutral;
    }
    node query(ll v, ll l, ll r, ll L, ll R)
    {
        tree[v] %= lazy[v / 2];
        lazy[v] %= lazy[v / 2];
        if (l >= L && r <= R)
        {
            ret tree[v];
        }
        if (l >= R || r <= L)
        {
            ret neutral;
        }
        ll m = (l + r) / 2;
        node re = query(v * 2, l, m, L, R) + query(v * 2 + 1, m, r, L, R);
        tree[v] = tree[v * 2] + tree[v * 2 + 1];
        lazy[v] = neutral;
        ret re;
    }
};
