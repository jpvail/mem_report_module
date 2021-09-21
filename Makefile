kernel ?= $(shell uname -r)
kdir ?= /lib/modules/$(kernel)/build

obj-m = mem_report.o

all: mem_report.ko mem_report_cli

mem_report_cli: mem_report_cli.c
	$(CROSS_COMPILE)gcc -Wall $< -o $@

mem_report.ko: mem_report.h mem_report.c
	$(MAKE) -C $(kdir) M=$$(pwd)

clean:
	rm -f *.ko *.o mem_report_cli

run: all
