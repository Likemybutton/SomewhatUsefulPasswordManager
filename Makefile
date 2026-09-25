##
# Simple password manager
#
# @file
# @version 0.1

CC=gcc
#hasher:=$(shell find "./" -name "hasher.c")
#dehasher:=$(shell find "./" -name "dehasher.c")
hasher=./hasher/hasher.c
dehasher=./dehasher/dehasher.c
csvHeaderPrinter:=./csvHeaderPrinter.c
hasherBinary=./Script/hasher
dehasherBinary=./Script/dehasher
csvHeaderPrinterBinary=./Script/csvHeaderPrinter

comp: $(hasher) $(dehasher)
	$(CC) $(hasher) -o $(hasherBinary) -lcrypto
	$(CC) $(dehasher) -o $(dehasherBinary) -lcrypto
	$(CC) $(csvHeaderPrinter) -o $(csvHeaderPrinterBinary)

helpHasher: $(hasherBinary)
	./$(hasherBinary) "--help"
helpDehasher: $(dehasherBinary)
	./$(dehasherBinary) "--help"
csvHeaderPrinter: $(csvHeaderPrinterBinary)
	./$(csvHeaderPrinterBinary) "--help"

cleanup: $(hasherBinary) $(dehasherBinary) $(csvHeaderPrinterBinary)
	rm $(hasherBinary)
	rm $(dehasherBinary)
	rm $(csvHeaderPrinterBinary)
# end
