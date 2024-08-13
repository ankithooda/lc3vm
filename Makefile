lc3:
	mkdir -p bin && gcc -Wall -Werror lc3_hardware.c lc3.c -o bin/lc3-vm -g

clean:
	rm -rf bin

