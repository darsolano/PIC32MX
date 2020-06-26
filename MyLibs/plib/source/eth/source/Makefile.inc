_VPATH	+=:$(ROOT)/eth/source

PROCOBJ	+= eth_ack_packet_lib.o
PROCOBJ	+= eth_close_lib.o
PROCOBJ	+= eth_config_lib.o
PROCOBJ	+= eth_descriptors_add_lib.o
PROCOBJ	+= eth_descriptors_get_lib.o
PROCOBJ	+= eth_descriptors_remove_lib.o
PROCOBJ	+= eth_events_lib.o
PROCOBJ	+= eth_flow_control_lib.o
PROCOBJ	+= eth_init_lib.o
PROCOBJ	+= eth_mac_config_lib.o
PROCOBJ	+= eth_mac_open_lib.o
PROCOBJ	+= eth_mac_set_address_max_lib.o
PROCOBJ	+= eth_rx_buffers_append_lib.o
PROCOBJ	+= eth_rx_filters_lib.o
PROCOBJ	+= eth_rx_get_packet_lib.o
PROCOBJ	+= eth_rx_set_buffer_size_lib.o
PROCOBJ	+= eth_status_get_lib.o
PROCOBJ	+= eth_tx_packet_lib.o
PROCOBJ	+= _eth_append_busy_list_lib.o
PROCOBJ	+= eth_miim_access.o
PROCOBJ	+= eth_miim_legacy.o


