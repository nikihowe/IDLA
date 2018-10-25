(c) Nikolaus Howe 2018

This is the starting point for those wanting to use the C++ IDLA simulation software.
If you have any questions, please write to nikihowe AT gmail DOT com and
I'll see what I can do to help.

Table of Contents:
(0) Overview of the problem.
(1) Example use of the program.
(2) Description of files and what they contain.

*******************************
* (0) Overview of the problem *
*******************************

The IDLA (Internal Diffusion Limited Aggregation) process
is a random algorithm one can run on any connected mathematical graph
(from now on, we will only speak of connected graphs). Given a graph of n nodes,
we give the graph an additional property: whether or not each node is 'occupied'.
We also designate one of the nodes of the graph as the 'starting node'.

IDLA is performed by repeatedly performing random walks on the graph.
To begin one such random walk, take a 'particle' and place it on the starting node.
If the node is not occupied, the particle will be 'absorbed' by the node,
and stay there (the node becomes 'occupied').
If the starting node was already occupied, then the particle will move to another
node, chosen uniformly at random from the neighbours of the current node.
If this new node is not occupied, the particle will settle here,
otherwise moving to a random neighbour of this new node
(which could well be the original node again). The particles continues
moving through the graph in such a fashion until it finds
an unoccupied node, at which point it settles.

There are two main versions of the IDLA process: sequential and parallel.
Both processes begin with all nodes unoccupied.
In the sequential version,
a particle is allowed to walk from the start node until it settles, following
the process described above. Once it finds a place to settle (for the first
particle, this will be as soon as it is placed at the starting node), we place
another particle at the starting node, and watch it perform its random walk
until settling. This procedure is repeated until all the nodes are occupied.
In the parallel version,
n particles (where n = number of nodes in the graph) are all placed at the
starting node at once. Additionally, each particle is given a unique index
from 1 to n (0 to n-1 in our code). At each step in time, all particles
settle if they can, then if they haven't settled yet, perform one step
of the random walk described above. If two or more particles try to settle in
the same location, the one with the smallest index is allowed to settle,
while the others are not. This process is continued until all particles
have settled.

This is the basic process. One can study many properties of the process,
the most common of which being the length of the longest walk
(note that this seems to be, on average, slightly longer in the parallel case).
Another interesting question
is, given a recording of all the paths taken by all particles in 
a sequential IDLA run, how can one transform these paths so as 
to instead represent a parallel IDLA run? What about vice versa?
We know of at least one transformation which is a bijection between the
two processes, but are there more? Also of interest is how we can
modify the IDLA process to make it more efficient, and also how it
performs on different graph topologies.

This code in this directory is intented to assist with the study
of such questions.

**********************************
* (1) Example use of the program *
**********************************

Based on the description of IDLA in (0), let's run a simple simulation
of Sequential IDLA on a path graph (just nodes in a line) and print
the runs of each particle to the standard output.

// This is code, which would go in the main function of main.cpp
int numberOfNodes = 10;
std::vector< std::vector<int> > g = makePath(numberOfNodes);
std::vector< std::vector<int> > sequentialRun = runSimpleGraph(g);
print2dVector(sequentialRun);

Brief explanation:

We choose to have a path graph of 10 nodes (in general, we'll
usually want to simulate graphs of 1000 nodes or more, but 10
nodes is easier for seeing what's going on). We store the graph
as an adjacency list. Each node in the graph has an index
from 0 to n-1 (in this case, 0 to 9), hence the use of ints.
We use the std::vector datastructure as our list of lists.

There are constructors for many of the common graph topologies;
descriptions can be found in graphConstructors.hpp. In this
example, we used the makePath(int size) constructor, which makes
a path graph with "size" nodes.

The output of runSimpleGraph is also a vector of vectors: each one
represents the walk taken from node to node by each particle
of the walk. The location in the outer vector represents which
particle's walk we're looking at, and the location in the 
inner vector is which timestep we're at. Note that the first
two walks will always settle in 0 and in 1 steps respectively,
due to the nature of the IDLA process and the fact that our
graphs don't have self-loops (though there is nothing preventing
you from adding them in if you write code to do so!).

Various ways of running standard IDLA or similar processes on a given graph
can be found in standardIDLA.hpp and modifiedIDLA.hpp, respectively.

