CXX   = g++
RM    = rm -r
TAR   = tar -cf
CP    = cp -r
MKDIR = mkdir -p

SRC_DIR = src
INC_DIR = includes
RES_DIR = assets
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
OBJETS  = $(patsubst %.cpp,%.cpp.o,$(notdir $(SOURCES)))

CXXFLAGS = --std=c++11
LIBS     = -lsfml-graphics -lsfml-window -lsfml-system

EXE     = projet
EXE_EXT = out

ARCHIVE     = binome38.tar
ARCHIVE_DIR = Botti-Kennel
PDF_DIR     = report

%.cpp.o: $(SRC_DIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.cpp.o: $(SRC_DIR)/**/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

main: CXXFLAGS += -O3
main: compilation

dev: CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -g
dev: CXXFLAGS += -Wold-style-cast -Wsign-conversion
dev: compilation

compilation: $(OBJETS)
	$(CXX) -o $(EXE).$(EXE_EXT) $^ $(LIBS)

all:
	main

pdf-make:
	cd report && \
	$(MAKE)

pdf-clean:
	cd report && \
	$(MAKE) clean

clean: pdf-clean
	-$(RM) $(OBJETS) "$(EXE).$(EXE_EXT)" "$(ARCHIVE)"

archive: pdf-make
	$(MKDIR) "$(ARCHIVE_DIR)"
	$(CP) "$(SRC_DIR)" "$(INC_DIR)" Makefile README.md "$(wildcard $(PDF_DIR)/*.pdf)" \
	      "$(RES_DIR)" "$(PDF_DIR)/imgs/DiagrammeUML.png" "$(ARCHIVE_DIR)"
	$(TAR) "$(ARCHIVE)" "$(ARCHIVE_DIR)"
	$(RM) "$(ARCHIVE_DIR)"
