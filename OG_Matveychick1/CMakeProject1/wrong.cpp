#include <bits/stdc++.h>

using type = long double;
using namespace std;

const double PI = atan2((double) 0, (double) -1);

const double eps = 1e-12;
const double inf = 1e+12;

bool equal(double a, double b) {
    return abs(a - b) <= eps;
}

bool less_(double a, double b) {
    return a < b - eps;
}

bool greater_(double a, double b) {
    return a > b + eps;
}


struct point {
    double x, y;

    point(double a, double b) :
            x(a),
            y(b) {}

    point() :
            point(0, 0) {}

    point to(point v) {
        return {v.x - x, v.y - y};
    }

    point operator+(point v) {
        return {v.x + x, v.y + y};
    }

    point operator-(point v) {
        return {x - v.x, y - v.y};
    }

    double operator*(point v) {
        return x * v.x + y * v.y;
    }

    double operator%(point v) {
        return x * v.y - y * v.x;
    }

    double angle() {
        return atan2(y, x);
    }

    friend double angle(point a, point b) {
        return atan2(a % b, a * b);
    }

    double len2() {
        return x * x + y * y;
    }

    double len() {
        return sqrt(len2());
    }

    double dist2(point p) {
        return to(p).len2();
    }

    double dist(point p) {
        return sqrt(dist2(p));
    }

    bool small(point a, point b) {
        return !less_(to(a) * to(b), 0);
    }

    bool operator==(point p) {
        return equal(x, p.x) && equal(y, p.y);
    }

    bool operator!=(point p) {
        return !(operator==(p));
    }

    bool lt(point a, point b) {
        return greater_(to(a) % to(b), 0);
    }

    bool rt(point a, point b) {
        return less_(to(a) % to(b), 0);
    }

    friend istream &operator>>(istream &in, point &p) {
        return in >> p.x >> p.y;
    }

    friend ostream &operator<<(ostream &out, point p) {
        return out << p.x << " " << p.y;
    }
};

struct line {
    point f, s;

    line(point a, point b) :
            f(a),
            s(b) {
        assert(a != b);
    }

    line() :
            f(),
            s() {}

    double dist2(point p) {
        double sq = p.to(f) % p.to(s);
        return sq * sq / f.dist2(s);
    }

    double dist(point p) {
        return p.to(f) % p.to(s) / f.dist(s);
    }

    bool have(point p) {
        return equal(f.to(p) % f.to(s), 0);
    }
};

struct segment {
    point f, s;

    segment(point a, point b) :
            f(a),
            s(b) {}

    segment() :
            f(),
            s() {}

    double dist2(point p) {
        if (f == s) {
            return f.dist2(p);
        }
        if (f.small(p, s) && s.small(p, f)) {
            return line(f, s).dist2(p);
        } else {
            return min(p.dist2(f), p.dist2(s));
        }
    }

    double dist(point p) {
        return sqrt(dist2(p));
    }

    bool have(point p) {
        return line(f, s).have(p) && !greater_(p.to(f) * p.to(s), 0);
    }

    bool check_intersect(segment o) {
        if (have(o.f) || have(o.s) || o.have(f) || o.have(s)) {
            return true;
        }
        point a = f;
        point b = s;
        point c = o.f;
        point d = o.s;
        return less_((a.to(b) % a.to(c)) * (a.to(b) % a.to(d)), 0) &&
               less_((c.to(d) % c.to(a)) * (c.to(d) % c.to(b)), 0);
    }

    double dist2(segment o) {
        if (check_intersect(o)) {
            return 0;
        }
        double a = dist2(o.f);
        double b = dist2(o.s);
        double c = o.dist2(f);
        double d = o.dist2(s);
        return min({a, b, c, d});
    }

    double dist(segment o) {
        return sqrt(dist2(o));
    }
};

const int maxn = 2e5 + 64;
int n, k;
point p[maxn];
double d[maxn];

point getp(int i) {
    i = (i + n) % n;
    return p[i];
}

bool vis(point q, int i) {
    return q.rt(p[i], p[i + 1]);
}

int left_tan(point q) {
    if (!vis(q, n - 1) && vis(q, 0)) {
        return 0;
    }
    int l = 0, r = n;
    if (vis(q, 0)) {
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (q.lt(p[0], p[m])) {
                if (vis(q, m)) {
                    r = m;
                } else {
                    l = m;
                }
            } else {
                l = m;
            }
        }
    } else {
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (q.lt(p[0], p[m])) {
                if (vis(q, m)) {
                    r = m;
                } else {
                    l = m;
                }
            } else {
                r = m;
            }
        }
    }
    assert(r < n);
    return r;
}

int right_tan(point q) {
    if (vis(q, n - 1) && !vis(q, 0)) {
        return 0;
    }
    int l = 0, r = n - 1;
    if (vis(q, 0)) {
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (q.rt(p[0], p[m])) {
                if (vis(q, m)) {
                    l = m;
                } else {
                    r = m;
                }
            } else {
                r = m;
            }
        }
    } else {
        while (r - l > 1) {
            int m = (l + r) / 2;
            //cerr << "m = " << m << endl;
            if (q.rt(p[0], p[m])) {
                if (vis(q, m)) {
                    l = m;
                } else {
                    r = m;
                }
            } else {
                l = m;
            }
        }
    }
    assert(r < n);
    return r;
#undef vis
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout << fixed << setprecision(15);
    cerr << fixed << setprecision(15);

    cin >> n >> k;
    --k;
    for (int i = 0; i < n; ++i) {
        cin >> p[i];
        p[i + n] = p[i];
    }
    fill_n(d, maxn, inf);
    d[k] = 0;
    for (int i = k + 1; i < n; ++i) {
        d[i] = min(d[i], d[i - 1] + p[i].dist(p[i - 1]));
    }
    d[0] = min(d[0], d[n - 1] + p[0].dist(p[n - 1]));
    for (int i = 1; i < k; ++i) {
        d[i] = min(d[i], d[i - 1] + p[i].dist(p[i - 1]));
    }
    for (int i = k - 1; i >= 0; --i) {
        d[i] = min(d[i], d[i + 1] + p[i].dist(p[i + 1]));
    }
    d[n - 1] = min(d[n - 1], d[0] + p[n - 1].dist(p[0]));
    for (int i = n - 2; i > k; --i) {
        d[i] = min(d[i], d[i + 1] + p[i].dist(p[i + 1]));
    }

    int m;
    cin >> m;
    while (m--) {
        point q;
        cin >> q;
        int i = left_tan(q);
        int j = right_tan(q);
        if (!q.lt(p[k], getp(k + 1))) {
            cout << q.dist(p[k]) << "\n";
        } else {
            cout << min(q.dist(p[i]) + d[i], q.dist(p[j]) + d[j]) << "\n";
        }
    }

    /*point q;
    while (cin >> q) {
        cout << right_tan(q) << endl;
    }*/

    return 0;
}
