
# Building and Running pressio-tutorials
This is a step-by-step page explaining how to build the tutorials from scratch on Unix.

### Disclaimer:
The guide below does **not** assume you are a Unix/CS/coding ninja, rather the opposite. It is written with the goal to provide a complete and detailed guide without leaving anything hidden. As such, if you are a Unix/CS/coding ninja, some steps will be obvious to you, so please do not get offended!

## Prerequisites
In order for the steps below to be successful, you need:

* C/C++ compilers: either Clang or GNU <span style="color:red">min versions</span>.
The current version of the tutorials does NOT need MPI. But if you have MPI C/C++ compiler wrappers, you can use those to build.

* CMake with minimun version 2.8.12 <span style="color:red">(check this)</span>.

Note: we purposefully avoid (for the time being) using distributed data structures in these tutorials because we want a simpler framework to explain some concepts, and because the code shown in the tutorials can be used for MPI-based distributed data structures with almost no change.


## 1. Prep
<!--- For the sake of clarity, let us assume your name is *John Doe*, and your username is `johndoe`. --->

(1a) Create (or choose) a directory where you want to clone all repos needed, e.g.:
```bash
mkdir $HOME/pressio_repos
```

(2a) Create an environmen variable to refer to this directory:
```bash
export PRESSIO_REPOS=$HOME/pressio_repos
```
this will make things easier and cleaner below.


## 2. Cloning
To build the tutorials, you need to clone the following repos:
```bash
cd ${PRESSIO_REPOS}
git clone https://github.com/Pressio/pressio-builder.git
git clone https://github.com/Pressio/pressio.git
git clone https://github.com/Pressio/pressio-tutorials.git
```
