CC=clang++
CFLAGS=-Wall -std=c++11
DEPENDENCIES=-lpthread -lboost_log -DBOOST_LOG_DYN_LINK 
TEST_DEPENDENCIES=	$(DEPENDENCIES) -lboost_unit_test_framework \
					-DBOOST_TEST_DYN_LINK
LDFLAGS=-v
TARGET=NeuralNetwork
MAIN=Main.c++
SOURCES=	Neuron.c++ NeuralNetwork.c++ \
			NeuralNetworkTrainer.c++
TEST_SOURCES=	$(SOURCES) MainTest.c++ NeuronTest.c++ NeuralNetworkTest.c++ \
				NeuralNetworkTrainerTest.c++
OBJECTS=$(SOURCES:.c++=.o)

all:
	$(CC) $(SOURCES) $(MAIN) $(CFLAGS) $(DEPENDENCIES) -o $(TARGET)

test:
	$(CC) $(TEST_SOURCES) $(CFLAGS) $(TEST_DEPENDENCIES) -o $(TARGET)Test
# ./NeuralNetworkTest --log_level=message

debug:
	$(CC) $(SOURCES) $(CFLAGS) -g $(DEPENDENCIES) -o $(TARGET)Debug

debug_test:
	$(CC) $(TEST_SOURCES) $(CFLAGS) -g $(TEST_DEPENDENCIES) -o \
	$(TARGET)DebugTest

clean:
	rm -rf $(TARGET) $(TARGET)Test $(TARGET)Debug $(TARGET)DebugTest
