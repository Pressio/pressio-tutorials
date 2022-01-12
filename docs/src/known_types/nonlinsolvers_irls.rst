
IRLS
====

This tutorial demonstrates how to use
the normal-equations-based Iterative Reweighted Least Squares solver from `pressio/solvers_nonlinear`
using data types natively supported in pressio.
Here, we use Eigen data types, but a similar code
can be used for other natively supported types.

**Headers**

.. include:: ../../../tutorials/nonlinsolvers_irls_known_types.cc
  :code: cpp
  :start-line: 48
  :end-line: 51

**Problem class**

.. include:: ../../../tutorials/nonlinsolvers_irls_known_types.cc
  :code: cpp
  :start-line: 52
  :end-line: 99

**Main**

.. include:: ../../../tutorials/nonlinsolvers_irls_known_types.cc
  :code: cpp
  :start-line: 100
  :end-line: 137

**Full Code**

The full code is available in `nonlinsolvers_irls_known_types.cc <https://github.com/Pressio/pressio-tutorials/blob/main/tutorials/nonlinsolvers_irls_known_types.cc>`_
