.PHONY: all

all: paper.pdf

paper.pdf: paper.bbl
	pdflatex paper
	pdflatex paper

paper.bbl: paper.aux
	bibtex paper || rm paper.bbl paper.blg

paper.aux: paper.tex
	pdflatex paper || rm paper.aux

clean:
	$(RM) paper.aux paper.bbl paper.blg paper.log paper.out paper.toc paper.pdf
