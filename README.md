# Fizplex

Fizplex is an early-stage linear programming solver. It currently handles small to medium-sized LPs in standard form.

## Goal & roadmap

The goal is a full-fledged, modern MIP solver. The next big milestone will be solving all Netlib instances, which will most likely require:

* Read MPS-files in non-standard form.
* Update the inverse iteratively with fixed-intervall refactorization.
* Use a simple equilibrium scaling.
* Write the solution vector on user request (currently only the optimal value is returned).

## Building & usage:

```sh
$ git submodule init
$ git submodule update
$ mkdir build && cd build
$ cmake ..
$ make
$ ./fizplex my_problem.mps
```

Running tests (from project root since example files are read in the process):

```sh
$ ./build/tests
```
