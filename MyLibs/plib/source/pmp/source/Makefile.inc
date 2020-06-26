_VPATH	+=:$(ROOT)/pmp/source

PROCOBJ	+= pmp_master_read_byte_block_lib.o pmp_master_read_word_block_lib.o
PROCOBJ	+= pmp_master_write_byte_block_lib.o pmp_master_write_word_block_lib.o
PROCOBJ	+= pmp_slave_read_buffer_lib.o pmp_slave_read_buffers_lib.o 
PROCOBJ	+= pmp_slave_write_buffer_lib.o pmp_slave_write_buffers_lib.o


