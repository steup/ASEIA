PROFILING        ?= 0
DEBUG            ?= 0
EMBEDDED         ?= 0
LTO              ?= 1

INCLUDES         +=
LDPATHS          +=
SYMBOLS          +=
CXXFLAGS         += -Wall -Wno-narrowing
LDFLAGS          +=
LIBS             +=
PACKAGES         +=

include make/rules.mk
