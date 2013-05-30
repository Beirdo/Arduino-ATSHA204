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
#include <Wire.h>

#ifdef TwoWire_h // Esure this code only gets built if you have Wire.h included in the main sketch

#include "SHA204.h"
#include "SHA204ReturnCodes.h"
#include "SHA204Definitions.h"
#include "SHA204I2C.h"

uint16_t SHA204I2C::SHA204_RESPONSE_TIMEOUT() {
	return SHA204_RESPONSE_TIMEOUT_VALUE;
}

SHA204I2C::SHA204I2C() {
	address = ((uint8_t) 0x64);
}

SHA204I2C::SHA204I2C(uint8_t deviceAddress) {
	address = deviceAddress;
}

void SHA204I2C::init() {
	chip_wakeup();
}

uint8_t SHA204I2C::receive_bytes(uint8_t count, uint8_t *data) {
	return receive_bytes(count, data, true);
}

uint8_t SHA204I2C::receive_bytes(uint8_t count, uint8_t *data, bool sendReadStartOp) {
	uint8_t i;

	if (sendReadStartOp) {
		start_operation(I2C_READ);
	}
	
	Wire.requestFrom(deviceAddress(), count);

	for (i = 0; i < count; i++) {
		while (!Wire.available()); // Wait for byte that is going to be read next
		*data++ = Wire.read(); // Store read value
	}

	return I2C_FUNCTION_RETCODE_SUCCESS;
}

uint8_t SHA204I2C::receive_byte(uint8_t *data) {
	start_operation(I2C_READ);
	Wire.requestFrom(deviceAddress(), (uint8_t)1);
	while (!Wire.available()); // Wait for byte that is going to be read next
	*data++ = Wire.read(); // Store read value

	return I2C_FUNCTION_RETCODE_SUCCESS;
}

uint8_t SHA204I2C::send_byte(uint8_t value) {
  return send_bytes(1, &value);
}

uint8_t SHA204I2C::send_bytes(uint8_t count, uint8_t *data) {
	start_operation(I2C_WRITE);
	Wire.beginTransmission(deviceAddress());
	int sent_bytes = Wire.write(data, count);
	int i2c_status = Wire.endTransmission();

	if (count > 0 && sent_bytes == count) {
		return I2C_FUNCTION_RETCODE_SUCCESS;
	}

	return I2C_FUNCTION_RETCODE_COMM_FAIL;
}

int SHA204I2C::start_operation(uint8_t readWrite) {
	Wire.beginTransmission(deviceAddress());
	Wire.write(&readWrite, (uint8_t)1);
	int status = Wire.endTransmission();
	
	delay(SHA204_COMMAND_EXEC_MAX); // Delay so device will respond to next set of communications

	return status;
}

uint8_t SHA204I2C::chip_wakeup() {
	// This was the only way short of manually adjusting the SDA pin to wake up the device
	Wire.beginTransmission(deviceAddress());
	int i2c_status = Wire.endTransmission();
	if (i2c_status != 0) {
		return SHA204_COMM_FAIL;
	}

	delay(SHA204_WAKEUP_DELAY);

	return SHA204_SUCCESS;
}

uint8_t SHA204I2C::receive_response(uint8_t size, uint8_t *response) {
	uint8_t count;
	uint8_t i2c_status;

	start_operation(I2C_READ);

	// Receive count byte.
	i2c_status = receive_byte(response);
	if (i2c_status != I2C_FUNCTION_RETCODE_SUCCESS) {
		return SHA204_COMM_FAIL;
	}

	count = response[SHA204_BUFFER_POS_COUNT];
	if ((count < SHA204_RSP_SIZE_MIN) || (count > size)) {
		return SHA204_INVALID_SIZE;
	}

	i2c_status = receive_bytes(count - 1, &response[SHA204_BUFFER_POS_DATA], false);

	if (i2c_status != I2C_FUNCTION_RETCODE_SUCCESS) {
		return SHA204_COMM_FAIL;
	}
	
	return SHA204_SUCCESS;
}

uint8_t SHA204I2C::send(uint8_t word_address, uint8_t count, uint8_t *buffer) {
	uint8_t i2c_status;

	//start_operation(I2C_WRITE);

	i2c_status = send_bytes(1, &word_address);
	if (i2c_status != I2C_FUNCTION_RETCODE_SUCCESS) {
		return SHA204_COMM_FAIL;
	}

	if (count == 0) {
		return SHA204_SUCCESS;
	}

	i2c_status = send_bytes(count, buffer);

	if (i2c_status != I2C_FUNCTION_RETCODE_SUCCESS) {
		return SHA204_COMM_FAIL;
	}
	
	return SHA204_SUCCESS;
}


uint8_t SHA204I2C::send_command(uint8_t count, uint8_t *command) {
	return send(SHA204_I2C_PACKET_FUNCTION_NORMAL, count, command);
}

uint8_t SHA204I2C::sleep(void) {
	return send(SHA204_I2C_PACKET_FUNCTION_SLEEP, 0, NULL);
}

uint8_t SHA204I2C::resync(uint8_t size, uint8_t *response) {
	uint8_t nine_clocks = 0xFF;
	
	send_bytes(1, &nine_clocks);

	// Try to send a Reset IO command if re-sync succeeded.
	return reset_io();
}

uint8_t SHA204I2C::reset_io() {
	return send(SHA204_I2C_PACKET_FUNCTION_RESET, 0, NULL);
}

#endif
