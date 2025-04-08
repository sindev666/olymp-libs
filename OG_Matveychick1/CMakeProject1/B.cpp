#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/rope>

using namespace std;
using namespace __gnu_pbds;
using namespace __gnu_cxx;

// /*
//                                                //////////**DEFINES - START**//////////

#define ret return
#define fi first
#define se second
#define mp make_pair
#define all(x) x.begin(), x.end()
#define be(x) x.begin()
#define en(x) x.end()
#define sz(x) ll(x.size())
#define for0(i, n) for (ll   i = 0; i < (n); ++i)
#define for1(i, n) for (ll   i = 1; i < (n); ++i)
#define rfor0(i, n) for (ll   i = (n) - 1; i >= 0; --i)
#define rfor1(i, n) for (ll   i = (n) - 1; i >= 1; --i)
#define rep(i, a, n) for (ll   i = a; i < ll(n); ++i)
#define rrep(i, a, n) for (ll   i = a - 1; i >= ll(n); --i)
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fastIO() ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
#define con continue
#define pb push_back
#define pob pop_back
#define deb(x) cout << (#x) << " is " << (x) << endl
#define ins insert
#define len(s) (s).length()
#define gi greater<int>()
#define gll greater<ll  >()
#define gstr greater<string>()
#define gpll greater<pair<ll  , ll  >>()
#define rast(x1, y1, x2, y2) sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))
#define rev reverse
#define ub upper_bound
#define lb lower_bound
#define bs binary_search
#define rs resize
#define last(a) a.back()
#define co count
#define ba(a) a.back()
#define um unordered_map
#define rsun(a) a.resize(unique(a.begin(), a.end())-a.begin())
#define endl '\n'
#ifdef OG_Matveychick1
bool local = true;
#else
bool local = false;
#endif

//                                                \\\\\\\\\\**DEFINES - END**\\\\\\\\\\
    // */

// /*
//                                                //////////**TYPEDEFS - START**//////////

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<char> vc;
typedef pair<int, int> pii;
typedef vector<pii> vpii;
typedef vector<string> vs;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<ull> vull;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;
typedef pair<double, double> pdd;
typedef long double ld;
typedef double D;
typedef vector<ld> vld;
typedef vector<pair<ld, ld>>
        vpld;
typedef string str;
typedef set<ll> sll;
typedef set<int> si;
typedef set<str> ss;
typedef set<pii> spii;
typedef multiset<int> msi;
typedef multiset<ll> msll;
typedef multiset<str> mss;
typedef multiset<pii> mspii;
typedef multiset<pll> mspll;
typedef map<str, str> mps;
typedef map<int, int> mpi;
typedef map<ll, ll> mpll;
typedef map<int, vi> mpvi;
typedef map<int, vll> mpvll;
typedef map<char, int> mpci;
typedef multimap<ll, ll> mmpll;
typedef multimap<str, str> mmps;
typedef multimap<int, int> mmpi;
typedef vector<vector<int>> vvi;
typedef vector<vector<ll>> vvll;
typedef vector<vector<long double>> vvld;
typedef vector<vvi> vvvi;
typedef vector<vector<char>> vvc;
typedef vector<vs> vvs;
typedef vector<D> vD;
typedef set<pair<ll, ll>>
        spll;
typedef pair<ull, ull> pull;
typedef vector<pull> vpull;
typedef vector<bool> vb;
typedef vector<vb> vvb;
typedef set<char> sc;
typedef queue<int> qi;
typedef queue<ll> qll;
typedef queue<bool> qb;
typedef vector<sll> vsll;
typedef queue<pair<ll, ll>>
        qpll;
typedef vector<vector<pair<int, int>>>
        vvpii;
typedef vector<vector<pair<ll, ll>>>
        vvpll;
typedef vector<spll> vspll;
typedef multiset<char> msc;
typedef queue<str> qs;
typedef vector<set<int>> vsi;
typedef priority_queue<ll> pqll;
typedef vector<vsll> vvsll;
typedef pair<ld, ld> pld;
typedef vector<vvll> vvvll;
typedef set<ld> sld;
typedef vector<vpld> vvpld;
typedef tree<ll, null_type, less<ll>, rb_tree_tag, tree_order_statistics_node_update>
        ordered_set;
