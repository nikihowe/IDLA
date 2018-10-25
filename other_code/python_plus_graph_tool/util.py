import pickle
from graph_functions import *

# Nicely print the list of paths taken by all particles (old version)
def vertex_list_print(path_list):
    for path in path_list:
        print("[", end="")
        for node in path:
            print("{}, ".format(int(node)), end="")
        print("]")

# Nicely print the list of paths taken by all particles
def list_print(path_list):
    for path in path_list:
        print(path)

# Return time of longest process
def time(list_of_paths):
    longest = 0
    for path in list_of_paths:
        if len(path) > longest:
            longest = len(path)
    return longest

# Save the seq, par, and copy-paste data
def save_all_data(seq_time, par_time, c_and_p, name1, name2, name3):
    st = np.array(seq_time)
    s = pickle.dumps(st, protocol=0)
    with open(name1, 'wb') as afile:
        afile.write(s)

    pt = np.array(par_time)
    s = pickle.dumps(pt, protocol=0)
    with open(name2, 'wb') as afile:
        afile.write(s)

    ata = np.array(c_and_p)
    s = pickle.dumps(ata, protocol=0) # protocol 0 is printable ASCII
    with open(name3, 'wb') as afile:
        afile.write(s)

# Save data
def save_data(seq_time, name1):
    st = np.array(seq_time)
    s = pickle.dumps(st, protocol=0)
    with open(name1, 'wb') as afile:
        afile.write(s)

# Load data
def load_data(filename):
    with open(filename, 'rb') as f:
        x = pickle.load(f)
    return x

# Save the log and linear scale plots of seq and par time
def save_plots(seq_time, par_time, filename, yscale, graph_desc):
    bins = np.linspace(0, 60, 60)
    # the histogram of the data
    plt.hist(seq_time, bins, normed=0, alpha=0.5, label="seq")
    plt.hist(par_time, bins, normed=0, alpha=0.5, label="par")
    plt.xlabel("Time")
    plt.ylabel("log(Number)")
    plt.title("Sequential and Parallel timings for {}".format(graph_desc))
    plt.grid(True)
    plt.yscale(yscale)
    plt.legend(loc='upper right')
    plt.savefig(filename)
    plt.close()
