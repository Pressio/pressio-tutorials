Background
==========

Reduced-order models (ROMs) can be used to simulate a variety of physical phenomena
modeled by partial differential equations (PDEs). In this tutorial, we will focus
on the 1-D Burgers' equation as a representative example.

1-D Burgers' Equation
----------------------

The 1-D Burgers' equation is a fundamental partial differential equation
that models various physical phenomena, including fluid dynamics and traffic flow.
It is given by:

.. math::

   \frac{\partial u}{\partial t} + u \frac{\partial u}{\partial x} = \nu \frac{\partial^2 u}{\partial x^2}

where :math:`u` is the velocity field, :math:`t` is time, :math:`x` is the spatial coordinate,
and :math:`\nu` is the viscosity coefficient.

While a more comprehensive introduction to the Burgers' equation is beyond the scope of this tutorial,
numerical methods for solving it typically involve discretizing the spatial domain
using finite difference or finite volume methods, and then integrating the resulting
system of ordinary differential equations (ODEs) in time using methods such as
Forward Euler, Runge-Kutta, or implicit schemes.

In this tutorial, we will focus on setting up a reduced-order model (ROM)
for the 1-D Burgers' equation using the Pressio library.
