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
 * \addtogroup zoul-examples
 * @{
 *
 * \defgroup weather-station Weather station
 *
 * DIY Weather station based on the Zoul module
 * @{
 *
 * \file
 *  Configuration file for the Weather station
 *
 * \author
 *         Antonio Lignan <antonio.lignan@gmail.com>
 */
/*---------------------------------------------------------------------------*/
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_
/*---------------------------------------------------------------------------*/

/* Do not drop below LPM1 to avoid problems with the I2C (theory not tested) */
// #define LPM_CONF_MAX_PM       1

/* Disable duty cycle (increases battery drain, for testing only) */
// #define NETSTACK_CONF_RDC     nullrdc_driver

/*---------------------------------------------------------------------------*/
/* Connection pin-out 
 * -------------+-----------------------------
 * Rain gauge   | PC0
 * Anemometer   | PC1
 * Wind vane    | ADC1 (PA5)
 *--------------+-----------------------------
 * I2C sensors connected via the default I2C 5-pin connector
 */

#define WEATHER_METER_CONF_ANEMOMETER_PIN       1
#define WEATHER_METER_CONF_ANEMOMETER_PORT      GPIO_C_NUM
#define WEATHER_METER_CONF_ANEMOMETER_VECTOR    NVIC_INT_GPIO_PORT_C

#define WEATHER_METER_CONF_RAIN_GAUGE_PIN       0
#define WEATHER_METER_CONF_RAIN_GAUGE_PORT      GPIO_C_NUM
#define WEATHER_METER_CONF_RAIN_GAUGE_VECTOR    NVIC_INT_GPIO_PORT_C

#define WEATHER_METER_CONF_RAIN_WIND_VANE_ADC   ZOUL_SENSORS_ADC1

#endif /* PROJECT_CONF_H_ */
/*---------------------------------------------------------------------------*/
/**
 * @}
 * @}
 */
