import sys

readline = sys.stdin.readline

n = int(readline())

arr = []

for i in xrange(n):
    val = int(readline())
    arr.append(val)

arr.sort()

def solve():
    global n, arr

    assert n >= 3

    tmp = []

    for i in xrange(n-2):
        for j in xrange(i+1, n-1):
            sumA = arr[i] + arr[j]

            l, r = j + 1, n
            while l < r:
                m = (l+r)>>1
                if sumA + arr[m] < 0:
                    l = m + 1
                else:
                    r = m

            lb = l

            l, r = j + 1, n
            while l < r:
                m = (l+r)>>1
                if sumA + arr[m] <= 0:
                    l = m + 1
                else:
                    r = m

            rb = l

            if lb < rb:
                tmp.append((i, j, (lb, rb)))
            #print '{0}, {1} segment: {2}, {3}'.format(i, j, lb, rb)

    assert tmp

    res = []
    for i in tmp:
        for j in xrange(*i[2]):
            elem = (arr[i[0]], arr[i[1]], arr[j])
            res.append(elem)

    res.sort()

    for i in res:
        print ' '.join(map(str, i))

try:
    solve()
except:
    print 'No Solution'

