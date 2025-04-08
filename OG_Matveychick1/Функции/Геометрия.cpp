using num = ld;
struct vec {
    num x, y;

    vec(num x, num y) : x(x), y(y) {}

    vec(const vec &a) : x(a.x), y(a.y) {}

    vec() : x(0), y(0) {}

    void operator+=(vec a) {
        x += a.x;
        y += a.y;
    }

    vec operator+(vec a) {
        ret {x + a.x, y + a.y};
    }

    void operator-=(vec a) {
        x -= a.x;
        y -= a.y;
    }

    vec operator-(vec a) {
        ret {x - a.x, y - a.y};
    }

    void operator*=(vec a) {
        x *= a.x;
        y *= a.y;
    }

    vec operator*(num a) {
        ret {x * a, y * a};
    }

    num operator*(vec a) {
        ret x * a.x + y * a.y;
    }

    num operator*=(ld a) {
        x *= a;
        y *= a;
    }

    num operator%(vec a) {
        ret x * a.y - y * a.x;
    }

    bool operator == (vec a){
        ret a.x==x && a.y==y;
    }

    num len2() {
        ret x * x + y * y;
    }

    friend istream &operator>>(istream &in, vec &a) {
        in >> a.x >> a.y;
        ret in;
    }

    friend ostream &operator<<(ostream &out, vec a) {
        out << a.x << " " << a.y;
        ret out;
    }

    num napr(vec a) {
        if ((*this) % a > 0)ret 1;
        else if ((*this) % a < 0)ret -1;
        else
            ret 0;
    }

    ld angle(vec a) {
        ret atan2l(a % *this, a * *this);
    }

    ld polar_angle() {
        ret atan2(y, x);
    }

    bool point_in_ray(vec a) //точка
    {
        ret (abs(*this % a - 0)<eps && *this * a >= -eps);
    }

    bool point_in_square_ray(vec a) //точка
    {
        ret (*this * a >= 0);
    }

    bool point_in_line(vec a) //точка
    {
        ret (*this % a == 0);
    }

    bool point_in_segment(vec a) //точка
    {
        ret (abs(*this % a - 0) < eps && *this * a >= -eps && vec(0 - x, 0 - y) * vec(a.x - x, a.y - y) >= -eps);
    }

    bool point_in_square_segment(vec a) //точка
    {
        ret (*this * a >= 0 && vec(0 - x, 0 - y) * vec(a.x - x, a.y - y) >= 0);
    }

    bool point_in_angle(vec a, vec b) //две крайние точки угла
    {
        ret (a.napr(b) * a.napr(*this) >= 0 && b.napr(a) * b.napr(*this) >= 0);
    }

    ld dist_from_point_to_line(vec a) //точка
    {
        ret *this % a / sqrt(len2());
    }

    ld dist_from_point_to_ray(vec a) //точка
    {
        ret (point_in_square_ray(a) ? *this % a / sqrt(len2()) : min(sqrt(vec(a).len2()), sqrt(vec(a - *this).len2())));
    }

    ld dist_from_point_to_segment(vec a) //точка
    {
        ret (point_in_square_segment(a) ? *this % a / sqrt(len2()) : min(sqrt(vec(a).len2()),
                                                                         sqrt(vec(a - *this).len2())));
    }

    void turn() {
        swap(x, y);
        x = -x;
    }

    void turn(ld a){
        *this=vec(x*cos(a)-y*sin(a),x*sin(a) + y*cos(a));
    }

    friend bool segment_in_segment(vec a1, vec a2, vec b1, vec b2) {
        vec a(a2.x - a1.x, a2.y - a1.y), b(a1.x - a2.x, a1.y - a2.y), al(b1.x - a1.x, b1.y - a1.y), ar(b2.x - a1.x, b2.y - a1.y), bl(b1.x - a2.x, b1.y - a2.y), br(b2.x - a2.x, b2.y - a2.y);
        vec c(b2.x - b1.x, b2.y - b1.y), d(b1.x - b2.x, b1.y - b2.y), cl(a1.x - b1.x, a1.y - b1.y), cr(a2.x - b1.x, a2.y - b1.y), dl(a1.x - b2.x, a1.y - b2.y), dr(a2.x - b2.x, a2.y - b2.y);
        if ((a % al) * (a % ar) <= 0 && (b % bl) * (b % br) <= 0 && (c % cl) * (c % cr) <= 0 && (d % dl) * (d % dr) <= 0)
        {
            if (a % al == 0 && a % ar == 0 && (max(a1.x, a2.x) < min(b1.x, b2.x) || max(b1.x, b2.x) < min(a1.x, a2.x) || max(a1.y, a2.y) < min(b1.y, b2.y) || max(b1.y, b2.y) < min(a1.y, a2.y)))
            {
                ret 0;
            }
            ret 1;
        }
        else
        {
            ret 0;
        }
    }

