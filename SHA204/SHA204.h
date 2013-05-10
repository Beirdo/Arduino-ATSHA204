/*
Copyright 2013 Nusku Networks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Arduino.h"

#ifndef SHA204_Library_h
#define SHA204_Library_h

class SHA204 {
private:
	uint8_t device_pin;
	volatile uint8_t *device_port_DDR, *device_port_OUT, *device_port_IN;
	void sha204c_calculate_crc(uint8_t length, uint8_t *data, uint8_t *crc);
	uint8_t sha204c_check_crc(uint8_t *response);
	void swi_set_signal_pin(uint8_t is_high);
	uint8_t swi_receive_bytes(uint8_t count, uint8_t *buffer);
	uint8_t swi_send_bytes(uint8_t count, uint8_t *buffer);
	uint8_t swi_send_byte(uint8_t value);
	uint8_t sha204p_receive_response(uint8_t size, uint8_t *response);
	uint8_t sha204p_send_command(uint8_t count, uint8_t * command);
	uint8_t sha204p_resync(uint8_t size, uint8_t *response);
	

public:
	SHA204(uint8_t pin);
	uint8_t sha204c_wakeup(uint8_t *response);
	uint8_t sha204c_send_and_receive(uint8_t *tx_buffer, uint8_t rx_size, uint8_t *rx_buffer, uint8_t execution_delay, uint8_t execution_timeout);
	uint8_t sha204c_resync(uint8_t size, uint8_t *response);	
	uint8_t sha204m_random(uint8_t * tx_buffer, uint8_t * rx_buffer, uint8_t mode);
	uint8_t sha204m_dev_rev(uint8_t *tx_buffer, uint8_t *rx_buffer);
	uint8_t sha204m_read(uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t zone, uint16_t address);
	uint8_t sha204m_execute(uint8_t op_code, uint8_t param1, uint16_t param2,
			uint8_t datalen1, uint8_t *data1, uint8_t datalen2, uint8_t *data2, uint8_t datalen3, uint8_t *data3,
			uint8_t tx_size, uint8_t *tx_buffer, uint8_t rx_size, uint8_t *rx_buffer);
	uint8_t sha204m_check_parameters(uint8_t op_code, uint8_t param1, uint16_t param2,
			uint8_t datalen1, uint8_t *data1, uint8_t datalen2, uint8_t *data2, uint8_t datalen3, uint8_t *data3,
			uint8_t tx_size, uint8_t *tx_buffer, uint8_t rx_size, uint8_t *rx_buffer);

	uint8_t getSerialNumber(uint8_t *response);
	uint8_t sha204p_sleep();
	uint8_t sha204p_wakeup();
};

#endif
