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
 */
/*---------------------------------------------------------------------------*/
/**
 * \addtogroup weather-station
 * @{
 *
 * \file
 * Weather station header
 *
 * \author
 *         Antonio Lignan <antonio.lignan@gmail.com>
 */
/*---------------------------------------------------------------------------*/
#ifndef WEATHER_STATION_H_
#define WEATHER_STATION_H_
/*---------------------------------------------------------------------------*/
#define UDP_CLIENT_PORT  8765
#define UDP_SERVER_PORT  5678
#define UDP_REGISTER_ID  190
/*---------------------------------------------------------------------------*/
#define WEATHER_STATION_SENSOR_PERIOD      (CLOCK_SECOND * 10)
#define ANEMOMETER_THRESHOLD_TICK          13  /**< 16 Km/h */
#define RAIN_GAUGE_THRESHOLD_TICK          15  /**< 4.19 mm multipliers */
/*---------------------------------------------------------------------------*/
#define WEATHER_STATION_WS_INTERVAL_MIN    5
#define WEATHER_STATION_WS_INTERVAL_MAX    600
/*---------------------------------------------------------------------------*/
typedef struct weather_station {
  
  uint8_t activate;
  uint16_t temperature;
  uint16_t counter;
  /*uint16_t humidity;
  uint16_t atmospheric_pressure;
  uint16_t wind_speed;
  uint16_t wind_dir;
  uint16_t rain_mm;
  uint16_t wind_dir_avg_int;
  uint16_t wind_speed_avg;
  uint16_t wind_speed_avg_int;
  uint16_t wind_speed_max;*/
} weather_station_t;

extern weather_station_t weather_sensor_values;
/*---------------------------------------------------------------------------*/
typedef struct weather_station_config {
  clock_time_t interval;
} weather_station_config_t;

extern weather_station_config_t ws_config;
/*---------------------------------------------------------------------------*/
extern process_event_t weather_station_started_event;
extern process_event_t weather_station_data_event;
/*---------------------------------------------------------------------------*/
#endif /* WEATHER_STATION_H_ */
/** @} */
