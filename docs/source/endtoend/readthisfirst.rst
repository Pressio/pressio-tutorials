Start reading here
==================

This section offers a suite of end-to-end tutorials/demos covering
some of the key ROM methods implemented in pressio.

Why doing this?
---------------

Describing how to use pressio for ROMs cannot be simply
boiled down to presenting the API, because setting up, running
and processing a ROM involves several steps.
We believe the best way to understand these steps and also
what pressio offers in this context is via demos where the logical sequence
of steps is presented and explained.
To this end, we have developed a workflow-based framework where
one can *express* what to do, and the framework helps
automating (to some extent) the execution.

Key features
------------

- each demo is fully documented in a step by step manner (see `here for example <swe_galerkin_default.html>`__) to show how to run each step

- each demo has an associated yaml workflow file,
  organized into several section each with a specific scope

- all `driver scripts <https://github.com/Pressio/pressio-tutorials/tree/develop/end-to-end-roms>`__
  are written in Python and are designed to be easy to run

- one can easily extend the framework by adding new ROM methods
  in pressio or account for variations
  in how each step is run

- pressio-demoapps: for the end-to-end demos we rely on pressio-demoapps,
  which have built-in support for the sample mesh.
  While we adopt a specific problem (2d Shallow water equations), one can easily
  setup and run similar workflows for any other problem in pressio-demoapps.
  This would only require minimal work.


Outcomes
--------

- by reading these end-to-end demos you should be able to understand
  the various steps and how these steps map to pressio code

- you can easily explore the impact of changing the parameter spaces,
  or the basis size by just changing the workflow file and rerunning

- while the workflow driver scripts and code are tailored to
  pressio-demoapps running in shared memory mode, one can easily
  recycle these scripts/code and adapt them to another code.
  Because the steps involved would NOT change, but how you implement
  each step does change and can be application specific.
