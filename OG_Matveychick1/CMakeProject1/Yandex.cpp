#include <bits/stdc++.h>
#include <ext/rope>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;
using namespace __gnu_cxx;

using ll = long long;
using vll = vector<ll>;
using vi = vector<int>;
using pll = pair<ll, ll>;
using vpll = vector<pll>;
using vvll = vector<vll>;
using str = string;
using sll = set<ll>;
using vvi = vector<vi>;
using ld = long double;
using vs = vector<str>;
using ordered_set = tree<ll, null_type, less<ll>, rb_tree_tag, tree_order_statistics_node_update>;
using ordered_multiset = tree<ll, null_type, less_equal<ll>, rb_tree_tag, tree_order_statistics_node_update>;
using vld = vector<ld>;
using vvld = vector<vld>;
using spll = set<pll>;
using vsll = vector<sll>;
using ull = uint64_t;
using vull = vector<ull>;
using pii = pair<int, int>;
using vpii = vector<pii>;
using vvpii = vector<vpii>;
using vb = vector<bool>;
using vvpll = vector<vpll>;
using vmsll = vector<multiset<ll>>;
using si = set<int>;
using vsi = vector<set<int>>;
using qll = queue<ll>;
using vc = vector<char>;
using vvc = vector<vc>;
using qpll = queue<pll>;
using vvs = vector<vs>;

#ifdef OG_Matveychick1
bool local = 1;
#else
bool local = 0;
#endif

const ll mod1 = 1e9 + 7;
const ll mod2 = 998244353;
const ll MAXLL = 9223372036854775807;
const ld eps = 1e-9;


template<typename T>
auto be(vector<T> &x) {
    return x.begin();
}

template<typename T>
auto be(set<T> &x) {
    return x.begin();
}

template<typename T>
auto en(vector<T> &x) {
    return x.end();
}

template<typename T>
auto en(set<T> &x) {
    return x.end();
}


template<typename T>
void reverse(vector<T> &x) {
    reverse(x.begin(), x.end());
}

template<typename T>
void fill(vector<T> &x, ll val) {
    fill(x.begin(), x.end(), val);
}

template<typename T>
ll sz(vector<T> &x) {
    return x.size();
}

ll sz(str &x) {
    return x.size();
}

template<typename T>
void rsun(vector<T> &x) {
    x.resize(unique(x.begin(), x.end()) - x.begin());
}

template<typename T>
ll sz(multiset<T> &x) {
    return x.size();
}

template<typename T>
ll sz(set<T> &x) {
    return x.size();
}

template<typename T>
T &ba(vector<T> &x) {
    return x.back();
}

template<typename T1, typename T2>
istream &operator>>(istream &in, pair<T1, T2> &a) {
    in >> a.first >> a.second;
    return in;
}

template<typename T1, typename T2>
ostream &operator<<(ostream &out, pair<T1, T2> &a) {
    out << a.first << ' ' << a.second << endl;
    return out;
}

template<typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for (int i = 0; i < a.size(); ++i) in >> a[i];
    return in;
}

template<typename T>
ostream &operator<<(ostream &out, const vector<T> &a) {
    for (auto i : a) {
        out << i << " ";
    }
    return out;
}

template<typename T>
ostream &operator<<(ostream &out, const vector<vector<T>> &a) {
    for (auto i : a) {
        out << i << endl;
    }
    return out;
}

template<typename T1, typename T2>
inline void amin(T1 &a, T2 b) {
    a = min(a, b);
}

template<typename T1, typename T2>
inline void amax(T1 &a, T2 b) {
    a = max(a, b);
}

double getTime() {
    return clock() / (double) CLOCKS_PER_SEC;
}

mt19937_64 rn(chrono::steady_clock::now().time_since_epoch().count());
//mt19937_64 rn(5);

ll rnd(ll l, ll r) {
    ll a = rn() % (r - l + 1) + l;
    return a;
}

void solve();

ll T = 1;

signed main(int argc, char **argv) {
    setlocale(LC_ALL, "Russian");
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cout.precision(12);
    cout << fixed;
    if (local && argc == 1) {
        freopen("input.txt", "r", stdin);
//        freopen("wrong.txt", "w", stdout);
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


struct line {
    ld k, b;

    line() {}

    line(ll k, ll b) : k(k), b(b) {}

    bool operator<(line &l) {
        return
                k > l.k;
    }

    ld intersection_x(line &l) {
        return (l.b - b) / (k - l.k);
    }
};

void solve() {

}
