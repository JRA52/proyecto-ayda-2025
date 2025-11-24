DESIGNAR = /home/ose1234/Ayda/DeSiGNAR-2.0.0

CXX = clang++ -std=c++14


WARN = -Wall -Wextra -Wno-sign-compare -Wno-write-strings -Wno-parentheses -Wno-invalid-source-encoding -Wno-cast-align

FLAGS = -DDEBUG -D_GLIBCXX__PTHREADS -g -O0 $(WARN)

INCLUDE = -I. -I $(DESIGNAR)/include

LIBS = -L $(DESIGNAR)/lib -lDesignar


OBJS = ImageGraph.o Segmenter.o Classifier.o main.o

TARGET = CellCounter

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(FLAGS) $(INCLUDE) -o $@ $(OBJS) $(LIBS)

ImageGraph.o: ImageGraph.hpp Definitions.hpp ImageGraph.cpp
	$(CXX) -c $(FLAGS) $(INCLUDE) ImageGraph.cpp

Segmenter.o: Segmenter.hpp Definitions.hpp Segmenter.cpp
	$(CXX) -c $(FLAGS) $(INCLUDE) Segmenter.cpp

Classifier.o: Classifier.hpp Definitions.hpp Classifier.cpp
	$(CXX) -c $(FLAGS) $(INCLUDE) Classifier.cpp

main.o: main.cpp ImageGraph.hpp Segmenter.hpp Classifier.hpp Definitions.hpp
	$(CXX) -c $(FLAGS) $(INCLUDE) main.cpp

.PHONY: clean
clean:
	$(RM) *~ $(TARGET) *.o