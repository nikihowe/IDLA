import numpy as np
import random
import time

NNODES = 100
NRUNS  = 100
def do_work():

    max_len = 0
    max_filled = -1

    # Run 100 walks on the path
    for i in range(NNODES):
        loc = 0
        length = 0
        while loc <= max_filled:
            if loc == 0:
                loc += 1
            else:
                loc = loc + 1 if random.random() > 0.5 else loc - 1
            length += 1
        max_filled = loc
        if length > max_len:
            max_len = length

    return max_len

if __name__ == "__main__":

    t0 = time.time()
    lengths = []
    for i in range(NRUNS):
        if i % 10 == 0: print(i)
        lengths.append(do_work())
    t1 = time.time()

    print("max", np.max(lengths))
    print("min", np.min(lengths))
    print("ave", np.mean(lengths))
    print("runtime", t1-t0)
