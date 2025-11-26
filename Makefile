DESIGNAR = /home/juanr/DOCUMENTOS/ayuda/DeSiGNAR-2.0.0

#  CXX = clang++ -std=c++14
CXX = clang++ -std=c++17

WARN = -Wall -Wextra -Wno-sign-compare -Wno-write-strings -Wno-parentheses -Wno-invalid-source-encoding -Wno-cast-align

FLAGS = -DDEBUG -D_GLIBCXX__PTHREADS -g -O0 $(WARN)

FRONTEND_LDFLAGS = -pthread -lstdc++fs

INCLUDE = -I. -I $(DESIGNAR)/include

# (DESIGNAR)
BACKEND_LIBS = -L $(DESIGNAR)/lib -lDesignar

# (SFML + X11)
FRONTEND_LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lX11

# Targets 
BACKEND_TARGET = backend
FRONTEND_TARGET = PQC_Analyzer

# Obj
BACKEND_OBJS = ImageGraph.o Segmenter.o Classifier.o Diagnostic.o backend.o 
FRONTEND_OBJS = main.o

# -------------------- PRINCIPAL TARGETS --------------------

all: $(BACKEND_TARGET) $(FRONTEND_TARGET)


$(FRONTEND_TARGET): $(FRONTEND_OBJS)
	# Incluimos FRONTEND_LDFLAGS antes de las librerías.
	$(CXX) $(FLAGS) $(INCLUDE) -o $@ $(FRONTEND_OBJS) $(FRONTEND_LDFLAGS) $(FRONTEND_LIBS)

$(BACKEND_TARGET): $(BACKEND_OBJS)
	$(CXX) $(FLAGS) $(INCLUDE) -o $@ $(BACKEND_OBJS) $(BACKEND_LIBS)


main.o: main.cpp
	$(CXX) -c $(FLAGS) $(INCLUDE) main.cpp

ImageGraph.o: ImageGraph.hpp Definitions.hpp ImageGraph.cpp
	$(CXX) -c $(FLAGS) $(INCLUDE) ImageGraph.cpp

Segmenter.o: Segmenter.hpp Definitions.hpp Segmenter.cpp
	$(CXX) -c $(FLAGS) $(INCLUDE) Segmenter.cpp

Classifier.o: Classifier.hpp Definitions.hpp Classifier.cpp
	$(CXX) -c $(FLAGS) $(INCLUDE) Classifier.cpp

Diagnostic.o: Diagnostic.hpp Definitions.hpp Diagnostic.cpp
	$(CXX) -c $(FLAGS) $(INCLUDE) Diagnostic.cpp

backend.o: backend.cpp ImageGraph.hpp Segmenter.hpp Classifier.hpp Definitions.hpp Diagnostic.hpp 
	$(CXX) -c $(FLAGS) $(INCLUDE) backend.cpp


.PHONY: clean
clean:
	$(RM) *~ $(BACKEND_TARGET) $(FRONTEND_TARGET) *.o