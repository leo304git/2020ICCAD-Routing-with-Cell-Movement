#
# This is for your executable
#
CFLAGS =-g -Wall -std=c++11
CFLAGS = -O3 -std=c++11
#
# This is the setting for TA
#
# CFLAGS = -g -Wall -DTA_KB_SETTING -std=c++11
# CFLAGS = -O3 -DTA_KB_SETTING -std=c++11
# CFLAGS = -O3 -m32 -DTA_KB_SETTING -std=c++11

###############
# PHONY rules #
###############
.PHONY : eda

eda : MAKEFLAGS = $(CFLAGS)

eda : .final


##############
# Executable #
#############
final: main.o parser.o MCtype.o Net.o CellInst.o Blockage.o Ggrid.o Pin.o Route.o Layer.o ExtraDemand.o
	g++ -o $@ $(MAKEFLAGS) main.o parser.o MCtype.o Net.o CellInst.o Blockage.o Ggrid.o Pin.o Route.o Layer.o ExtraDemand.o
#	strip $@



################
# Object files #
################
main.o: main.cpp parser.h
	g++ -g -c $(MAKEFLAGS) main.cpp

parser.o: parser.cpp parser.h MCtype.h  Net.h Ggrid.h  Layer.h ExtraDemand.h CellInst.h
	g++ -g -c $(MAKEFLAGS) parser.cpp

MCtype.o:MCtype.cpp MCtype.h Pin.h Blockage.h Layer.h
	g++ -g -c $(MAKEFLAGS) MCtype.cpp

Net.o:Net.cpp Net.h Layer.h Pin.h Route.h Ggrid.h
	g++ -g -c $(MAKEFLAGS) Net.cpp

CellInst.o:CellInst.h MCtype.h Pin.h Blockage.h Net.h
	g++ -g -c $(MAKEFLAGS) CellInst.cpp

Blockage.o:Blockage.h Layer.h
	g++ -g -c $(MAKEFLAGS) Blockage.cpp

Ggrid.o:Ggrid.cpp Ggrid.h
	g++ -g -c $(MAKEFLAGS) Ggrid.cpp

Pin.o:Pin.cpp Pin.h Layer.h
	g++ -g -c $(MAKEFLAGS) Pin.cpp

Route.o:Route.cpp Route.h  Pin.h Ggrid.h
	g++ -c $(MAKEFLAGS) Route.cpp

Layer.o:Layer.cpp Layer.h
	g++ -c $(MAKEFLAGS) Layer.cpp

ExtraDemand.o:ExtraDemand.cpp ExtraDemand.h Layer.h
	g++ -c $(MAKEFLAGS) ExtraDemand.cpp

#######################
# No dependency rules #
#######################

clean:
	rm -f *.o 

ctags:
	ctags *.cpp *.h