typedef tree<ll, null_type, less_equal<ll>, rb_tree_tag, tree_order_statistics_node_update>
        ordered_multiset;

//                                                \\\\\\\\\\**TYPEDEFS - END**\\\\\\\\\\
    // */


// /*
//                                                //////////**CONSTANTS - START**//////////

const ld pi = acosl(-1);
const ll mod1 = 1e9 + 7;
const ll mod2 = 998244353;
const ll MAXLL = 9223372036854775807;
//const ll MAXINT = 2147483647;
const ld eps = 1e-4;

//                                                \\\\\\\\\\**CONSTANTS - END**\\\\\\\\\\
    // */


// /*
//                                                //////////**TEMPLATES - START**//////////


template<typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for (T &i : a) in >> i;
    return in;
}

template<typename T1, typename T2>
istream &operator>>(istream &in, pair<T1, T2> &a) {
    in >> a.fi >> a.se;
    return in;
}

template<typename T1, typename T2>
ostream &operator<<(ostream &out, pair<T1, T2> &a) {
    out << a.fi << " " << a.se;
    return out;
}

template<typename T1, typename T2>
istream &operator>>(istream &in, vector<pair<T1, T2>>

&a) {
    for (
        pair<T1, T2> &i
            : a)
        in >> i.fi >> i.
                se;
    return
            in;
}

template<typename T>
ostream &operator<<(ostream &out, const vector<T> &a) {
    for (auto i : a) {
        out << i << " ";
    }
    return out;
}

template<typename T1, typename T2>
ostream &operator<<(ostream &out, vector<pair<T1, T2>>

&a) {
    for (
        pair<T1, T2> i
            : a)
        out << i.fi << " " << i.se <<
            endl;
    return
            out;
}

template<typename T1>
ostream &operator<<(ostream &out, vector<vector<T1>> &a) {
    for (vector<T1> i : a) {
        for (T1 j : i) out << j << " ";
        out << endl;
    }
    return out;
}

template<typename T1, typename T2>
inline T1 min(T1 a, T2 b) {
    b = (T1) b;
    return a > b ? b : a;
}

template<typename T1, typename T2>
inline T1 max(T1 a, T2 b) {
    b = (T1) b;
    return a > b ? a : b;
}

template<typename T1, typename T2>
inline void amin(T1 &a, T2 b) {
    a = min(a, b);
}

template<typename T1, typename T2>
inline void amax(T1 &a, T2 b) {
    a = max(a, b);
}


//                                                \\\\\\\\\\**TEMPLATES - END**\\\\\\\\\\
    // */


// This bear is a good alternative to duck!!!
/*
    ????      ??????
  ???????????????????
 ????????????????  ???
???   ???????????  ???
???  ???????????? ??
 ??????????????????
   ??????????????? ?
   ?????????????????
   ???????   ???    ??
    ????  ??????????
     ????     ?? ???
   ???????????? ?????
  ????????????????????
 ????????   ??  ???????
 ???????         ?????
 */


ld getTime() {
    return (ld) clock() / (ld) CLOCKS_PER_SEC;
}


mt19937_64 rn(chrono::steady_clock::now().time_since_epoch().count());
//mt19937_64 rn(4);

ll rnd(ll l, ll r) {
    ll a = rn() % (r - l + 1) + l;
    return a;
}

void solve();

ll T = 1;

signed main(int argc, char **argv) {
//    setlocale(LC_ALL, "RUS");
    fastIO()
    cout.precision(12);
    cout << fixed;
    if (local && argc == 1) {
        freopen("input.txt", "r", stdin);
//        freopen("5.txt", "w", stdout);
    }
//    cin >> T;
    while (T--) {
        solve();
    }
    if (local && argc == 1) {
        cout << endl << fixed << "time = " << getTime();
    }
    return 0;
}


