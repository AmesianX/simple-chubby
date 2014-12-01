##

TARGET = simple-chubby

TEXFILES = $(wildcard *.tex)
PDF_FIGURES = $(wildcard figs/*.pdf)
PNG_FIGURES = $(wildcard figs/*.png)
FIGURES= $(PDF_FIGURES) $(PNG_FIGURES)
TEXFILE = $(TARGET).tex
PDF = $(TARGET).pdf

all: $(PDF)

%.pdf: %.tex %.bib %.blg %.toc $(TEXFILES) $(FIGURES)
	pdflatex $*.tex
	bibtex $*
	pdflatex $*.tex
	pdflatex $*.tex

%.blg: %.bib
	pdflatex $*.tex
	bibtex $*
	pdflatex $*.tex

%.toc: %.tex
	pdflatex $*.tex

clean:
	/bin/rm -f $(PDF) *.dvi *.aux *.ps *~ *.log *.out *.lot *.lof *.toc *.blg *.bbl url.sty

view:
	pdflatex $(TARGET).tex
	okular $(PDF)

osx:
	pdflatex $(TARGET).tex
	open $(PDF)

evince:
	pdflatex $(TARGET).tex
	evince $(PDF) &

windows:
	pdflatex $(TARGET).tex
	explorer.exe $(PDF) &

check:
	pdflatex $(TARGET).tex | grep -i -e "undefined" -e "multiply"
