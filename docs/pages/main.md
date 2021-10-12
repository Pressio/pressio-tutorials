
# Pressio C++ Tutorials


@m_class{m-frame m-default}

@parblock
Tutorials suite covering the [pressio C++ library](https://pressio.github.io/pressio/html/index.html).
@endparblock


## Build and Run

@m_class{m-note m-info}

@parblock
All you need CMake > 3.18.0 and a C++14 compliant compiler.
@endparblock

```bash
git clone --recursive git@github.com:Pressio/pressio-tutorials.git
export CXX=<path-to-your-CXX-compiler>
cd pressio-tutorials && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
```
Individual executables for all tutorials are built inside the build subdirectory.


## Disclaimer

This is still work-in-progress, some coverage might be missing.
We are actively working on adding more.
If you don't find something, you can temporarily look at [C++ tests subdirectory](https://github.com/Pressio/pressio/tree/main/tests),
which contains tests for everything. These tests are readable,
but are obviously not formatted in a tutorial fashion.

## License and Citation
The BSD-3 license is available [here](https://pressio.github.io/various/license/).

We are working on publishing this. Our arXiv preprint at: https://arxiv.org/abs/2003.07798

## Questions?
Find us on Slack: https://pressioteam.slack.com or
open an issue on [github](https://github.com/Pressio/pressio-tutorials).
