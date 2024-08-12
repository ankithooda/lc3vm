lc3:
	mkdir -p bin && gcc -g -o bin/lc3-vm lc3.c lc3_hardware.c

clean:
	rm -rf bin

