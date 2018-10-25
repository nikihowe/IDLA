(c) Nikolaus Howe 2018

This code is the product of work over the summer for Thomas Sauerwald (and John Sylvester and Nicolas Rivera) during Summer 2018
at the Computer Laboratory at Cambridge.

The bulk of this work was focused on developing simulations for the sequential and parallel IDLA processes on different graph topologies.

Initial work was performed in Python3 using GraphTool and Numpy arrays. Due to speed requirements, development was
later shifted to C++. To see a simple comparison between running speeds in the three languages, these three files:

(C++)    ./cpp/fastestPossible.cpp
(Python) ./other_code/python_plus_numpy/fastestPossible.py
(Julia)  ./other_code/julia/fastestPossible.jl

run the exact same process, yet have different runtimes (with Julia and C++ comparable, but both much faster than Python).

Please see the readme in the cpp directory for instructions on how to use the software, the plots directory for
some example plots, and the other_code directory for some old code and some very specific code.
