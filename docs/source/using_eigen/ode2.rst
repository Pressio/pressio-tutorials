ode: tutorial 2
===============

This tutorial shows how to integrate the `original Lorenz system <https://en.wikipedia.org/wiki/Lorenz_system>`__ using explicit time integration.

.. literalinclude:: ../../../ode-using-eigen-types/tutorial2/main.cc
   :language: cpp
   :lines: 48-143


Process results
---------------

.. code-block:: bash

   # from within $BUILDDIR/ode-using-eigen-types/tutorial2
   python3 plot.py

.. image:: ../../../ode-using-eigen-types/tutorial2/plot.png
  :width: 50 %
  :align: center
  :alt: Lorenz
