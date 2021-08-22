# Pressio C++ Tutorials

*Leading-edge reduced order models (ROMs) for dynamical systems in science and engineering.*

This website presents the [C++ tutorials](https://github.com/Pressio/pressio-tutorials),
which use the [pressio C++ core library](https://pressio.github.io/pressio/html/index.html).<br>
<!-- The full project website is [here](https://pressio.github.io/). -->

## Get Started

Requires CMake > 3.13.0 and a C++ compiler with C++11 support:
```bash
git clone --recursive git@github.com:Pressio/pressio-tutorials.git
export CXX=<path-to-your-CXX-compiler>
cd pressio-tutorials && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
```
The executables for all tutorials are built inside the build subdirectory.

## License and Citation
The BSD-3 license is available [here](https://pressio.github.io/various/license/).

We are working on publishing this. Our arXiv preprint at: https://arxiv.org/abs/2003.07798

## Questions?
Find us on Slack: https://pressioteam.slack.com or
open an issue on [github](https://github.com/Pressio/pressio-tutorials).
