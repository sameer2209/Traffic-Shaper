# Traffic-Shaper

Traffic-Shaper is a logging program for a system that will store the incoming requests in an unlimited size buffer Q1 which have deterministic resource requirement of R. If enough resources are available, the request will be moved to another unlimited size buffer Q2 and if server is not busy it will be executed. Each request will take ‘t’ time to execute on the server. The rate at which resource tokens are created is equal to ‘x’.

If the resource requirement is > L than the requests will be dropped. 

**Important feature: This is a logger with two different logging levels.** To see how to use the different logging levels see the usage section below.

## How to build

The program uses make to build the binaries

```bash
make trafficshaper
```

## Usage

The program needs the following command line options:
* -r: rate at which requests are generated
* -R: resource required by each request
* -L: fixed size of the token buffer
* -x: rate at which resources i.e tokens are generated
* -t: time required by each request to be serviced
* --log-level: logging level 

**NOTE: The following points are important to note:** 
* All the above flags are required.
* All the flags take positive integer values
* --log-level has two valid values, 1 represents normal log level and 2 represents verbose logging level

```python
# general syntax to run the program
./trafficshaper -r r -R R -L L -x x -t t --log-level l

# for example
./trafficshaper -r 2 -R 3 -L 5 -x 1 -t 2 --log-level 2
```