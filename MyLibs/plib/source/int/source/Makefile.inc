_VPATH	+=:$(ROOT)/int/source

PROCOBJ	+= clear_core_sw0_lib.o clear_core_sw1_lib.o  
PROCOBJ	+= clear_core_sw0_leg_lib.o clear_core_sw1_leg_lib.o
PROCOBJ	+= set_core_sw0_lib.o set_core_sw1_lib.o
PROCOBJ	+= set_core_sw0_leg_lib.o set_core_sw1_leg_lib.o
PROCOBJ	+= int_configure_system_lib.o
PROCOBJ	+= int_enable_interrupts_lib.o int_disable_interrupts_lib.o int_restore_interrupts_lib.o
PROCOBJ	+= int_enable_mv_int_lib.o int_enable_sv_int_lib.o
PROCOBJ	+= int_get_pending_int_lib.o 
PROCOBJ	+= int_tbl_lib.o int_tbl_legacy_lib.o
PROCOBJ += int_set_ebase_lib.o int_set_ebase_vs_lib.o int_set_vector_spacing_lib.o

# These files aren't needed in the list of .o files, since they are sucked into the int_tbl_lib.c 
# int_tbl_1xx_2xx_legacy_lib.c
# int_tbl_1xx_2xx_lib.c
# int_tbl_3xx_4xx_legacy_lib.c
# int_tbl_3xx_4xx_lib.c
# int_tbl_5xx_6xx_7xx_legacy_lib.c
# int_tbl_5xx_6xx_7xx_lib.c

# This file is ignored because it is "obsolete" - I guess
# int_tbl_lib_obs.c
