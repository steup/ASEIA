SOURCES = $(wildcard *.graphml)
TARGETS = $(addsuffix .akg, $(basename ${SOURCES}))

DE_Y    = stripY.xslt
XSLT    = xsltproc

.PHONY: all clean

all: ${TARGETS}

%.akg: %.graphml ${DE_Y}
	@echo "Removing yED extensiona from $<"
	@xsltproc ${DE_Y} $< > $@

clean:
	rm -rf *.akg
