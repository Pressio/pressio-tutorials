
# Building and Running pressio-tutorials
This step-by-step page explains how to build the tutorials from scratch on Unix.

*Disclaimer*: the guide below does **not** assume you are a Unix/CS/coding ninja, rather the opposite. It is written with the goal to provide a complete and detailed guide without taking any step for granted. As such, if you are a Unix/CS/coding ninja, some steps will be obvious to you, so please do not get offended!

## Prerequisites
In order for the steps below to be successful, you need:

* C and C++11 compilers: Clang/GNU/Intel.
The current version of the tutorials does NOT need MPI. But if you have MPI compiler wrappers, you can use those to build.

* wget: this is used to get the source code of Eigen.

* CMake >= 3.11.0.

* Bash >= 3.2.57.

**Note**: we purposefully avoid (for the time being) using distributed data structures in these tutorials because we want a simpler framework to explain some concepts. However, we remark that the code shown in the tutorials can be used for MPI-based distributed data structures with almost no change. This will be explained in more detail within some of the tutorials.


<!---------------------------------------------------->
## 1. Prep

(a) Create (or choose) a directory where you want to clone all repos needed and where to put all builds, e,g.:

```bash
mkdir $HOME/pressio_{repos,builds}
```

(b) To make things easier and cleaner below, create environment variables to refer to these directories:

```bash
export PRESSIO_REPOS=$HOME/pressio_repos
export PRESSIO_BUILDS=$HOME/pressio_builds
```

(c) Unless you already have them, set the following compilers environment variable:

```bash
export CC=<path-to-your-C-compiler>
export CXX=<path-to-your-CXX-compiler>
```
These are needed because `CC` and `CXX` are used to do all the builds. Fortran is not needed to build the tutorials.


<!---------------------------------------------------->
## 2. Cloning

To build the tutorials, you need to clone the following repos:

```bash
cd ${PRESSIO_REPOS}

git clone git@github.com:Pressio/pressio.git
cd pressio; git checkout v0.2.1; cd -

git clone git@github.com:Pressio/pressio-builder.git
cd pressio-builder; git checkout master; cd -

git clone git@github.com:Pressio/pressio-tutorials.git
```

<!---------------------------------------------------->
## 3. Install TPLs

We only need Eigen (for now), so you can simply run the command:

```bash
cd ${PRESSIO_REPOS}/pressio-builder
./main_tpls.sh -dryrun=no -tpls=eigen -target-dir=${PRESSIO_BUILDS}
```
This will install Eigen and since Eigen is header-only, nothing is built. 
The installation process is simply a clone of the Eigen headers.
To learn more about the script's command line args, type `./main_tpls.sh -h`.

<!---------------------------------------------------->
## 4. Install Pressio
From the same directory, i.e. `${PRESSIO_REPOS}/pressio-builder`, run the command:

```bash
./main_pressio.sh \
	-dryrun=no \
	-pressio-src=${PRESSIO_REPOS}/pressio \
	-target-dir=${PRESSIO_BUILDS} \
	-cmake-generator-name=default_for_tutorials
```
For this stage, `pressio` is also not built since it is header-only. 
To learn more about the script's command line args, type `./main_pressio.sh -h`.

<!---------------------------------------------------->
## 5. Building the tutorials
From the same directory, i.e. `${PRESSIO_REPOS}/pressio-builder`, run the command:

```bash
./main_tutorials.sh \
	-dryrun=no \
	-pressio-tutorials-src=${PRESSIO_REPOS}/pressio-tutorials \
	-target-dir=${PRESSIO_BUILDS} \
	-build-mode=Release \
	-eigen-path=${PRESSIO_BUILDS}/eigen/install \
	-pressio-path=${PRESSIO_BUILDS}/pressio/install
```
To learn more about the script's command line args, type `./main_tutorials.sh -h`.

<!---------------------------------------------------->
## 6. Understanding the tutorials
To understand the tutorials, look at the source codes in `${PRESSIO_REPOS}/pressio-tutorials/tutorials`.
To run them, from the build directory, i.e. `${PRESSIO_BUILDS}/pressio-tutorials/build`, run the executables as: 
```bash 
./executable_name
```
