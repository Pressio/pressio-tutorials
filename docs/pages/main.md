
# Pressio C++ Tutorials


@m_class{m-frame m-default}

@parblock
This website contains the tutorials
covering the [pressio C++ library](https://pressio.github.io/pressio/html/index.html).

Disclaimer: this is still work-in-progress, some coverage might be missing.
We plan to improve this over time.
@endparblock


## Building

@m_class{m-note m-warning}

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

@m_class{m-note m-info}

@parblock
Individual executables for all tutorials are built inside the build subdirectory.
@endparblock


## License and Citation
The BSD-3 license is available [here](https://pressio.github.io/various/license/).

We are working on publishing this. Our arXiv preprint at: https://arxiv.org/abs/2003.07798

## Questions?
Find us on Slack: https://pressioteam.slack.com or
open an issue on [github](https://github.com/Pressio/pressio-tutorials).
