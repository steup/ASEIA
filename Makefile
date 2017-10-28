PROFILING        ?= 0
DEBUG            ?= 0
EMBEDDED         ?= 0
LTO              ?= 0

INCLUDES         +=
LDPATHS          +=
SYMBOLS          +=
CXXFLAGS         += -Wall -Wno-narrowing
LDFLAGS          +=
LIBS             +=
PACKAGES         +=

include make/rules.mk
