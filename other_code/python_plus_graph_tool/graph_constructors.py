from graph_tool.all import *
import numpy as np
import util
import copy

# Make a cycle of size
def make_cycle(size):
    g = circular_graph(size)

    # Each vertex has property whether it is full or not
    g.vp["full"] = g.new_vertex_property("bool",
            vals=[False for i in range(g.num_vertices())])
    return g

# Make a clique of size
def make_clique(size):
    g = complete_graph(size)

    # Each vertex has property whether it is full or not
    g.vp["full"] = g.new_vertex_property("bool",
            vals=[False for i in range(g.num_vertices())])
    return g

# Make a m by n 2D grid
def make_grid(m, n):
    g = lattice([m, n])

    # Each vertex has property whether it is full or not
    g.vp["full"] = g.new_vertex_property("bool",
            vals=[False for i in range(g.num_vertices())])
    return g

# Make a m by n torus
def make_torus(m, n):
    g = lattice([m, n], periodic=True)

    # Each vertex has property whether it is full or not
    g.vp["full"] = g.new_vertex_property("bool",
            vals=[False for i in range(g.num_vertices())])
    return g

# Make a segment graph
def make_path(size):
    g = make_cycle(size)
    g.remove_edge(g.edge(0, size-1))
    return g

# Make a binary tree of depth d
def make_binary_tree(d):
    g = Graph(directed=False)
    v = g.add_vertex()
    add_nodes(g, v, d)

    # Each vertex has property whether it is full or not
    g.vp["full"] = g.new_vertex_property("bool",
            vals=[False for i in range(g.num_vertices())])
    return g

# Helper function for making binary tree
def add_nodes(g, v, d):
    if d > 0:
        d -= 1

        w1 = g.add_vertex()
        g.add_edge(v, w1)
        add_nodes(g, w1, d)

        w2 = g.add_vertex()
        g.add_edge(v, w2)
        add_nodes(g, w2, d)
