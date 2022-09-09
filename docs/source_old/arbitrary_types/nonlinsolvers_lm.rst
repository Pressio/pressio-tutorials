Levenberg-Marquardt
===================

This tutorial demonstrates how to use
the normal-equations-based Levenberg-Marquardt solver from `pressio/solvers_nonlinear`
using custom data types.

**Headers**

.. include:: ../../../tutorials/nonlinsolvers_lm_custom_types.cc
  :code: cpp
  :start-line: 48
  :end-line: 52

**Problem class**

.. include:: ../../../tutorials/nonlinsolvers_lm_custom_types.cc
  :code: cpp
  :start-line: 53
  :end-line: 100

**Traits and operations required for custom types**

.. include:: ../../../tutorials/nonlinsolvers_lm_custom_types.cc
  :code: cpp
  :start-line: 101
  :end-line: 214

**Sovlver headers**

.. admonition:: Intentionally included *after* traits and operations for custom types are defined
  :class: important

  .. include:: ../../../tutorials/nonlinsolvers_lm_custom_types.cc
    :code: cpp
    :start-line: 215
    :end-line: 217

**Main**

.. include:: ../../../tutorials/nonlinsolvers_lm_custom_types.cc
  :code: cpp
  :start-line: 218
  :end-line: 256

**Full Code**

The full code is available in `nonlinsolvers_lm_custom_types.cc <https://github.com/Pressio/pressio-tutorials/blob/main/tutorials/nonlinsolvers_lm_custom_types.cc>`_
