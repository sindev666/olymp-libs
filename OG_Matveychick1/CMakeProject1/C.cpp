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




const ll N = 1005;

ll n, a[N][N], m[N];
vvvll dp1;

ll get(ll x, ll y) {
    ret (x - y) * (x - y);
}

ll get(ll i, ll x1, ll x2) {
    vll b{0, (ll) 2e6};
    rep(j, x1 - 1, m[i]) b.pb(a[i][j]);
    for0(j, x2 - 1) b.pb(a[(i + 1) % n][j]);
    sort(all(b));
    ll ans = 0;
    for1(j, sz(b)) ans += get(b[j], b[j - 1]);
    ret ans;
}

void add(msll &s, ll &sm, ll x) {
    auto itl = prev(s.ub(x)), itr = s.lb(x);
    sm -= get(*itl, *itr);
    sm += get(*itl, x) + get(x, *itr);
    s.ins(x);
}

void prec() {
    dp1.rs(n);
    for0(i, n) {
        dp1[i].rs(m[i] + 1, vll(m[(i + 1) % n] + 1));
        for1(xi, m[i] + 1) {
            msll s;
            s.ins(0);
            s.ins(2e6);
            ll sm = (ll) 2e6 * (ll) 2e6;
            rep(j, xi - 1, m[i]) add(s, sm, a[i][j]);
            for1(xn, m[(i + 1) % n] + 1) {
                dp1[i][xi][xn] = sm;
                add(s, sm, a[(i + 1) % n][xn - 1]);
            }
        }
    }
}

void solve() {
    cin >> n;
    for0(i, n) {
        cin >> m[i];
        for0(j, m[i]) cin >> a[i][j];
    }
    prec();
    vvvll dp(n);
    for0(i, n) dp[i].rs(m[i] + 1, vll(m[0] + 1));
    for0(i, n) for0(j, m[i] + 1) for0(k, m[0] + 1) dp[i][j][k] = 1e18;
    for1(x, m[0] + 1) dp[0][x][x] = 0;
    ll ans = 1e18;
    for1(i, n) {
        rep(xp, max(1, m[i - 1] / 2 - 10), min(m[i - 1] + 1, m[i - 1] / 2 + 10)) {
            for1(x0, m[0] + 1) {
                for1(xi, m[i] + 1) {
                    ll cst = dp[i - 1][xp][x0] + dp1[i - 1][xp][xi] + (i + 1 == n ? dp1[n - 1][xi][x0] : 0);
                    amin(dp[i][xi][x0], cst);
                    if (i + 1 == n) amin(ans, cst);
                }
            }
        }
    }
    cout << ans << endl;
}