We can print this vector of vectors with the print2dVector.
Other printing functions, as well as other utilities, can be
found in util.hpp.

Another standard thing we might want to do with our code
is transform from one kind of process (sequential or parallel)
to the other. A slight modification of the above code yields the following.

// Same code as before, but with a transformation too
int numberOfNodes = 10;
std::vector< std::vector<int> > g = makePath(numberOfNodes);
std::vector< std::vector<int> > sequentialRun = runSimpleGraph(g);
std::vector< std::vector<int> > parallelRun = sequentialToParallel(sequentialRun);
print2dVector(sequentialRun);
print2dVector(parallelRun);

The standard StP transformation
(https://arxiv.org/abs/1808.09219, page 12)
is implemented as sequentialToParallel. The PtS
transformation (same paper) is implemented as
parallelToSequential. These and other
transformations can be found in
standardTransformations.hpp and
otherTransformations.hpp, respectively.

Numerous additional examples of things one can do with this
software are described in the exampleSimulations.hpp file.

***************************************************
* (2) Description of files and what they contain. *
***************************************************

- exampleSimulations.hpp and exampleSimulations.cpp -

A number of examples, some more interesting
than others, on possible ways to use the code.

- fastestPossible.cpp - 

A somewhat optimized simple sequential IDLA
process on the path. Written to be compared with
similar code written in Julia and Python, which
can be found in the other directories
(see top-level readme).

- graphConstructors.hpp and graphConstructors.cpp - 

A number of constructors for some common graphs.

- leadingPathCoefficient.cpp -

Code written to calculate, via simulation and via
an expected value calculation, the leading coefficient
for the dispersion time of sequential IDLA
on the path. It turned out it's hard to get
many sig figs on that constant.

- main.cpp - 

The place where you get to put code you want to run.

- makefile - 

A very simple makefile. Run "make" to compile all the code
into an executable called "sim", which can be run
by executing "./sim"

- modifiedIDLA.hpp and modifiedIDLA.cpp - 

Contains many modified versions of the IDLA process,
and related processes. These are
the rotor-router process, IDLA with one-step
lookahead, IDLA with momentum, and 
IDLA with a number of start points
distributed evenly throughout the graph.

- old.cpp - 

Contains some old code. Not much useful here,
but kept just in case (that's what I'm
telling myself :p).

- optimizedIDLA.hpp and optimizedIDLA.cpp -

Contains functions for running IDLA on the path and
on the cycle, optimized to not need an adjacency list.
They are mostly not useful, though they should
perform marginally faster than using functions
from standardIDLA. One important exception is
runFastSequentialPathLengths,
which uses bootstrapping to simulate walks on 
a path significantly faster than would be otherwise
possible, and should be used when one wants to
simulate walks on a path of over a few thousand nodes
(note however that it only returns the lengths
of the walks, not the walks themselves).

- otherTransformations.hpp and otherTransformations.cpp -

A number of different transformations, both from 
sequential to parallel and from parallel to sequential.
None of these are inverses of each other,
and at time of writing StP1 and PtS1 are the only
such known pair of transformations. It is unknown
whether all of these transformations
are bijections, but sequentialToParallel2
and parallelToSequential2 are
(though they are not inverses of one another).

- random.hpp and random.cpp - 

Contains functions to free up random nodes in a graph
(make the 'unoccupied') and then track how long
it takes for a particle to find one of them.

- readme.txt - 

This file :)

- rngTest.cpp - 

A sanity check for the RNGenerator.

- sim - 

The name of the executable that is created when one runs "make" in this directory

- standardIDLA.hpp and standardIDLA.cpp - 

The 'standard' sequential and parallel IDLA simulation functions,
along with helper functions for them.

- standardTransformations.hpp and standardTransformations.cpp - 

The 'standard' transformations StP1 and PtS1,
and some slight modifications to StP1.

- util.hpp and util.cpp - 

Contains a number of utility functions,
a random number generator class (BitGenerator),
and a struct (particleAndStep) used for keeping track
of 'locations' in the list of walks
that is returned by the IDLA process.

- visualization.hpp and visualization.cpp - 

Contains functions for recording out outputting
the levels of (different kinds of) clustering
at different points of the IDLA process
on a graph.
