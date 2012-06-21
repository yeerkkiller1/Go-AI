VERSION = 1.0.4

goDummy:	dummy.o gmp.o
	cc dummy.o gmp.o -o goDummy

dummy.o:	dummy.c gmp.h
	cc -c dummy.c -Wall

gmp.o:	gmp.c gmp.h
	cc -c gmp.c -Wall

tar:	dummy.c dummy.h gmp.c gmp.h Makefile README
	cd ..; tar cvf - goDummy-$(VERSION)/dummy.c goDummy-$(VERSION)/dummy.h goDummy-$(VERSION)/gmp.c goDummy-$(VERSION)/gmp.h goDummy-$(VERSION)/Makefile goDummy-$(VERSION)/README | gzip -9v >goDummy-$(VERSION).tar.gz
