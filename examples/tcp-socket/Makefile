all: tcp-server tcp-serverV2 tcp-clientV2

CONTIKI=../..
UIP_CONF_IPV6=1
CFLAGS+= -DUIP_CONF_IPV6_RPL
#CONTIKI_WITH_IPV4 = 1
include $(CONTIKI)/Makefile.include
