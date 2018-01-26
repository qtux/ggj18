# disable implicit rules
.SUFFIXES:
# disable verbose output
$(VERBOSE).SILENT:

# set the all target to be the default (phony) target
default_target: all
.PHONY: default_target

# define the build folder
BUILDDIR = build

# forward make calls to the one located in the build folder
all:
%: $(BUILDDIR)/Makefile
	$(MAKE) -C $(BUILDDIR) -f Makefile $*

# execute cmake in $(BUILDDIR) if the Makefile does not exist
$(BUILDDIR)/Makefile:
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR) && cmake ..
