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
#include "sys/etimer.h"
#include "dev/bmpx8x.h"
#include "dev/sht25.h"
#include "dev/weather-meter.h"
#include "weather-station.h"
#include "dev/leds.h"
#include "cfs/cfs.h"
#include "cfs/cfs-coffee.h"


#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/adc-sensors.h"
#include <math.h>


#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "httpd-simple.h"
/*---------------------------------------------------------------------------*/
#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define ADC_PIN              2
/*---------------------------------------------------------------------------*/
weather_station_t weather_sensor_values;
/*---------------------------------------------------------------------------*/
process_event_t weather_station_started_event;
process_event_t weather_station_data_event;
/*---------------------------------------------------------------------------*/
weather_station_config_t ws_config;

/*---------------------------------------------------------------------------*/
PROCESS(weather_station_process, "Weather Station process");
/*---------------------------------------------------------------------------*/

static void
rain_callback(uint16_t value)
{
  PRINTF("WS: *** Rain gauge over threshold (%u ticks)\n", value);
  weather_meter.configure(WEATHER_METER_RAIN_GAUGE_INT_OVER,
                          (value + RAIN_GAUGE_THRESHOLD_TICK));
}
/*---------------------------------------------------------------------------*/
static void
wind_speed_callback(uint16_t value)
{
  PRINTF("WS: *** Wind speed over threshold (%u ticks)\n", value);
}
/*---------------------------------------------------------------------------*/
static void
poll_sensors(void)
{
  //struct sensors_sensor *sensor;
  weather_sensor_values.counter++;

  

  /* Poll the temperature  sensor */

  adc_sensors.configure(ANALOG_AAC_SENSOR,2);
  weather_sensor_values.temperature= adc_sensors.value(ANALOG_AAC_SENSOR);
  
  



  
/* récupération de la température ERREUR A LA COMPILATION
PRINTF(" :%d",sensor->value(ANALOG_AAC_SENSOR));
weather_sensor_values.temperature=sensor->value(ANALOG_AAC_SENSOR);
PRINTF("Activate :%d",weather_sensor_values.activate);*/


 /* PRINTF("WS: Pressure = %u.%u(hPa)\n", (weather_sensor_values.atmospheric_pressure / 10),
                                    (weather_sensor_values.atmospheric_pressure % 10));*/

  PRINTF("WS: Temperature %02d.%02d ºC, ", (weather_sensor_values.temperature / 100),
                                       (weather_sensor_values.temperature % 100));

 

 
  /* Post the event */
  process_post(PROCESS_BROADCAST, weather_station_data_event, NULL);
}
/*---------------------------------------------------------------------------*/
static int
interval_post_handler(char *key, int key_len, char *val, int val_len)
{
  int fd;
  int rv = 0;
  uint8_t buf[2];

  if(key_len != strlen("interval") ||
     strncasecmp(key, "interval", strlen("interval")) != 0) {
    return HTTPD_SIMPLE_POST_HANDLER_UNKNOWN;
  }

  rv = atoi(val);

  if(rv < WEATHER_STATION_WS_INTERVAL_MIN ||
     rv > WEATHER_STATION_WS_INTERVAL_MAX) {
    return HTTPD_SIMPLE_POST_HANDLER_ERROR;
  }

  ws_config.interval = rv * CLOCK_SECOND;
  PRINTF("WS: new interval tick is: %u\n", rv);

  fd = cfs_open("WS_int", CFS_READ | CFS_WRITE);
  if(fd >= 0) {
    buf[0] = ((uint8_t *)&ws_config.interval)[0];
    buf[1] = ((uint8_t *)&ws_config.interval)[1];
    if(cfs_write(fd, &buf, 2) > 0) {
      PRINTF("WS: interval saved in flash\n");
    }
    cfs_close(fd);
  }

  return HTTPD_SIMPLE_POST_HANDLER_OK;
}
/*---------------------------------------------------------------------------*/
HTTPD_SIMPLE_POST_HANDLER(interval, interval_post_handler);
/*---------------------------------------------------------------------------*/

/*Process pour récupérer la température
PROCESS(temperature_process,"get the temperature");
AUTOSTART_PROCESSES(&temperature_process);

PROCESS_THREAD(temperature_process,ev,data){
  PROCESS_BEGIN();
  struct sensors_sensor *sensor;
  sensor->configure(ANALOG_AAC_SENSOR,5);
  static struct timer et;
  timer_set(&et,CLOCK_SECOND/3);

  while(1){
  
    if(timer_expired(&et))
    {
      weather_sensor_values.temperature=sensor->value(ANALOG_AAC_SENSOR);
      //printf("temperature : %d \n",weather_sensor_values.temperature);
      timer_reset(&et);
    }
  }

 PROCESS_END();
  
}*/

PROCESS_THREAD(weather_station_process, ev, data)
{
  static int fd;
  static uint8_t buf[2];
  static struct etimer et;
  

  PROCESS_BEGIN();



  weather_station_started_event = process_alloc_event();
  weather_station_data_event = process_alloc_event();



  /* Post the event to notify subscribers */
  process_post(PROCESS_BROADCAST, weather_station_started_event, NULL);

  /* Start the webserver */
  process_start(&httpd_simple_process, NULL);
  
  /*Thread pour la température*/
  //process_start(&temperature_process, NULL);

  /* Read configuration from flash */
  ws_config.interval = WEATHER_STATION_SENSOR_PERIOD;
  fd = cfs_open("WS_int", CFS_READ | CFS_WRITE);
  if(fd >= 0) {
    if(cfs_read(fd, &buf, 2) > 0) {
      ws_config.interval = (buf[1] << 8) + buf[0];
    }
    cfs_close(fd);
  }

  PRINTF("WS: interval %u\n", (uint16_t)(ws_config.interval / CLOCK_SECOND));

  /* The HTTPD_SIMPLE_POST_HANDLER macro should have already created the
   * interval_handler()
   */
  httpd_simple_register_post_handler(&interval_handler);

  /* Start the periodic process */
  etimer_set(&et, ws_config.interval);

  weather_sensor_values.counter = 0;


  while(1) {

    PROCESS_YIELD();

    if(ev == httpd_simple_event_new_config) {
      PRINTF("WS: New configuration over httpd, restarting timer\n");
      etimer_stop(&et);
      etimer_set(&et, ws_config.interval);
    }

    if(ev == PROCESS_EVENT_TIMER && data == &et) {
      poll_sensors();
      etimer_set(&et, ws_config.interval);
    }
  }

  PROCESS_END();
}


/*---------------------------------------------------------------------------*/
/** @} */
