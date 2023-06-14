ode: tutorial 3
===============

Integrate the `original Lorenz system <https://en.wikipedia.org/wiki/Lorenz_system>`__ using implicit time integration.

.. literalinclude:: ../../../ode-using-eigen-types/tutorial3/main.cc
   :language: cpp
   :lines: 48-

Process results
---------------

Move to ``<your-build-dir>/ode-using-eigen-types/tutorial3`` and run:

.. code-block:: bash

   ./ode_eigen_exe3
   python3 plot.py

.. image:: ../../../ode-using-eigen-types/tutorial3/plot.png
  :width: 50 %
  :align: center
  :alt: Lorenz
