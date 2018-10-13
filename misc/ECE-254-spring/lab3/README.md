# Lab 3 - Producer Consumer Problem
By: Michael Rokas & Luke Seewald

## Description
The goal of this lab is to create two solutions to the producer consumer problem in a linux environment, one using processes and the other using threads. The results are documented and compared in the [report](lab3_rpt.pdf)
and conclusions about when to use which are made.

## How to build and test
To build, simply run the ```make``` command to compile the project in the threads or processes directory.

To run the program run the following command: ```./produce.out <N> <B> <P> <C>```<br>
N = Number of items to generate <br>
B = Buffer size <br>
P = Number of producers <br>
C = Number of consumers <br>

To run a large set of tests run the following command to run a testing script designed by the lab instructors: ```python collect_data.py```
