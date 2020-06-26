_VPATH	+=:$(ROOT)/ports/source

PROCOBJ	+= port_reset_pins_lib.o port_set_pins_analog_in_lib.o port_set_pins_analog_out_lib.o 
PROCOBJ	+= port_set_pins_digital_in_lib.o port_set_pins_digital_out_lib.o port_read_bits_lib.o port_write_lib.o
PROCOBJ	+= port_clear_bits_lib.o port_set_bits_lib.o port_toggle_bits_lib.o port_read_lib.o
PROCOBJ	+= _ports_tbl_libp.o

_ports_tbl_libp.o: _ports_tbl_libp.c
	$(CC) $(CFLAGS) -c $< -o $@	


