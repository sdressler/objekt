include ../Makefile.in

TARGET = objekt

LIBS = $(addprefix -l, $(MODULES))
DEPS = $(addsuffix .a, $(addprefix ../lib/lib, $(MODULES)))

#LIBS = -lMicroGraph -lLoopProcessor #-lKDV -lDataDirection # -lBlockProcessor -lLoopProcessor -lDataDirection

BUILDDIR = $(ROOT)build/
SRCDIR = $(realpath .)/
LIBDIR = $(ROOT)lib/

TARGETLIB = $(ROOT)$(TARGET).so

SRC = $(wildcard *.cpp)
OBJ = $(addprefix $(BUILDDIR), $(SRC:%.cpp=%.o))

all: $(TARGETLIB) $(DEPS)

$(TARGETLIB): $(OBJ) $(DEPS)
	@echo "  LD   $(notdir $(OBJ)) $(LIBS) -> $(TARGETLIB)"
	@$(CXX) -L$(LIBDIR) -o $@ $(OBJ) -Wl,--whole-archive $(LIBS) -Wl,--no-whole-archive $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGETLIB)

$(OBJ): | $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

## RULE FOR BUILDING OBJS ##
$(BUILDDIR)%.o : $(SRCDIR)%.cpp
	@echo "  CXX  $(notdir $<) -> $(notdir $@)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@
