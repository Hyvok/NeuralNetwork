CC=clang++
CFLAGS=-Wall -std=c++11 -lpthread -lboost_log -DBOOST_LOG_DYN_LINK
LDFLAGS=-v
TARGET=NeuralNetwork
SOURCES=Main.c++ Synapse.c++ Neuron.c++ NeuralNetwork.c++
OBJECTS=$(SOURCES:.c++=.o)

all:
	$(CC) $(SOURCES) $(CFLAGS) -o $(TARGET)

clean:
	rm -rf $(TARGET)
