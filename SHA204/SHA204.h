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

//FIXME: Update the following block to be built into the classes somehow
// IMPORTANT: The following line needs to match the type of connection you're using
#define SHA204_SWI_BITBANG

#ifdef SHA204_SWI_BITBANG
#ifndef SHA204_RESPONSE_TIMEOUT
#include "SHA204SWI.h" // Make sure to include the SWI_* stuff for this definition
//! SWI response timeout is the sum of receive timeout and the time it takes to send the TX flag.
#define SHA204_RESPONSE_TIMEOUT   ((uint16_t) SWI_RECEIVE_TIME_OUT + SWI_US_PER_BYTE)  //! SWI response timeout is the sum of receive timeout and the time it takes to send the TX flag.
#endif
#endif

#ifdef SHA204_I2C
#ifndef SHA204_RESPONSE_TIMEOUT
#define SHA204_RESPONSE_TIMEOUT     ((uint16_t) 37)
#endif

#endif

#include "Arduino.h"

#ifndef SHA204_Library_h
#define SHA204_Library_h

class SHA204 {
private:
	void calculate_crc(uint8_t length, uint8_t *data, uint8_t *crc);
	uint8_t check_crc(uint8_t *response);
	virtual uint8_t receive_bytes(uint8_t count, uint8_t *buffer) = 0;
	virtual uint8_t send_bytes(uint8_t count, uint8_t *buffer) = 0;
	virtual uint8_t send_byte(uint8_t value) = 0;
	virtual uint8_t receive_response(uint8_t size, uint8_t *response) = 0;
	virtual uint8_t send_command(uint8_t count, uint8_t * command) = 0;
	virtual uint8_t chip_wakeup() = 0; // Called this because wakeup() was causing method lookup issues with wakeup(*response)
	

public:
	uint8_t send_and_receive(uint8_t *tx_buffer, uint8_t rx_size, uint8_t *rx_buffer, uint8_t execution_delay, uint8_t execution_timeout);
	virtual uint8_t resync(uint8_t size, uint8_t *response) = 0;
	uint8_t random(uint8_t * tx_buffer, uint8_t * rx_buffer, uint8_t mode);
	uint8_t dev_rev(uint8_t *tx_buffer, uint8_t *rx_buffer);
	uint8_t read(uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t zone, uint16_t address);
	uint8_t execute(uint8_t op_code, uint8_t param1, uint16_t param2,
			uint8_t datalen1, uint8_t *data1, uint8_t datalen2, uint8_t *data2, uint8_t datalen3, uint8_t *data3,
			uint8_t tx_size, uint8_t *tx_buffer, uint8_t rx_size, uint8_t *rx_buffer);
	uint8_t check_parameters(uint8_t op_code, uint8_t param1, uint16_t param2,
			uint8_t datalen1, uint8_t *data1, uint8_t datalen2, uint8_t *data2, uint8_t datalen3, uint8_t *data3,
			uint8_t tx_size, uint8_t *tx_buffer, uint8_t rx_size, uint8_t *rx_buffer);

	uint8_t serialNumber(uint8_t *response);
	virtual uint8_t sleep() = 0;
	uint8_t wakeup(uint8_t *response);
};

#endif
