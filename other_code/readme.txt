(c) Nikolaus Howe 2018

This is the other code directory, which is less interesting than the C++ directory. None of this code
is actively used or supported, so you're probably looking for the C++ directory. However, if this
_is_ where you want to be, here's what's here.

*Mathematica*
This contains a notebook used to approximate the leading constant
of the dispersion time using standard IDLA on a path. I wasn't able
to get it to give me a final answer.

*python_plus_graph_tool*
This is where my work began. Many of the functions and data structures used in the cpp directory
were first experimented with here. We abandoned this way of doing things because it was
simply much too slow to run simulations on graphs of size > 1000 nodes.

*python_plus_numpy*
An attempt to make the code fast without leaving Python. It didn't make enough of a difference,
so we abandoned this too.

*julia*
A translation of the algorithm to run a simple random walk on a path. This was done to test
whether we could use julia for our purposes. The answer is yes, as it runs many times
faster than its Python equivalent. In the end though, we decided to go with C++ for 
maintainability purposes.
