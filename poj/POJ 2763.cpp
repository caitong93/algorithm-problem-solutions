#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <climits>
#include <set>
#include <queue>
#include <map>
#include <cmath>
#include <cassert>
#include <bitset>
#include <stdexcept>
using namespace std;
#define rep(i, s, t) for(int (i)=(s);(i)<=(t);++(i))
const int N = 100005;
typedef long long ll;
const ll Mod = 1e9 + 7;

namespace BIT {
    const int Limit = N*2;
    int c[Limit + 1];

    void init() { memset(c, 0, sizeof(c)); }

    int sum(int x) {
        int ret = 0;
        while ( x > 0 ) {
            ret += c[x];
            x -= x & (-x);
        }
        return ret;
    }

    void upd(int x, int v, int n) {
        while ( x <= n ) {
            c[x] += v;
            x += x & (-x);
        }
    }
}

int vs[N*2], id[N], dep[N*2], w[N], in[N], out[N], f[N*2][20], n, timer;
struct Edge { int id, v, nxt; } E[N*2];
int head[N], enm;

void add_edge(int u, int v, int i) {
    ++ enm;
    E[enm] = (Edge) {i, v, head[u]};
    head[u] = enm;
}

void dfs(int fa, int u, int d) {
    vs[timer] = u;
    id[u] = timer;
    dep[timer ++] = d;

    for(int i = head[u]; i != -1; i = E[i].nxt) {
        const Edge& e = E[i];
        if ( e.v == fa ) continue;
        BIT::upd(timer, w[e.id], n << 1);
        in[e.id] = timer;
        dfs(u, e.v, d+1);
        vs[timer] = u;
        BIT::upd(timer, -w[e.id], n << 1);
        out[e.id] = timer;
        dep[timer ++] = d;
    }
}

int my_min(int a[], int x, int y) {
    if ( a[x] < a[y] ) return x; return y;
}

void rmq_init(int a[], int n) {
    rep(i, 0, n-1) f[i][0] = i;
    for(int k = 1, len = 1; (1 << k) < n; ++ k, len <<= 1)
        for(int i = 0; i + ( 1 << k) <= n; ++ i ) {
            f[i][k] = my_min( a, f[i][k-1], f[i+len][k-1] );
        }
}

int rmq_query(int a[], int u, int v) {
    if ( u > v ) swap(u, v);
    int len = v - u + 1;
    int k = log2(len);
    return my_min( a, f[u][k], f[ v - (1 << k) + 1 ][k] );
}

int lca(int u, int v) {
    return vs[ rmq_query( dep + 1, id[u] - 1, id[v] - 1 ) + 1 ];
}

int get_path_value(int u, int v) {
    int x = lca(u, v);
    return BIT::sum( id[u] ) + BIT::sum( id[v] ) -  BIT::sum( id[x] ) * 2;
}

int main() {
#ifdef _LOCA_ENV_
    freopen("input.in", "r", stdin);
#endif // _LOCA_ENVw
    int q, s;
    while ( scanf("%d%d%d", &n, &q, &s) != EOF ) {
        enm = 0;
        memset(head, -1, sizeof(head));
        rep(i, 1, n-1) {
            int u, v;
            scanf("%d%d%d", &u, &v, &w[i]);
            add_edge(u, v, i);
            add_edge(v, u, i);
        }

        int m = 2 * n - 1;
        BIT::init();
        timer = 1;
        dfs(-1, 1, 0);

        rmq_init(dep + 1, m);

        int x = s, y, z;
        rep(i, 1, q) {
            int op;
            scanf("%d", &op);
            if ( op == 0 ) {
                scanf("%d", &y);
                int ans = get_path_value(x, y);
                x = y;
                printf("%d\n", ans);
            } else {
                scanf("%d%d", &y, &z);
                BIT::upd( in[y], z - w[y], n << 1);
                BIT::upd( out[y], w[y] - z, n << 1 );
                w[y] = z;
            }
        }
    }

    return 0;
}
