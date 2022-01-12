IRLS
====

This tutorial demonstrates how to use
the normal-equations-based Iterative Reweighted Least Squares solver from `pressio/solvers_nonlinear`
using custom data types.

**Headers**

.. include:: ../../../tutorials/nonlinsolvers_irls_custom_types.cc
  :code: cpp
  :start-line: 48
  :end-line: 52

**Problem class**

.. include:: ../../../tutorials/nonlinsolvers_irls_custom_types.cc
  :code: cpp
  :start-line: 53
  :end-line: 101

**Traits and operations required for custom types**

.. include:: ../../../tutorials/nonlinsolvers_irls_custom_types.cc
  :code: cpp
  :start-line: 102
  :end-line: 336

**Sovlver headers**

.. admonition:: Intentionally included *after* traits and operations for custom types are defined
  :class: important

  .. include:: ../../../tutorials/nonlinsolvers_irls_custom_types.cc
    :code: cpp
    :start-line: 337
    :end-line: 339

**Main**

.. include:: ../../../tutorials/nonlinsolvers_irls_custom_types.cc
  :code: cpp
  :start-line: 340
  :end-line: 376

**Full Code**

The full code is available in `nonlinsolvers_irls_custom_types.cc <nonlinsolvers_irls_custom_types.cc>`_
