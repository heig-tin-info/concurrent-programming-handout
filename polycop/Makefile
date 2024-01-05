BUILDDIR = build

all: | $(BUILDDIR)
	latexmk -xelatex  -file-line-error -synctex=1 -outdir=$(BUILDDIR) main.tex
	mv $(BUILDDIR)/main.pdf $(BUILDDIR)/pco.pdf

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	latexmk -c
	$(RM) -rf $(BUILDDIR) *.bbl *.listing *.loa *.lol *.lof