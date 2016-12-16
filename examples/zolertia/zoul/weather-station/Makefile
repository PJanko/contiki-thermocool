all: udp-client

TARGET = zoul

CONTIKI = ../../../..

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
CONTIKI_TARGET_SOURCEFILES += sht25.c bmpx8x.c weather-meter.c adc-sensors.c sensors.c relay.c
PROJECT_SOURCEFILES += weather-sensors.c weather-station.c httpd-simple.c 

CONTIKI_WITH_IPV6 = 1
include $(CONTIKI)/Makefile.include
