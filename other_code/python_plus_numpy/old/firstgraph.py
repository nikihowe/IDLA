from graph_tool.all import *
import util
from graph_func import *

# g is the graph
#g, vertex_list, is_taken = make_cycle(700)
g = make_clique(6)

# We start with n particles and have them travel sequentially through the graph,
# settling at the first open space they see

# Store the particle paths as a list of lists, where the list at place i
# is the path followed by particle i
start_vertex = 0

# I think this is important because otherwise would return np array each time
neighbour_cache = get_neighbour_cache(g, g.get_vertices())

list_of_particle_paths = run_sequential_particles(g, start_vertex, neighbour_cache)

#util.list_print(list_of_particle_paths)
#util.int_list_print(list_of_particle_paths)
par_list = sequential_to_parallel(list_of_particle_paths)
#util.int_list_print(par_list)
