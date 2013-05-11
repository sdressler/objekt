include Makefile.in
include Makefile.target

TARGET = LLVMGraph.so

ROOT = $(realpath .)/
SRCDIR = src/

SUBDIRS = $(addprefix $(SRCDIR),$(MODULES))
 
#SUBDIRS = 	src/MicroGraph \#
			#src/KDV \
			#src/DataDirection
			#src/BlockProcessor \
			#src/LoopProcessor src/DataDirection
			

BUILDDIRS = build-src $(SUBDIRS:%=build-%)
CLEANDIRS = clean-src $(SUBDIRS:%=clean-%)

OBJ = $(wildcard build/*.o)

all: $(BUILDDIRS)

$(SUBDIRS): $(BUILDDIRS)
$(SRCDIR):  $(BUILDDIRS)

$(BUILDDIRS):
	@$(MAKE) ROOT=$(ROOT) -C $(@:build-%=%)

build-src: $(addprefix build-, $(SUBDIRS))

clean: $(CLEANDIRS)
	rm -f $(TARGET)
	rm -rf graph*
	rm -rf build

$(CLEANDIRS):
	$(MAKE) ROOT=$(ROOT) -C $(@:clean-%=%) clean

graph:
	dot -Tpdf -O graph.dot
	dot -Tpdf -O graphs/*.dot

run: $(BUILDDIRS) $(TARGET)
	mkdir -p graphs
	$(LLVM_ROOT)/bin/opt \
	-instnamer \
	-load ./$(TARGET) -kdv \
	$(BITCODE) > $(BITCODE:.bc=.tf.bc)
	$(MAKE) graph

.PHONY: subdirs $(SUBDIRS)
.PHONY: subdirs $(BUILDDIRS)
.PHONY: subdirs $(CLEANDIRS)
.PHONY: all clean
