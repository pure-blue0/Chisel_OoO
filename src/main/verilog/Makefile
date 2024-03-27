TOP=GenAddress
VSRC=$(TOP).v
cppgen:$(VSRC)
	verilator --cc $(VSRC)

exgen:
	make -j -C obj_dir -f V$(TOP).mk

clean:
	rm -rf obj_dir

