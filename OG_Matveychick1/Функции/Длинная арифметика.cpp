str minus1(str a, str b)
{
    rev(all(a));
    rev(all(b));
    while (sz(a) < sz(b))
    {
        a += '0';
    }
    while (sz(b) < sz(a))
    {
        b += '0';
    }
    bool bl = 0;
    str c = "";
    for0(i, sz(a))
    {
        ll x = a[i] - b[i] - bl;
        bl = 0;
        if (x < 0)
        {
            bl = 1;
            x += 10;
        }
        c += x + '0';
    }
    while (ba(c) == '0' && sz(c) > 1)
    {
        c.pob();
    }
    rev(all(c));
    ret c;
}

str plus1(str a, str b)
{
    rev(all(a));
    rev(all(b));
    while (sz(a) < sz(b))
    {
        a += '0';
    }
    a += '0';
    while (sz(b) < sz(a))
    {
        b += '0';
    }
    bool bl = 0;
    str c = "";
    for0(i, sz(a))
    {
        ll x = a[i] + b[i] + bl - 2 * '0';
        bl = 0;
        if (x > 9)
        {
            bl = 1;
            x -= 10;
        }
        c += x + '0';
    }
    while (ba(c) == '0' && sz(c) > 1)
    {
        c.pob();
    }
    rev(all(c));
    ret c;
}

str umn1 (str a, str b)
{
    vs s;
    str pl = "";
    rfor0(i, sz(b))
    {
        str sum = "0";
        for0(j, b[i] - '0')
        {
            sum = plus1(sum, a);
        }
        s.pb(sum + pl);
        pl += '0';
    }
    for0(i, sz(s) - 1)
    {
        s[i + 1] = plus1(s[i], s[i + 1]);
    }
    ret s[sz(s) - 1];
}

str del1(str a, ll b)
{
    str c = "";
    ll x = 0;
    for0(i, sz(a))
    {
        x *= 10;
        x += a[i] - '0';
        c += x / b + '0';
        x %= b;
    }
    ret c;
}