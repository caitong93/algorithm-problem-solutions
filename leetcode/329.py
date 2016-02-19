from itertools import product

class Solution(object):
    def longestIncreasingPath(self, matrix):
        """
        :type matrix: List[List[int]]
        :rtype: int
        """
        if not matrix:
            return 0

        def memo(f):
            cache = {}
            def wrap(i, j):
                key = i, j
                #print cache
                #print key
                ret = cache.get(key)
                if not ret:
                    ret = f(i, j)
                    cache[key] = ret
                return ret
            return wrap
        
        rows = len(matrix)
        cols = len(matrix[0])
        
        dt = [i for i in product([-1, 0, 1], [-1, 0, 1])]

        @memo
        def dp(x, y):
            ans = 1
            for dx, dy in dt:
                if abs(dx + dy) == 1:
                    nx, ny = x + dx, y + dy
                    if 0 <= nx and 0 <= ny and nx < rows and ny < cols \
                        and matrix[nx][ny] < matrix[x][y]:
                        ans = max(ans, dp(nx, ny) + 1)
            return ans

        ret = 1
        for x, y in product(xrange(rows), xrange(cols)):
            ret = max(ret, dp(x, y))

        return ret

# print Solution().longestIncreasingPath([[9,9,4],[6,6,8],[2,1,1]])
            