//UVa 12056
#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-8;
const double PI  = acos(-1.0);
const double TAU = 2.0 * PI;
const double INF = 1e99;

int sig(double x) {return x < -EPS ? -1 : x > EPS ? 1 : 0;}
template<class T> T pow2(T x) {return x * x;}

struct Point{
    double x, y;
    Point (double x = 0, double y = 0): x(x), y(y) {}
    void read () { scanf("%lf%lf", &x, &y); }
    void write () { printf("%lf %lf", x, y); }

    bool operator == (const Point& u) const { return dcmp(x - u.x) == 0 && dcmp(y - u.y) == 0; }
    bool operator != (const Point& u) const { return !(*this == u); }
    bool operator < (const Point& u) const { return dcmp(x - u.x) < 0 || (dcmp(x-u.x)==0 && dcmp(y-u.y) < 0); }
    bool operator > (const Point& u) const { return u < *this; }
    bool operator <= (const Point& u) const { return *this < u || *this == u; }
    bool operator >= (const Point& u) const { return *this > u || *this == u; }
    Point operator + (const Point& u) { return Point(x + u.x, y + u.y); }
    Point operator - (const Point& u) { return Point(x - u.x, y - u.y); }
    Point operator * (const double u) { return Point(x * u, y * u); }
    Point operator / (const double u) { return Point(x / u, y / u); }
    double operator * (const Point& u) { return x*u.y - y*u.x; }
};
typedef Point Vector;


class Line {
public:
    Point a, b;
    Line() {}
    Line(Point a, Point b): a(a), b(b) {}
};

class Circle {
public:
    Point o;
    double r;
    Circle() {}
    Circle(Point o, double r): o(o), r(r) {}

    // interior          (d < R - r)         ----> -2
    // interior tangents (d = R - r)         ----> -1
    // concentric        (d = 0)
    // secants           (R - r < d < R + r) ---->  0
    // exterior tangents (d = R + r)         ---->  1
    // exterior          (d > R + r)         ---->  2
    int posi(Circle c) {
        double d = (o - c.o).len();
        int in = sig(d - fabs(r - c.r)), ex = sig(d - (r + c.r));
        return in<0 ? -2 : in==0? -1 : ex==0 ? 1 : ex>0? 2 : 0;
    }

    // IMPORTANT: Ensure this->posi(c) = 0
    // chord(弦) of two circle
    // two points is ccw in *this circle
    Line chord(Circle c) {
        Vector v = c.o - o;
        double co = (pow2(r) + v.len2() - pow2(c.r)) / (2 * r * v.len());
        double si = sqrt(fabs(1.0 - pow2(co)));
        return Line(v.L(co, si).e(r) + o, v.R(co, si).e(r) + o);
    }
};


// -PI <= th <= PI
struct Range {
    double t;
    int evt;
    Point p;
    Range() {}
    Range(double t, int evt, Point p) : t(t), evt(evt), p(p) {}

    bool operator <(const Range &s) const {
        return sig(t - s.t) < 0 || (sig(t - s.t) == 0 && evt > s.evt);
    }
};

const int MAX = 1000 + 10;
Circle C[MAX];
Range R[MAX*2];

// sort circle with desending of radii
bool cmp_r(const Circle &a, const Circle &b) {return a.r > b.r;}
// Area of circular segment(弓形)
double segment_area(double r, double t) {
    return pow2(r) * (t - sin(t)) / 2;
}
double union_circle(Circle C[], int &n)
{
    sort(C, C + n, cmp_r);
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (sig(C[i].r) == 0) break;
        int j = 0;
        for (j = 0; j < k; j++)
            if (C[i].posi(C[j]) < 0 || !sig((C[i].o - C[j].o).len()))
                break;
        if (j == k)
            C[k++] = C[i];
    }
    n = k;

    double ans = 0;
    for (int i = 0; i < n; ++ i)
    {
        Point mpi = Point(- C[i].r, 0.0) + C[i].o;
        int nc = 0, rcnt = 0;
        R[rcnt++] = Range(-PI,  1, mpi);
        R[rcnt++] = Range( PI, -1, mpi);
        for (int j = 0; j < n; ++ j)
        {
            if (j == i || C[i].posi(C[j])) continue;

            Line l = C[i].chord(C[j]);
            double jR = (l.a - C[i].o).ang(), jL = (l.b - C[i].o).ang();

            if (sig(jR - jL) > 0) ++ nc;
            R[rcnt++] = Range(jR,  1, l.a);
            R[rcnt++] = Range(jL, -1, l.b);
        }
        sort(R, R + rcnt);

        double pj = - PI;
        Point  pp = mpi;
        for(int j = 0; j < rcnt; ++ j)
        {
            nc += R[j].evt;
            if((nc == 2 && R[j].evt > 0) || nc == 0)
                ans += segment_area(C[i].r, R[j].t - pj) + (pp ^ R[j].p) / 2;
            pj = R[j].t; pp = R[j].p;
        }
    }
    return ans;
}

int main(){
    int n;
    while (scanf("%d", &n) != EOF) {
        if(n==0) break;
        for (int i = 0; i < n; i++)
            scanf("%lf%lf%lf", &C[i].o.x, &C[i].o.y, &C[i].r);

        double ans = union_circle(C, n);
        printf("%.3lf\n", ans);
    }
    return 0;
}

