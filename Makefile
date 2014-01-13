CC=clang++
CFLAGS=-Wall -std=c++11 
DEPENDENCIES=-lpthread -lboost_log -DBOOST_LOG_DYN_LINK
TEST_DEPENDENCIES=-lboost_unit_test_framework -lpthread -lboost_log -DBOOST_LOG_DYN_LINK -DBOOST_TEST_DYN_LINK
LDFLAGS=-v
TARGET=NeuralNetwork
SOURCES=Main.c++ Synapse.c++ Neuron.c++ NeuralNetwork.c++
TEST_SOURCES=SynapseTest.c++ Synapse.c++ Neuron.c++ NeuronTest.c++ NeuralNetworkTest.c++ NeuralNetwork.c++
OBJECTS=$(SOURCES:.c++=.o)

all:
	$(CC) $(SOURCES) $(CFLAGS) $(DEPENDENCIES) -o $(TARGET)

test:
	$(CC) $(TEST_SOURCES) $(CFLAGS) $(TEST_DEPENDENCIES) -o $(TARGET)Test

clean:
	rm -rf $(TARGET) $(TARGET)Test
