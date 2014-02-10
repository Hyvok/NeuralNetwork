CC=			clang++
OPTIM=		-O2
CFLAGS=		$(OPTIM) -Wall -std=c++11
DEP=		-lpthread -lboost_log -DBOOST_LOG_DYN_LINK -lpng \
			-lboost_program_options
TEST_DEP=	$(DEP) -lboost_unit_test_framework -DBOOST_TEST_DYN_LINK
LDFLAGS=	-v
TARGET=		NeuralNetwork
MAIN=		Main.c++
SRC=		Neuron.c++ NeuralNetwork.c++ NnTrainer.c++ \
			NnImageMap.c++ StatusBar.c++
TEST_SRC=	$(SRC) ./tests/MainTest.c++ ./tests/NeuronTest.c++ \
			./tests/NeuralNetworkTest.c++ ./tests/NnTrainerTest.c++ \
			./tests/NnImageMapTest.c++ ./tests/StatusBarTest.c++

all:
	$(CC) $(SRC) $(MAIN) $(CFLAGS) $(DEP) -o $(TARGET)

test:
	$(CC) $(TEST_SRC) $(CFLAGS) $(TEST_DEP) -o $(TARGET)Test
# ./NeuralNetworkTest --log_level=message

debug:
	$(CC) $(SRC) $(CFLAGS) -g $(DEP) -o $(TARGET)Debug

debug_test:
	$(CC) $(TEST_SRC) $(CFLAGS) -g $(TEST_DEP) -o \
	$(TARGET)DebugTest

clean:
	rm -rf $(TARGET) $(TARGET)Test $(TARGET)Debug $(TARGET)DebugTest
