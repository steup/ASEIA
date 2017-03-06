PROFILING        := 0
DEBUG            := 1
EMBEDDED         := 0

INCLUDES         :=
LDPATHS          :=
SYMBOLS          :=
CXXFLAGS         := -O3 -Wall -flto=4 -fuse-linker-plugin
LDFLAGS          := -O1 -flto=4 -fuse-linker-plugin
AR               := gcc-ar
RANLIB           := gcc-ranlib
LIBS             :=
PACKAGES         :=

include make/rules.mk
