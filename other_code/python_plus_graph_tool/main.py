import pickle
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
from graph_tool.all import *
import util
from graph_functions import *
import numpy as np

def timing_test():
    #import time
    import cProfile

    g = make_clique(200)

    tseq = []
    #tpar = []
    #tpar2 = []
    #tt   = []

    for i in range(40):
        if i % 1 == 0: print(i)
        #t0 = time.time()
        a = run_sequential_particles(g, 0)
        #t1 = time.time()
        #tseq.append(t1-t0)

        #t0 = time.time()
        #sequential_to_parallel(a)
        #t1 = time.time()
        #tt.append(t1-t0)

        #t0 = time.time()
        #a = naive_run_parallel_particles(g, 0)
        #t1 = time.time()
        #tpar.append(t1-t0)

        #t0 = time.time()
        #a = queue_run_parallel_particles(g, 0)
        #t1 = time.time()
        #tpar2.append(t1-t0)


    print("average seq time: {}".format(np.mean(tseq)))
    #print("average par time: {}".format(np.mean(tpar)))
    #print("average par2 time: {}".format(np.mean(tpar2)))
    #print("average t time: {}".format(np.mean(tt)))

def load_bars():

    names = ["data/clique/clique_transf_8_10000.ser", "data/clique/clique_transf_64_1000.ser", 
             "data/cycle/cycle_transf_8_10000.ser", "data/cycle/cycle_transf_64_1000.ser"]

    for name in names:
        a = util.load_data(name)
        t = a.mean(axis=0)
        plt.title(name[name.index('e/') + 2 : name.index('.')])
        plt.bar(np.arange(len(t)),t)
        plt.savefig("plots/" + name[name.index('e/') + 2 : name.index('ser')] + '.png')
        plt.close()
    

def run_simulations():

    # Set how many times to simulate, and where to simulate from
    SIMULATION_COUNT = 100
    START_VERTEX = 0

    # Construct the graph
    #g = make_clique(10)
    #g = make_clique(8) 
    g = make_segment(100)

    # We use these to store sequential time, parallel time, and 
    # the locations where we made the cuts in the transformation
    seq_time = []
    par_time = []
    all_append_locations = []
    all_maxes = []

    for i in range(SIMULATION_COUNT):
        if i % 1 == 0: print(i)

        # Get the list of sequential paths
        path_list = run_sequential_particles(g, START_VERTEX)

        # Transform to list of parallel paths, and record append locations
        par_list, max_changes = sequential_to_parallel_count_max(path_list)

        all_maxes.append(max_changes)
        
        # Run the parallel simulation manually
        #par_list = run_parallel_particles(g, START_VERTEX)
        #print("original parallel")
        #util.list_print(par_list2)
        #print()
        #seq_list, _ = parallel_to_sequential(par_list)
        #seq_list_o, _ = parallel_to_sequential2(par_list)
        #print("transformed seq")
        #util.list_print(path_list)
        #print()
        #par_list2, _   = sequential_to_parallel(seq_list)
        #par_list2_o, _ = sequential_to_parallel2(seq_list_o)
        #if par_list == par_list2 and par_list == par_list2_o:
            #"ok"
            #print("ok")
        #else:
            #print("_____________________________________________________________")

        #util.list_print(par_list2)
        #print()
        #util.list_print(path_list)
        #print('\n')
        # Store the results
        #seq_time.append(util.time(path_list))
        #par_time.append(util.time(par_list2))
        #all_append_locations.append(np.array(append_locations))

        # Prepare graph for next simulation
        reset_graph(g) # reset the nodes to empty

    heights = np.bincount(all_maxes)
    plt.bar(range(len(heights)), heights, align="center")
    plt.title("Changes in length local max for path, |v| = 10, n = 1000")

    plt.show()
    #util.list_print(par_time)
    # Save the plots with a name
    #save_plots(seq_time, par_time, 'example/linear.png.png', 'linear', "C8")
    #save_plots(seq_time, par_time, 'example/log.png', 'log', "C8")

    # Save the data with a name
    #save_data(seq_time, par_time, all_append_locations,
            #"example/seq.ser", "example/par.ser", "example/copy_and_past.ser")

if __name__ == "__main__":
    run_simulations()
    #load_bars()
    #timing_test()
