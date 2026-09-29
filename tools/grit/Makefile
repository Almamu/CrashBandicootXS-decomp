# Minimal build of the vendored grit (see README.md). Replaces upstream's
# autotools build; needs only a C++ compiler and libpng (both already in
# the devshell and in CI), no FreeImage.
CXX      ?= g++
CXXFLAGS ?= -O2 -w
DEFS     := -DPACKAGE_VERSION=\"0.10.0\"
INCS     := -Icldib -Ilibgrit -Iextlib -I. $(shell pkg-config --cflags libpng)
LIBS     := $(shell pkg-config --libs libpng)

SRCS := cldib/cldib_adjust.cpp cldib/cldib_conv.cpp cldib/cldib_core.cpp \
        cldib/cldib_tmap.cpp cldib/cldib_tools.cpp cldib/cldib_wu.cpp \
        libgrit/cprs.cpp libgrit/cprs_huff.cpp libgrit/cprs_lz.cpp \
        libgrit/cprs_rle.cpp libgrit/grit_core.cpp libgrit/grit_misc.cpp \
        libgrit/grit_prep.cpp libgrit/grit_shared.cpp libgrit/grit_xp.cpp \
        libgrit/logger.cpp libgrit/pathfun.cpp \
        srcgrit/cli.cpp srcgrit/grit_main.cpp extlib/fi.cpp
OBJS := $(SRCS:.cpp=.o)
HDRS := $(wildcard cldib/*.h libgrit/*.h srcgrit/*.h extlib/*.h) grit_version.h

.PHONY: all clean

all: grit
	@:

grit: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) $(DEFS) $(INCS) -c -o $@ $<

clean:
	$(RM) grit $(OBJS)
