# Libraries
import numpy as np
import copy

# Niki files
from graph_constructors import *
import util
import time

# We store g as a 2-level np array (adjacency list)

# Simulate the random walk of particle number particle_number
# starting at start on graph,
# and save the trajectory to path_list
def generate_and_save_path(g, start, is_full):
    current = start
    path_list = [start]

    while is_full[current]:
        # Go to a random neighbour
        current = np.random.choice(g[current])

        # Record we were at this vertex
        path_list.append(current)
    is_full[current] = True

    return path_list

def generate_and_save_path_fast(g, start, is_full):
    current = start
    length = 0

    while is_full[current]:
        #print(g.vp["full"][current])
        #print(current)
        #print()
        # Go to a random neighbour
        current = np.random.choice(g[current])

        # Update path length
        length += 1
    is_full[current] = True

    return length

"""

def generate_and_save_path_big(i, n, g, start, neighbour_cache, length_pool):
    current = start
    length = 0

    assert n * n == g
    assert True

    while g.vp["full"][current]:
        #print(g.vp["full"][current])
        #print(current)
        #print()

        # Check the speedup condition
        if (i > n  and i % n == 0
                and g.vp["full"][i + n] == True):

            move_r = np.random.choice([False, True])
            current = g.get_vertices[i + n] if move_r else g.get_vertices[i - n]

            time_taken = np.random.choice(length_pool)
            length += time_taken

        else:
            # Go to a random neighbour
            current = np.random.choice(neighbour_cache[current])

            # Update path length
            length += 1

    g.vp["full"][current] = True

    return length

"""

# Run the sequential simulation
def run_sequential_particles(g, start_vertex):
    #t0 = time.time()

    list_of_particle_paths = []
    is_full = [False for e in g]

    for e in g:
        list_of_particle_paths.append(
                generate_and_save_path(
                    g, start_vertex, is_full))

    #t1 = time.time()
    #print("fast time:", t1-t0)
    return list_of_particle_paths

# Run the sequential simulation
def run_sequential_particles_fast(g, start_vertex, big=False, filename=""):

    list_of_particle_lengths = []
    is_full = [False for e in g]

    if big:
        n = int(sqrt(len(g)))
        length_pool = util.load_data(filename)
        for i,e in enumerate(g):
            list_of_particle_lengths.append(
                    generate_and_save_path_big(
                        i, g, start_vertex, filename))
    else:
        for e in enumerate(g):
            list_of_particle_lengths.append(
                    generate_and_save_path_fast(g, start_vertex, is_full))

    return list_of_particle_lengths

# Run the sequential simulation
def run_sequential_particles_hyper(g, start_vertex):

    max_len = 0
    is_full = [False for e in g]

    for e in enumerate(g):
        length = generate_and_save_path_fast(g, start_vertex, is_full)
        if length > max_len:
            max_len = length

    return max_len

# Run single path
def run_single_path(g, is_full):

    length = generate_and_save_path_fast(g, 0, is_full)
    return length

