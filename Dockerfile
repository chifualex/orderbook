# GCC support can be specified at major, minor, or micro version
# (e.g. 8, 8.2 or 8.2.0).
# See https://hub.docker.com/r/library/gcc/ for all supported GCC
# tags from Docker Hub.
# See https://docs.docker.com/samples/library/gcc/ for more on how to use this image
FROM ubuntu:22.04

RUN apt-get update -y
RUN apt-get upgrade -y

# reinstall certificates, otherwise git clone command might result in an error
RUN apt-get install --reinstall ca-certificates -y

# install developer dependencies
RUN apt-get install -y git build-essential cmake --no-install-recommends

# These commands copy your files into the specified directory in the image
# and set that as the working location
COPY . /orderbook
WORKDIR /orderbook

# compile with CMake
RUN bash -c "mkdir build"
RUN bash -c "cd build"
RUN bash -c "cmake .."
RUN bash -c "cmake --build ."

# This command runs your application, comment out this line to compile only
CMD ["../app/./OrderBookApp UDP"]

LABEL Name=alechifu Version=0.0.1
