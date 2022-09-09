
advance functions
=================

Suppose that you have:

.. include:: ../../../tutorials/ode_advancer.cc
  :code: cpp
  :start-line: 52
  :end-line: 95


advance_n_times
---------------

.. include:: ../../../tutorials/ode_advancer.cc
  :code: cpp
  :start-line: 98
  :end-line: 114

.. include:: ../../../tutorials/ode_advancer.cc
  :code: cpp
  :start-line: 117
  :end-line: 135

.. include:: ../../../tutorials/ode_advancer.cc
  :code: cpp
  :start-line: 138
  :end-line: 165


advance_n_times_and_observe
---------------------------

.. include:: ../../../tutorials/ode_advancer.cc
  :code: cpp
  :start-line: 182
  :end-line: 197


Similarly works for the other cases accepting the time step setter, etc.

advance_to_target_time
----------------------

This always needs a functor to set the time step.

.. include:: ../../../tutorials/ode_advancer.cc
  :code: cpp
  :start-line: 200
  :end-line: 226



advance_to_target_time_and_observe
----------------------------------

This is like above, but you also need to pass an obsever.
