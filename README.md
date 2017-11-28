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
$ make
$ ./bin/fizplex my_problem.mps
```

Running tests:

```sh
$ make test
```
Note, that this requires [googletest].

[googletest]: https://github.com/google/googletest