"""

# Run the parallel simulation
def run_parallel_particles_naive(g, start_vertex):

    is_full = []
    path_list = [[] for i in range(g.num_vertices())]

    # Record which particles are still active
    active_list = [True for i in range(g.num_vertices())]
    location_list = [0 for i in range(g.num_vertices())]

    # Record how many holes we still need to fill
    num_empty = g.num_vertices()

    while num_empty > 0:
        for i, cur_loc in enumerate(location_list):
            if active_list[i]:
                path_list[i].append(cur_loc)
                # Settle if you can
                if not g.vp["full"][cur_loc]:
                    g.vp["full"][cur_loc] = True
                    active_list[i] = False
                    num_empty -= 1
                    continue

                # Take one step
                location_list[i] = np.random.choice(neighbour_cache[cur_loc])

    reset_graph(g)
    return path_list

# Run the parallel simulation
def run_parallel_particles(g, start_vertex):
    from collections import deque

    # Important because otherwise would return
    # a newly created np array each time (slow)
    neighbour_cache = get_neighbour_cache(g, g.get_vertices())

    # To record the paths the particles take
    path_list = [[] for i in range(g.num_vertices())]

    # Use a tuple to record which particles are where
    start_locations = [(int(i), start_vertex) for i in g.vertices()]

    # We'll get the next particle to move from this queue
    part_q = deque(start_locations)

    while part_q: # while not empty

        # Get the next particle and location, and record it
        part, loc = part_q.popleft()
        path_list[part].append(loc)

        # Settle if you can
        if not g.vp["full"][loc]:
            g.vp["full"][loc] = True
            continue

        # Take one step
        next_loc = np.random.choice(neighbour_cache[loc])
        part_q.append((part, next_loc))

    reset_graph(g)
    return path_list

# Perform the sequentual to parallel transformation
def sequential_to_parallel(seq_path_list):
    
    path_list = copy.deepcopy(seq_path_list)

    # For each node, which particle ended up here?
    resting_place = {} # location # : particle #
    for i in range(len(path_list)):
        resting_place[path_list[i][-1]] = i
    #util.list_print(path_list)
    #print(resting_place)

    # Remember whether we have seen this node yet
    have_seen = [False for i in range(len(path_list))]

    times_appended = [0 for i in range(len(path_list))]

    j = 0
    while not all(have_seen):
        for i, seq_path in enumerate(path_list):
            #print("j = {}, i = {}".format(j, i))
            if j < len(seq_path):
                cur_loc = seq_path[j]
                if not have_seen[cur_loc]:
                    # Perform the cut-and-paste op if necessary
                    if j + 1 < len(seq_path):
                        path_list[resting_place[cur_loc]] += seq_path[j+1:]
                        times_appended[resting_place[cur_loc]] += 1
                        path_list[i] = seq_path[:j+1]
                        # Update resting places
                        temp = resting_place[seq_path[-1]]
                        resting_place[seq_path[-1]] = resting_place[cur_loc]
                        resting_place[cur_loc] = temp
                    have_seen[cur_loc] = True
                #parallel_path_list[i].append(cur_loc)
        j += 1

    #util.int_list_print(seq_path_list)
    #print()
    #util.int_list_print(path_list)
    return path_list, times_appended

# Perform the sequentual to parallel transformation
def sequential_to_parallel_rev(seq_path_list):

    path_list = copy.deepcopy(seq_path_list)
    path_list.reverse()

    # For each node, which particle ended up here?
    resting_place = {} # location # : particle #
    for i in range(len(path_list)):
        resting_place[path_list[i][-1]] = i
    #util.list_print(path_list)
    #print(resting_place)

    # Remember whether we have seen this node yet
    have_seen = [False for i in range(len(path_list))]

    times_appended = [0 for i in range(len(path_list))]

    j = 0
    while not all(have_seen):
        for i, seq_path in enumerate(path_list):
            #print("j = {}, i = {}".format(j, i))
            if j < len(seq_path):
                cur_loc = seq_path[j]
                if not have_seen[cur_loc]:
                    # Perform the cut-and-paste op if necessary
                    if j + 1 < len(seq_path):
                        path_list[resting_place[cur_loc]] += seq_path[j+1:]
                        times_appended[resting_place[cur_loc]] += 1
                        path_list[i] = seq_path[:j+1]
                        # Update resting places
                        temp = resting_place[seq_path[-1]]
                        resting_place[seq_path[-1]] = resting_place[cur_loc]
                        resting_place[cur_loc] = temp
                    have_seen[cur_loc] = True
                #parallel_path_list[i].append(cur_loc)
        j += 1

    #util.int_list_print(seq_path_list)
    #print()
    #util.int_list_print(path_list)
    #path_list.reverse()
    return path_list, times_appended
    
# Perform the sequentual to parallel transformation
def sequential_to_parallel_count_max(seq_path_list):
    max_changes = 0

    path_list = copy.deepcopy(seq_path_list)
    longest = len(max(path_list, key=len))

    # For each node, which particle ended up here?
    resting_place = {} # location # : particle #
    for i in range(len(path_list)):
        resting_place[path_list[i][-1]] = i
    #util.list_print(path_list)
    #print(resting_place)

    # Remember whether we have seen this node yet
    have_seen = [False for i in range(len(path_list))]

    j = 0
    while not all(have_seen):
        for i, seq_path in enumerate(path_list):
            #print("j = {}, i = {}".format(j, i))
            if j < len(seq_path):
                cur_loc = seq_path[j]
                if not have_seen[cur_loc]:
                    # Perform the cut-and-paste op if necessary
                    if j + 1 < len(seq_path):
                        path_list[resting_place[cur_loc]] += seq_path[j+1:]
                        if len(path_list[resting_place[cur_loc]]) > longest:
                            longest = len(path_list[resting_place[cur_loc]]) 
                            max_changes += 1

                        path_list[i] = seq_path[:j+1]
                        # Update resting places
                        temp = resting_place[seq_path[-1]]
                        resting_place[seq_path[-1]] = resting_place[cur_loc]
                        resting_place[cur_loc] = temp
                    have_seen[cur_loc] = True
                #parallel_path_list[i].append(cur_loc)
        j += 1

    #util.int_list_print(seq_path_list)
    #print()
    #util.int_list_print(path_list)
    return path_list, (max_changes)

# Perform the parallel to sequentual transformation
def parallel_to_sequential(par_path_list):

    path_list = copy.deepcopy(par_path_list)

    # For each node, which particle ended up here?
    resting_place = {} # location # : particle #
    for i, path in enumerate(path_list):
        resting_place[path[-1]] = i

    # Remember whether we have seen this node yet
    have_seen = [False for i in range(len(path_list))]

    times_appended = [0 for i in range(len(path_list))]

    #print("original")
    #util.list_print(par_path_list)
    #print()

    for part,path in enumerate(path_list):
        #print("step: {}".format(part))
        #util.list_print(path_list)
        for i,loc in enumerate(path):
            if not have_seen[loc]:
                have_seen[loc] = True
                if i < len(path) - 1:
                    cut = path[i+1:]
                    path_list[resting_place[loc]] += cut
                    times_appended[resting_place[loc]] += 1
                    path_list[part] = path[:i+1]

                    # Reset resting places
                    temp = resting_place[path[-1]]
                    resting_place[path[-1]] = resting_place[loc]
                    resting_place[loc] = temp
                    break # this is crucial!

    #print("final")
    #util.list_print(path_list)
    #print()
    #print()
    return path_list, times_appended
    
# Perform the parallel to sequentual transformation
def parallel_to_sequential_rev(par_path_list):

    path_list = copy.deepcopy(par_path_list)
    path_list.reverse()

    # For each node, which particle ended up here?
    resting_place = {} # location # : particle #
    for i, path in enumerate(path_list):
        resting_place[path[-1]] = i

    # Remember whether we have seen this node yet
    have_seen = [False for i in range(len(path_list))]

    times_appended = [0 for i in range(len(path_list))]

    #print("original")
    #util.list_print(par_path_list)
    #print()

    for part,path in enumerate(path_list):
        #print("step: {}".format(part))
        #util.list_print(path_list)
        for i,loc in enumerate(path):
            if not have_seen[loc]:
                have_seen[loc] = True
                if i < len(path) - 1:
                    cut = path[i+1:]
                    path_list[resting_place[loc]] += cut
                    times_appended[resting_place[loc]] += 1
                    path_list[part] = path[:i+1]

                    # Reset resting places
                    temp = resting_place[path[-1]]
                    resting_place[path[-1]] = resting_place[loc]
                    resting_place[loc] = temp
                    break # this is crucial!

    #print("final")
    #util.list_print(path_list)
    #print()
    #print()
    return path_list, times_appended

# Save simulated path length data for a large number of path graphs of size n
def generate_paths(n):
    print("working on size {}".format(n))
    g = make_segment(n)

    path_lengths = []
    for i in range(10000):
        if i % 1000 == 0: print(i)
        # Fill all but the last vertex
        for j in range(g.num_vertices() - 1):
            g.vp["full"][j] = True
        g.vp["full"][g.num_vertices()-1] = False
        path_lengths.append(run_single_path(g))
    return path_lengths

# Save path lengths up to size n
def generate_paths_up_to(n):
    for i in range(2, n):
        path_lengths = generate_paths(i)
        util.save_data(path_lengths, "times/length{}times_10000.ser".format(i))

# Simulate a walk on a larger path of length n**2
def run_sequential_big(n, filename):
    g = make_segment(n * n)

    paths = []
    lengths = []
    for i in range(g.get_vertices()):
        # take n * n random walks, as usual
        path, length = run_sequential_big(g, 0, True, filename)
        paths.append(path)
        lengths.append(length)

    return path, lengths
"""



