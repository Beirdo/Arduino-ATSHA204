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

#ifndef SHA204_Library_SWI_h
#define SHA204_Library_SWI_h

/* bitbang_config.h */

#define PORT_ACCESS_TIME  		(630)	//! time it takes to toggle the pin at CPU clock of 16 MHz (ns)
#define START_PULSE_WIDTH  		(4340)	//! width of start pulse (ns)
#define BIT_DELAY	      		(4)		//! delay macro for width of one pulse (start pulse or zero pulse, in ns)
#define RX_TX_DELAY        		(15)		//! turn around time when switching from receive to transmit
#define START_PULSE_TIME_OUT	(255)	//! This value is decremented while waiting for the falling edge of a start pulse.
#define ZERO_PULSE_TIME_OUT		(26)	//! This value is decremented while waiting for the falling edge of a zero pulse.

/* swi_phys.h */

#define SWI_FUNCTION_RETCODE_SUCCESS     ((uint8_t) 0x00) //!< Communication with device succeeded.
#define SWI_FUNCTION_RETCODE_TIMEOUT     ((uint8_t) 0xF1) //!< Communication timed out.
#define SWI_FUNCTION_RETCODE_RX_FAIL     ((uint8_t) 0xF9) //!< Communication failed after at least one byte was received.

/* sha204_swi.c */
#define SHA204_SWI_FLAG_CMD     ((uint8_t) 0x77) //!< flag preceding a command
#define SHA204_SWI_FLAG_TX      ((uint8_t) 0x88) //!< flag requesting a response
#define SHA204_SWI_FLAG_IDLE    ((uint8_t) 0xBB) //!< flag requesting to go into Idle mode
#define SHA204_SWI_FLAG_SLEEP   ((uint8_t) 0xCC) //!< flag requesting to go into Sleep mode

#endif
