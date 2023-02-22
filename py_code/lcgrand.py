import ctypes

# load the lcgrand shared library
lcgrand = ctypes.CDLL('./lcgrand.so')

#declare the fucntion prototypes for the lcgrand functions
lcgrand.lcgrand_init.argtypes = [ctypes.c_long]
lcgrand.lcgrand.argtypes = []
lcgrand.lcgrandd.argtypes = []

#define wrapper functions for the lcgrand functions
def PyInit_lcgrand(seed):
    lcgrand.lcgrand_init(seed)

def get_random():
    return lcgrand.lcgrand()

def get_random_double():
    return lcgrand.lcgrandd()