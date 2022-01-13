Gauss-Newton with QR Factorization
==================================

This tutorial demonstrates how to use
the Gauss-Newton with QR factorization solver from `pressio/solvers_nonlinear`
using data types natively supported in pressio.
Here, we use Eigen data types, but a similar code
can be used for other natively supported types.

**Headers**

.. include:: ../../../tutorials/nonlinsolvers_gn_qr_known_types.cc
  :code: cpp
  :start-line: 49
  :end-line: 50

**Problem class**

.. include:: ../../../tutorials/nonlinsolvers_gn_qr_known_types.cc
  :code: cpp
  :start-line: 51
  :end-line: 98

**Main**

.. include:: ../../../tutorials/nonlinsolvers_gn_qr_known_types.cc
  :code: cpp
  :start-line: 99
  :end-line: 137

**Full Code**

The full code is available in `nonlinsolvers_gn_qr_known_types.cc <https://github.com/Pressio/pressio-tutorials/blob/main/tutorials/nonlinsolvers_gn_qr_known_types.cc>`_
