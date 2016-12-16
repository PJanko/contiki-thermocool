/*
 * Copyright (c) 2016, Antonio Lignan - antonio.lignan@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */
/*---------------------------------------------------------------------------*/
/**
 * \addtogroup weather-station
 * @{
 *
 * \file
 * Weather station application
 *
 * \author
 *         Antonio Lignan <antonio.lignan@gmail.com>
 */
/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include "lib/random.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-udp-packet.h"
#include "weather-station.h"
#include "dev/relay.h"
#include "dev/leds.h"
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "lib/sensors.h"
#include <math.h>

#include "dev/adc-sensors.h"
/*---------------------------------------------------------------------------*/
#define DEBUG DEBUG_PRINT12
#include "net/ip/uip-debug.h"
/*---------------------------------------------------------------------------*/
#define APP_STRING            "Weather station UDP client"
#define VERSION_STRING        "v.0.1"
#define SEND_INTERVAL		  10
#define SEND_TIME	          (random_rand() % (SEND_INTERVAL))
/*---------------------------------------------------------------------------*/
static struct uip_udp_conn *client_conn;
static uip_ipaddr_t server_ipaddr;
/*---------------------------------------------------------------------------*/
static weather_station_t *msgPtr = &weather_sensor_values;
/*---------------------------------------------------------------------------*/
PROCESS_NAME(weather_station_process);
PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);
/*---------------------------------------------------------------------------*/


static void
tcpip_handler(void)
{
  char *str;
  uint8_t state;

  
  if(uip_newdata()) {
    str = uip_appdata;
    str[uip_datalen()] = '\0';

    weather_sensor_values.activate=(uint8_t)atoi(str);
   
    if (!weather_sensor_values.activate)
      relay.value(RELAY_ON);
    else 
      relay.value(RELAY_OFF);
     printf("Relay: switch is now --> %u\n", relay.status(SENSORS_ACTIVE));

    printf("DATA recv '%s'\n", str);
  }
}
/*---------------------------------------------------------------------------*/
static void
send_packet(void)
{
  /* leds_toggle(LEDS_RED); */
  PRINTF("Send --> %u\n", msgPtr->counter);
  uip_udp_packet_sendto(client_conn, msgPtr, sizeof(weather_station_t),
                        &server_ipaddr, UIP_HTONS(UDP_SERVER_PORT));
}
/*---------------------------------------------------------------------------*/
static void
print_local_addresses(void)
{
  int i;
  uint8_t state;

  PRINTF("Client IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n");
      if (state == ADDR_TENTATIVE) {
	    uip_ds6_if.addr_list[i].state = ADDR_PREFERRED;
      }
    }
  }
  PRINTF("Server address: ");
  PRINT6ADDR(&server_ipaddr);
  PRINTF("\n");
}
/*---------------------------------------------------------------------------*/
static void
set_global_address(void)
{
  uip_ipaddr_t ipaddr;
  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
  uip_ip6addr(&server_ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 1);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
  PROCESS_BEGIN();

  PROCESS_PAUSE();
  set_global_address();
  SENSORS_ACTIVATE(relay);
 
  PRINTF("UDP client process started\n");
  print_local_addresses();

  printf("*-----------------------------*\n");
  printf("%s - %s\n", APP_STRING, VERSION_STRING);
  printf("*-----------------------------*\n");

  /* new connection with remote host */
  client_conn = udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT), NULL); 
  if(client_conn == NULL) {
    PRINTF("No UDP connection available, exiting the process!\n");
    PROCESS_EXIT();
  }

  udp_bind(client_conn, UIP_HTONS(UDP_CLIENT_PORT)); 

  PRINTF("Created a connection with the server ");
  PRINT6ADDR(&client_conn->ripaddr);
  PRINTF(" local/remote port %u/%u\n",
	UIP_HTONS(client_conn->lport), UIP_HTONS(client_conn->rport));

  /* Start the weather station process */
  process_start(&weather_station_process, NULL);

  while(1) {
    PROCESS_YIELD();
    if(ev == tcpip_event) {
      tcpip_handler();
    }

    if(ev == weather_station_started_event) {
      printf("Weather station started\n");
      msgPtr->activate=1;

    } else if (ev == weather_station_data_event) {
      printf("Weather station data available\n");
      send_packet();
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/** @} */
