from math import log

class LCG:
    def __init__(self, seed=1):
        self.seed = seed
        self.a = 16807
        self.m = 2147483647
        self.q = self.m // self.a
        self.r = self.m % self.a

    def rand(self):
        hi = self.seed // self.q
        lo = self.seed % self.q
        test = self.a * lo - self.r * hi
        if test > 0:
            self.seed = test
        else:
            self.seed = test + self.m
        return self.seed / self.m

lcg = LCG()
print(-1 * log(lcg.rand()))