#!/bin/bash
#
shopt -s nullglob
for ref in *.bib
do
	bibtex $ref
done

for doc in *.tex
do
	pdflatex -interaction batchmode $doc
	pdflatex -interaction batchmode $doc
	pdflatex -interaction batchmode $doc
done
rm -f *.aux *.log *.maf *.mtc *.mtc0 *.out *.toc *.bbl *.blg