#if __name__ == "__main__":
    # Testing area

    #generate_paths_up_to(30)

    #from matplotlib import pyplot as plt
    #bins = np.linspace(0, 25, 25)
    #plt.hist(paths, bins)
    #plt.show()

    #my_list = [[0],[0, 1],[0, 1, 2],[0, 1, 2, 1, 0, 4],[0, 1, 0, 4, 3]]
    #my_list = [[0], [0, 4], [0, 5], [0, 7], [0, 7, 2], [0, 6], [0, 4, 2, 4, 3], [0, 2, 7, 2, 6, 3, 1]]
    #my_list = [[0], [0, 8], [0, 8, 5], [0, 6], [0, 8, 0, 6, 4, 5, 8, 5, 1, 5, 4, 8, 0, 7, 1, 2], [0, 3], [0, 6, 3, 8, 7], [0, 4], [0, 1], [0, 9]]
    #my_list = [[0], [0, 1], [0, 6], [0, 9], [0, 8], [0, 9, 0, 6, 7, 0, 1, 8, 6, 0, 7, 3], [0, 5], [0, 1, 8, 5, 7, 9, 4, 0, 5, 1, 7, 8, 9, 3, 9, 0, 3, 8, 2], [0, 4], [0, 1, 6, 7]]

    #par_list,_ = sequential_to_parallel(my_list)
    #par_list2, _ = sequential_to_parallel2(my_list)
    #seq_list,_ = parallel_to_sequential(my_list)
    #seq_list2,_ = parallel_to_sequential2(my_list)

    #util.list_print(my_list)
    #print()
    #util.list_print(seq_list)
    #print()
    #util.list_print(seq_list2)
    #util.list_print(par_list)
    #print()
    #util.list_print(par_list2)
    #print("original parallel")
    #print(type(my_list))
    #util.list_print(my_list)
    #print()
    #print("new sequential")
    #print(type(seq_list))
    #util.list_print(seq_list)
