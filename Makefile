# I know it defies a bit the purpose of having nob... but for now I like it like this

SRC := $(wildcard clear/qlock*.c) $(wildcard opti1/qlock*.c) $(wildcard opti2/qlock*.c)
PCK := $(subst qlock,pck_qlock, $(SRC))
BIN := $(subst qlock,bin_qlock, $(SRC:.c=))

all: nob $(PCK) $(BIN)

nob: nob.c nob.h stb_c_lexer.h
	gcc -o nob nob.c

clean:
	rm -f clear/pck_*
	rm -f clear/bin_*
	rm -f opti1/pck_*
	rm -f opti1/bin_*
	rm -f opti2/pck_*
	rm -f opti2/bin_*

clear/pck_qloc%.c clear/bin_qloc%: clear/qloc%.c nob
	./nob clear/qloc$*.c
	cp build/qlock.c clear/pck_qloc$*.c
	cp build/qlock clear/bin_qloc$*
	cat clear/pck_qloc$*.c
opti1/pck_qloc%.c opti1/bin_qloc%: opti1/qloc%.c nob
	./nob opti1/qloc$*.c
	cp build/qlock.c opti1/pck_qloc$*.c
	cp build/qlock opti1/bin_qloc$*
	cat opti1/pck_qloc$*.c
opti2/pck_qloc%.c opti2/bin_qloc%: opti2/qloc%.c nob
	./nob opti2/qloc$*.c
	cp build/qlock.c opti2/pck_qloc$*.c
	cp build/qlock opti2/bin_qloc$*
	cat opti2/pck_qloc$*.c
