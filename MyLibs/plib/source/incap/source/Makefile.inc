_VPATH	+=:$(ROOT)/incap/source

PROCOBJ	+= read_capture1_lib.o
PROCOBJ	+= read_capture2_lib.o
PROCOBJ	+= read_capture3_lib.o
PROCOBJ	+= read_capture4_lib.o
PROCOBJ	+= read_capture5_lib.o

# All these files are obsolete, and are implemented via #define macros in incap.h
# close_capture1_lib.c
# close_capture2_lib.c
# close_capture3_lib.c
# close_capture4_lib.c
# close_capture5_lib.c
# config_int_capture1_lib.c
# config_int_capture2_lib.c
# config_int_capture3_lib.c
# config_int_capture4_lib.c
# config_int_capture5_lib.c
