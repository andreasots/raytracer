.PHONY: all

all: clean paper.pdf

paper.pdf: paper.bbl
	pdflatex paper
	pdflatex paper

paper.bbl: add-enumeration paper.aux
	bibtex paper || rm paper.bbl paper.blg
	./add-enumeration paper.bbl
	mv paper.bbl.fixed paper.bbl

paper.aux: paper.tex
	pdflatex paper || rm paper.aux

clean:
	$(RM) paper.aux paper.bbl paper.blg paper.log paper.out paper.toc paper.pdf add-enumeration

add-enumeration:
	$(CXX) add-enumeration.cpp -o add-enumeration
