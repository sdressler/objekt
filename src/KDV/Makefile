include ../../Makefile.in

TARGET = KDV

BUILDDIR = $(ROOT)build/$(TARGET)/
SRCDIR = $(realpath .)/
LIBDIR = $(ROOT)lib/

TARGETLIB = $(LIBDIR)lib$(TARGET).a

SRC = $(wildcard *.cpp) $(wildcard Handler/*.cpp)
OBJ = $(addprefix $(BUILDDIR), $(SRC:%.cpp=%.o))

all: $(TARGETLIB)

$(TARGETLIB): $(OBJ)
	@echo "  AR   $(notdir $(OBJ)) -> $(TARGETLIB)"
	@$(AR) cr $(TARGETLIB) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGETLIB)

$(OBJ): | $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)/Handler

## RULE FOR BUILDING OBJS ##
$(BUILDDIR)%.o : $(SRCDIR)%.cpp
	@echo "  CXX  $(notdir $<) -> $(notdir $@)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

#include ../Makefile.in
#
#TARGET = KernelVolume
#
#LIBS = -lArgParser -lTypeHandlers
#
#BUILDDIR = $(ROOT)build/
#SRCDIR = $(realpath .)/
#LIBDIR = $(ROOT)lib/
#
#TARGETLIB = $(ROOT)$(TARGET).so
#
#SRC = $(wildcard *.cpp)
#OBJ = $(addprefix $(BUILDDIR), $(SRC:%.cpp=%.o))
#
#all: $(TARGETLIB)
#
#$(TARGETLIB): $(OBJ) ../lib/libArgParser.a ../lib/libTypeHandlers.a
#	@echo "  LD   $(notdir $(OBJ)) $(LIBS) -> $(TARGETLIB)"
#	@$(CXX) -L$(LIBDIR) -o $@ $(OBJ) -Wl,--whole-archive $(LIBS) -Wl,--no-whole-archive $(LDFLAGS)
#
#clean:
#	rm -f $(OBJ) $(TARGETLIB)
#
#$(OBJ): | $(BUILDDIR)
#
#$(BUILDDIR):
#	mkdir -p $(BUILDDIR)
#
### RULE FOR BUILDING OBJS ##
#$(BUILDDIR)%.o : $(SRCDIR)%.cpp
#	@echo "  CXX  $(notdir $<) -> $(notdir $@)"
#	@$(CXX) $(CXXFLAGS) -c $< -o $@
