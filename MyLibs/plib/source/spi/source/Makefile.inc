_VPATH	+=:$(ROOT)/spi/source

PROCOBJ	+= spi_chn_close_lib.o 
PROCOBJ	+= spi_chn_data_rdy_lib.o 
PROCOBJ	+= spi_chn_getc_lib.o 
PROCOBJ	+= spi_chn_get_rov_lib.o 
PROCOBJ	+= spi_chn_gets_lib.o 
PROCOBJ	+= spi_chn_open_lib.o 
PROCOBJ	+= spi_chn_putc_lib.o 
PROCOBJ	+= spi_chn_puts_lib.o 
PROCOBJ	+= spi_chn_set_brg_lib.o 
PROCOBJ	+= spi_chn_change_mode_lib.o 
PROCOBJ	+= spi_chn_tx_buff_empty_lib.o 
PROCOBJ	+= spi_chn_configure_lib.o 
PROCOBJ	+= spi_chn_enable_lib.o 
PROCOBJ	+= spi_chn_set_bitrate_lib.o 
PROCOBJ	+= spi_chn_rx_buff_empty_lib.o  
PROCOBJ	+= spi_chn_rx_buff_count_lib.o 
PROCOBJ	+= spi_chn_tx_buff_full_lib.o 
PROCOBJ	+= spi_chn_tx_buff_count_lib.o 
PROCOBJ	+= spi_chn_get_tur_lib.o 
PROCOBJ	+= spi_chn_is_busy_lib.o 
PROCOBJ	+= spi_chn_get_status_lib.o 
PROCOBJ	+= spi_io_tbl_libp.o 
PROCOBJ	+= spi_map_tbl_libp.o
PROCOBJ	+= spi_chn_configure_ex_lib.o
PROCOBJ	+= spi_chn_open_ex_lib.o 

spi_io_tbl_libp.o: spi_io_tbl_libp.c
	$(CC) $(CFLAGS) -c $< -o $@	

spi_map_tbl_libp.o: spi_map_tbl_libp.c
	$(CC) $(CFLAGS) -c $< -o $@	
