CC=clang++
CFLAGS=-Wall -std=c++11
DEPENDENCIES=-lpthread -lboost_log -DBOOST_LOG_DYN_LINK
TEST_DEPENDENCIES=-lboost_unit_test_framework -lpthread -lboost_log -DBOOST_LOG_DYN_LINK -DBOOST_TEST_DYN_LINK
LDFLAGS=-v
TARGET=NeuralNetwork
SOURCES=Main.c++ InputSynapse.c++ OutputSynapse.c++ Neuron.c++ NeuralNetwork.c++
TEST_SOURCES=MainTest.c++ InputSynapseTest.c++ InputSynapse.c++ OutputSynapseTest.c++ OutputSynapse.c++ Neuron.c++ NeuronTest.c++ NeuralNetworkTest.c++ NeuralNetwork.c++
OBJECTS=$(SOURCES:.c++=.o)

all:
	$(CC) $(SOURCES) $(CFLAGS) $(DEPENDENCIES) -o $(TARGET)

test:
	$(CC) $(TEST_SOURCES) $(CFLAGS) $(TEST_DEPENDENCIES) -o $(TARGET)Test
# ./NeuralNetworkTest --log_level=message

debug:
	$(CC) $(SOURCES) $(CFLAGS) -g $(DEPENDENCIES) -o $(TARGET)Debug

debug_test:
	$(CC) $(TEST_SOURCES) $(CFLAGS) -g $(TEST_DEPENDENCIES) -o $(TARGET)DebugTest

clean:
	rm -rf $(TARGET) $(TARGET)Test $(TARGET)Debug $(TARGET)DebugTest
