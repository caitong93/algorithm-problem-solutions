#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <climits>
#include <set>
#include <queue>
#include <map>
#include <unordered_map>
#include <cmath>
#include <cassert>
#include <bitset>
#include <stdexcept>
using namespace std;
#define rep(i, s, t) for(int (i)=(s);(i)<=(t);++(i))

const int N = 2e5;

int hi[N+5], L[N+5], R[N+5], ans[N+5], stk[N+5], n;

inline int Max(int a, int b) { return a >= b ? a : b; }

const int BUF_SIZ = 10 * ( 1 << 20 );
char buf[BUF_SIZ], *ptr, sbuf[512];

int readInt() {
    int ret = 0;
    while ( !isdigit(*ptr) ) {
        ++ ptr;
    }
    while ( isdigit(*ptr) ) {
        ret = ret * 10 + ( *ptr - '0' );
        ++ ptr;
    }
    return ret;
}

void writeInt(int x, int term) {
    sprintf(sbuf, "%d", x);
    for(char *i = sbuf; *i; ++ i) {
        *ptr ++ = *i;
    }
    if ( !term ) *ptr ++ = ' ';
}

int main() {
#ifdef _LOCA_ENV_
    freopen("input.in", "r", stdin);
    //freopen("outout.out", "w", stdout);
#endif // _LOCA_ENV

#if 1
    fread(buf, BUF_SIZ, 1, stdin);
    ptr = buf;
    n = readInt();
    rep(i, 1, n) {
        hi[i] = readInt();
    }
#else
    scanf("%d", &n);
    rep(i, 1, n) {
        scanf("%d", hi + i);
    }
#endif // 1

    /*** Main flow ***/

/***  method 1
    int top = 0;
    hi[0] = -INT_MAX;
    stk[top ++] = 0;

    for(int i = 1; i <= n; ++ i) {
        while ( hi[ stk[top - 1] ] >= hi[i] ) {
            -- top;
        }
        L[i] = stk[top - 1];
        stk[top ++] = i;
    }

    top = 0;
    hi[n+1] = -INT_MAX;
    stk[top ++] = n + 1;

    for(int i = n; i >= 1; -- i) {
        while ( hi[ stk[top - 1] ] >= hi[i] ) {
            -- top;
        }
        R[i] = stk[top - 1];
        stk[top ++] = i;
    }
***/

    /*** method 2 ***/
    hi[0] = -INT_MAX;
    hi[n+1] = -INT_MAX;
    int top = 0;
    stk[top ++] = 0;

    for(int i = 1; i <= n + 1; ++ i) {
        while ( hi[ stk[top - 1] ] > hi[i] ) {
            R[ stk[top - 1] ] = i;
            -- top;
        }
        stk[top ++] = i;
    }

    top = 0;
    stk[top ++] = n + 1;

    for(int i = n; i >= 1; -- i) {
        while ( hi[ stk[top - 1] ] > hi[i] ) {
            L[ stk[top - 1] ] = i;
            -- top;
        }
        stk[top ++] = i;
    }

    rep(i, 1, n) {
        int len = R[i] - L[i] - 1;
        ans[len] = Max ( ans[len], hi[i] );
    }

    for(int i = n-1; i >= 1; -- i) ans[i] = Max ( ans[i], ans[i+1] );

#if 1
    ptr = buf;
    for(int i = 1; i < n; ++ i) writeInt(ans[i], 0); writeInt(ans[n], 1);
    fwrite(buf, ptr - buf, 1, stdout);
#else
    for(int i = 1; i < n; ++ i) printf("%d ", ans[i]); printf("%d\n", ans[n]);
#endif // 1

    return 0;
}
