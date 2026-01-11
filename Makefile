# I know it defies a bit the purpose of having nob... but for now I like it like this

.PHONY: all
all: nob
	./nob qlock_clear
	./nob qlock_opti1
	./nob qlock_opti2
	./nob gol
	./nob badapple
	./nob lang

nob: nob.c nob.h stb_c_lexer.h
	gcc -o nob nob.c

.PHONY: clean
clean:
	rm -f qlock_clear/pck_*
	rm -f qlock_clear/bin_*
	rm -f qlock_opti1/pck_*
	rm -f qlock_opti1/bin_*
	rm -f qlock_opti2/pck_*
	rm -f qlock_opti2/bin_*
	rm -f gol/pck_*
	rm -f gol/bin_*
	rm -f badapple/pck_*
	rm -f badapple/bin_*
	rm -f lang/pck_*
	rm -f lang/bin_*
