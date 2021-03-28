CC = gcc
CFLAGS = -Wall -g -MD
RMFILES = expression_tree.png output.log output.tex output.aux output.pdf texput.log expression_tree.gv

all: diff

diff: dif.o
	$(CC) -o $@ $^
	./diff
	@ pdflatex output.tex
	@ dot -Tpng expression_tree.gv -o expression_tree.png 

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf diff *.d *.o $(RMFILES)

-include *.d