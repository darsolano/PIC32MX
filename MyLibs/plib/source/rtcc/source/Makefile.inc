_VPATH	+=:$(ROOT)/rtcc/source

PROCOBJ	+= _rtcc_set_alrm_time_and_date_lib.o _rtcc_set_time_and_date_lib.o 
PROCOBJ	+= _rtcc_write_in_alarm_window_lib.o _rtcc_wait_clock_off_lib.o
PROCOBJ	+= rtcc_alarm_disable_lib.o rtcc_alarm_enable_lib.o rtcc_chime_disable_lib.o 
PROCOBJ	+= rtcc_chime_enable_lib.o rtcc_disable_lib.o rtcc_enable_lib.o
PROCOBJ	+= rtcc_init_lib.o rtcc_set_alarm_date_lib.o 
PROCOBJ	+= rtcc_set_alarm_rpt_count_lib.o rtcc_set_alarm_rpt_lib.o 
PROCOBJ	+= rtcc_set_alarm_time_date_lib.o rtcc_set_alarm_time_lib.o 
PROCOBJ	+= rtcc_set_calibration_lib.o rtcc_set_date_lib.o
PROCOBJ	+= rtcc_set_time_date_lib.o rtcc_set_time_lib.o rtcc_shutdown_lib.o
PROCOBJ	+= rtcc_weekday_lib.o rtcc_open_lib.o 
PROCOBJ	+= rtcc_alarm_pulse_high_lib.o rtcc_alarm_pulse_low_lib.o rtcc_alarm_pulse_toggle_lib.o
 