/*
    ___        __              __   ______          __        _____ __             __          __  __
   /   | _____/ /___  ______ _/ /  / ____/___  ____/ /__     / ___// /_____ ______/ /______   / / / /__  ________
  / /| |/ ___/ __/ / / / __ `/ /  / /   / __ \/ __  / _ \    \__ \/ __/ __ `/ ___/ __/ ___/  / /_/ / _ \/ ___/ _ \
 / ___ / /__/ /_/ /_/ / /_/ / /  / /___/ /_/ / /_/ /  __/   ___/ / /_/ /_/ / /  / /_(__  )  / __  /  __/ /  /  __/
/_/  |_\___/\__/\__,_/\__,_/_/   \____/\____/\__,_/\___/   /____/\__/\__,_/_/   \__/____/  /_/ /_/\___/_/   \___/
*/






#pragma GCC optimize("O3")
#pragma GCC target("avx2")

const ll N = 3e5 + 5, K = 20;

ll n, s[N], used[N], ans1[N], ans2[N], in[N], ou[N], ti, sz[N], st[N];
vpll g[N], dn[N], g1[N];
ll ct[N], pct[N], act[N], cct[N];
array<ll, 3> up[K][N];
vector<array<ll, 3>> z[N];
array<ll, 3> e[N];

ll sizes(ll v, ll p = -1) {
    s[v] = 1;
    for (auto [x, w] : g[v]) {
        if (x == p || used[x]) con;
        sizes(x, v);
        s[v] += s[x];
    }
    ret s[v];
}

ll get_centroid(ll v, ll S, ll p = -1) {
    for (auto [x, w] : g[v]) {
        if (x == p || used[x] || s[x] < S / 2) con;
        ret get_centroid(x, S, v);
    }
    ret v;
}

void dfs1(ll v, ll c, ll p, ll cn, ll sm, ll W) {
    in[v] = ti++;
    s[v] = 1;
    up[0][v] = {p, W, W};
    for1(i, K) up[i][v] = {up[i - 1][up[i - 1][v][0]][0],
                           min(up[i - 1][v][1], up[i - 1][up[i - 1][v][0]][1] + up[i - 1][v][2]),
                           up[i - 1][v][2] + up[i - 1][up[i - 1][v][0]][2]};
    if (sm < 0) sm = 0, cn++;
    ans1[c] += sm;
    ans2[c] += cn;
    for (auto [x, w] : g[v]) {
        if (x == p || used[x]) con;
        dfs1(x, c, v, cn, sm + w, w);
        s[v] += s[x];
    }
    ou[v] = ti;
}

ll get(ll v) {
    ll x = 0;
    rfor0(i, K) {
        if (up[i][v][1] >= -x) {
            x += up[i][v][2], v = up[i][v][0];
        }
    }
    ret up[0][v][0];
}

void dfs2(ll v, ll c, ll h, ll p) {
    ct[v] = c;
    pct[v] = h;
    if (v == 22) {
        bool fl = 0;
    }
    if (up[K - 1][v][1] >= 0) {
        act[v] = up[K - 1][v][2];
        cct[v] = 0;
    } else {
        ll u = get(v);
        if (u != c) {
            act[v] = act[u];
            cct[v] = 1 + cct[u];
        } else {
            act[v] = 0;
            cct[v] = 1;
        }
    }
    z[c].pb({v, act[v], cct[v]});
    z[h].pb({v, act[v], cct[v]});
    ans1[v] += act[v];
    for (auto [x, w] : g[v]) {
        if (x == p || used[x]) con;
        dfs2(x, c, h, v);
    }
}

bool cmp(array<ll, 3> a, array<ll, 3> b) {
    ret a[1] < b[1];
}

ll dfs3(ll v, ll p, ll &sum, ll &cnt, ll sm, ll cn, ll u, ll G = -1, ll G1 = -1) {
    dn[v].clear();
    bool fl = 1;
    if (sm < 0) dn[u].pb({sm, v}), sm = 0, cn++, u = v, fl = 0;
    sum += sm, cnt += cn;
    sz[v] = 1;
    if (v == p && G != -1) {
        ll x = G, w = G1;
        sz[v] += dfs3(x, v, sum, cnt, sm + w, cn, u);
    } else {
        for (auto [x, w] : g[v]) {
            if (x == p || used[x]) con;
            sz[v] += dfs3(x, v, sum, cnt, sm + w, cn, u);
        }
    }
    ret (fl ? sz[v] : 0);
}