    friend ld dist_from_segment_to_segment(vec a1, vec a2, vec b1, vec b2) {
        ret (segment_in_segment(a1, a2, b1, b2) ? 0.0 : min(abs(vec(a2 - a1).dist_from_point_to_segment(vec(b1 - a1))),
                                                            min(abs(vec(a2 - a1).dist_from_point_to_segment(
                                                                        vec(b2 - a1))),
                                                                min(abs(vec(b2 - b1).dist_from_point_to_segment(
                                                                            a1 - b1)),
                                                                    abs(vec(b2 - b1).dist_from_point_to_segment(
                                                                            a2 - b1))))));
    }
};

struct line {
    num a, b, c;
    line(){}
    line(vec x, vec y) {
        a=y.y-x.y;
        b=x.x-y.x;
        c=-a*x.x-b*x.y;
    }
    line(num aa, num bb, vec cc)
    {
        a=aa;
        b=bb;
        c=-a*cc.x-b*cc.y;
    }
    line(num a, num b, num c):a(a),b(b),c(c){}
    friend ostream& operator<<(ostream& out, line a)
    {
        out<<a.a<<" "<<a.b<<" "<<a.c;
        ret out;
    }
    friend istream& operator>>(istream& in, line& a)
    {
        in>>a.a>>a.b>>a.c;
        ret in;
    }
    bool point_in_line(vec aa)
    {
        ret (a*aa.x+b*aa.y+c==0);
    }
    num napr(vec aa)
    {
        if (a*aa.x+b*aa.y+c<0)ret -1;
        else if (a*aa.x+b*aa.y+c>0) ret 1;
        else ret 0;
    }
    ld dist_from_point_to_line (vec aa) //точка
    {
        ret (a*aa.x+b*aa.y+c)/sqrt(vec(a,b).len2());
    }
    vec point_of_intersection_of_lines(line aa)
    {
        if (a!=0)ret vec((-b*(aa.c*a-c*aa.a) / (b*aa.a - aa.b*a)-c) / a,(aa.c*a-c*aa.a) / (b*aa.a - aa.b*a));
        else ret vec((-aa.b*(c*aa.a-aa.c*a) / (aa.b*a - b*aa.a)-aa.c) / aa.a,(c*aa.a-aa.c*a) / (aa.b*a - b*aa.a));
    }
    bool lines_is_parallel (line aa)
    {
        if (a==0)
        {
            if (aa.a != 0) ret 0;
            ret 1;
        }
        else
        {
            if (b==0)
            {
                if (aa.b != 0) ret 0;
                ret 1;
            }
            ret abs(aa.a/a-aa.b/b) < eps;
        }
    }
    ld dist_of_parallel_lines(line aa)
    {
        vec z(a,b);
        ld d=(-c)/sqrt(z.len2());
        z*=d/sqrt(z.len2());
        ret aa.dist_from_point_to_line(z);
    }
};
struct circle
{
    num r;
    vec t;
    circle(){}
    circle(num x, num y, num r):t(vec(x,y)),r(r){}
    bool point_in_circle(vec a)
    {
        ret sqrt(vec(a-vec(t.x, t.y)).len2()) <= r;
    }
    friend istream& operator>> (istream& in, circle& a)
    {
        cin>>a.t.x>>a.t.y>>a.r;
        ret in;
    }
    ld ln()
    {
        ret pi*2*r;
    }
};
line bisector_of_three_points(vec x, vec y, vec z)
{
    y-=x;
    z-=x;
    vec X=vec(y-z)*(sqrt(z.len2())/(sqrt(z.len2())+sqrt(y.len2())));
    X+=z;
    X+=x;
    ret line(x,X);
}
ll area_of_triangle(vec a, vec b, vec c)
{
    ret vec(b-a)%vec(c-a);
}
ll area_of_polygon(vector<vec>& a)
{
    ll sum=0;
    rep(i,2,sz(a))
    {
        sum+= area_of_triangle(a[0],a[i-1],a[i]);
    }
    ret sum;
}
bool point_in_polygon(ll n, vec p, vector<vec>& a)
{
    a.pb(a[0]);
    for0(i,n)
    {
        if (a[i]==p)
        {
            ret 1;
        }
    }
    for1(i,n+1)
    {
        if (vec(a[i-1]-a[i]).point_in_segment(p-a[i]))
        {
            ret 1;
        }
    }
    ld sum=0;
    for1(i,n+1)
    {
        sum+=vec(a[i]-p).angle(a[i-1]-p);
    }
    ret(abs(abs(sum)-pi*2)<eps?1:0);
}
bool convex_polygon(vector<vec> a)
{
    a.pb(a[0]);
    a.pb(a[1]);
    ll n=sz(a);
    bool mi=0,ma=0;
    rep(i,2,n)
    {
        if (vec(a[i-1]-a[i-2])%vec(a[i]-a[i-2])>0)ma=1;
        if (vec(a[i-1]-a[i-2])%vec(a[i]-a[i-2])<0)mi=1;
    }
    ret !(mi&&ma);
}
bool f(pair<vec,ll>& a, pair<vec,ll>& b)
{
    ret a.fi.polar_angle()<b.fi.polar_angle();
}