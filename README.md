A simple neural network implementation mainly for fun. With some sort of unit 
tests as well.

http://www.dgkelectronics.com

# Currently supports:
* Learning from png images
  *   Currently converts all images to grayscale
* Feed-forward networks
* Neuron activation functions:
  *   Linear
  *   Sigmoid
* Any number of neurons and layers (within implementation limits)
* Training using:
  *   Regular back-propagation
    *   With linear activation function
    *   With sigmoid activation function
  *   Incremental training
* Currently random order only
* Configuration via:
  *   Command-line

# Bugs (or features):
* Command-line options   *--network* (or *-n*) cannot be the last option before 
input files, otherwise the command-line option parsing will fail

# Dependencies:
* clang++ (tested with clang version 3.2-7ubuntu1)
* boost_log (tested with *libboost-log1.54-dev*)
* boost_program_options (tested with *libboost-program-options1.54-dev*)

## For tests you need:
* boost_unit_test_framework (tested with *libboost-test1.54-dev*)

# How you can try it out:
 1. Get dependencies
 2. Compile project with:
 ```
 make
 ```
 3. You can run an example with 2x2px images with the command: 
 ```
 ./NeuralNetwork --learning_rate 0.5 --network 4 --iterations 100000 ./test_images/2x2/* 
 ```

Kalle Hyvönen <kalle.hyvonen@dgkelectronics.com>