inline void insert(priority_queue<pll, vpll, greater<pll>> &q, ll v, ll mn, ll x) {
    for (auto [sm, u] : dn[v]) q.push({x - sm - mn, u});
}

void rec1(ll v) {
    sort(all(z[v]), cmp);
    ll sum = 0, cnt = 0;
    priority_queue<pll, vpll, greater<pll>> q;
    dfs3(v, v, sum, cnt, 0, 0, v);
    insert(q, v, 0, 0);
    ll lst = 0;
    for (auto [x, sm, cn] : z[v]) {
        if (x == 22) {
            bool fl = 0;
        }
        sum += (sm - lst) * sz[v];
        while (sz(q) && q.top().fi <= sm) {
            auto [y, u] = q.top();
            q.pop();
            ll mn = sm - y;
            cnt -= s[u];
            sum += sz[u] * mn;
            sz[v] += sz[u];
            insert(q, u, mn, sm);
        }
        ans1[x] += sum;
        ans2[x] += cnt + (s[v] - s[pct[x]]) * cn;
        lst = sm;
    }
    z[v].clear();
}

void rec2(ll v, ll G, ll G1) {
    sort(all(z[G]), cmp);
    ll sum = 0, cnt = 0;
    priority_queue<pll, vpll, greater<pll>> q;
    dfs3(v, v, sum, cnt, 0, 0, v, G, G1);
    insert(q, v, 0, 0);
    ll lst = 0;
    for (auto [x, sm, cn] : z[G]) {
        if (x == 22) {
            bool fl = 0;
        }
        sum += (sm - lst) * sz[v];
        while (sz(q) && q.top().fi <= sm) {
            auto [y, u] = q.top();
            q.pop();
            ll mn = sm - y;
            cnt -= s[u];
            sum += sz[u] * mn;
            sz[v] += sz[u];
            insert(q, u, mn, sm);
        }
        ans1[x] -= sum;
        ans2[x] -= cnt;
        lst = sm;
    }
    z[G].clear();
}

void centroidka(ll v) {
    used[v] = 1;

    dfs1(v, v, v, 0, 0, 0);
    for (auto [x, w] : g[v]) {
        if (used[x]) con;
        dfs2(x, v, x, x);
    }
    rec1(v);
    for (auto [x, w] : g[v]) {
        if (used[x]) con;
        rec2(v, x, w);
    }

    for (auto [x, w] : g[v]) {
        if (used[x]) con;
        centroidka(get_centroid(x, sizes(x)));
    }
}

void rdfs1(ll v, ll p) {
    s[v] = 1;
    for (auto [x, w] : g[v]) {
        if (x == p) con;
        rdfs1(x, v);
        s[v] += s[x];
    }
    sort(all(g[v]), [&](pll x, pll y) { ret s[x.fi] > s[y.fi]; });
}

void rdfs2(ll v, ll p) {
    st[ti] = v;
    in[v] = ti++;
    for (auto [x, w] : g[v]) {
        if (x == p) con;
        rdfs2(x, v);
    }
    ou[v] = ti;
}

void reorder() {
    rdfs1(0, 0);
    rdfs2(0, 0);
    for1(i, n) {
        auto [u, v, w] = e[i];
        g1[in[u]].pb({in[v], w});
        g1[in[v]].pb({in[u], w});
    }
    for0(i, n) swap(g1[i], g[i]);
}

void solve() {
    n = 3e5;
    cin >> n;
    for1(i, n) {
        ll u = rnd(1, i), v = i + 1, w = rnd(-1, 1);
        cin >> u >> v >> w;
        u--;
        v--;
        g[u].pb({v, w});
        g[v].pb({u, w});
        e[i] = {u, v, w};
    }
    reorder();
    centroidka(get_centroid(0, sizes(0)));

    vll a1(n), a2(n);
    for0(i, n) a1[st[i]] = ans1[i], a2[st[i]] = ans2[i];
    cout << a1 << endl << a2;
}