// ----------------------------------------------------------------------------
//         ATMEL Microcontroller Software Support  -  Colorado Springs, CO -
// ----------------------------------------------------------------------------
// DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
// DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
// OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ----------------------------------------------------------------------------
/** \file
 *  \brief  Example of an Application That Uses the SHA204 Library
 *  \author Atmel Crypto Products
 *  \date   October 7, 2010
*/

#include <stddef.h>                   // data type definitions

#include "sha204_lib_return_codes.h"  // declarations of function return codes
#include "sha204_comm_marshaling.h"   // definitions and declarations for the Command module


/** \brief This function evaluates a function return code
 *         and puts the device to sleep if the return code
 *         indicates that the device is awake.
 * \param[in] ret_code return code of the last call to a SHA204 library function
 */
void evaluate_ret_code(uint8_t ret_code)
{
	if ((ret_code == SHA204_PARSE_ERROR)
				|| (ret_code == SHA204_CMD_FAIL)
				|| (ret_code == SHA204_RX_FAIL))
		// We got some kind of response. Return codes of
		// SHA204_PARSE_ERROR and SHA204_CMD_FAIL indicate
		// a consistent response whereas SHA204_RX_FAIL
		// just indicates that we received some bytes,
		// possibly garbled. In all these cases we put
		// the device to sleep.
		(void) sha204p_sleep();
}



/** \brief This function serves as an example for
 *         the SHA204 MAC command.
 *
 *         In an infinite loop, it issues the same command
 *         sequence using the Command Marshaling layer of
 *         the SHA204 library.
 * @return exit status of application
 */
int main(void)
{
	// declared as "volatile" for easier debugging
	volatile uint8_t ret_code;

	uint8_t i;

	// Make the command buffer the size of the MAC command.
	static uint8_t command[MAC_COUNT_LONG];

	// Make the response buffer the size of a MAC response.
	static uint8_t response[MAC_RSP_SIZE];

   // expected MAC response in mode 0
	static const uint8_t mac_mode0_response_expected[MAC_RSP_SIZE] =
	{
			MAC_RSP_SIZE,                                   // count
			0x06, 0x67, 0x00, 0x4F, 0x28, 0x4D, 0x6E, 0x98,
			0x62, 0x04, 0xF4, 0x60, 0xA3, 0xE8, 0x75, 0x8A,
			0x59, 0x85, 0xA6, 0x79, 0x96, 0xC4, 0x8A, 0x88,
			0x46, 0x43, 0x4E, 0xB3, 0xDB, 0x58, 0xA4, 0xFB,
			0xE5, 0x73                                       // CRC
	};

	// data for challenge in MAC mode 0 command
	const uint8_t challenge[MAC_CHALLENGE_SIZE] = {
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
	};


	// Initialize the hardware interface.
	// Depending on which interface you have linked the
	// library to, it initializes SWI UART, SWI GPIO, or TWI.
	sha204p_init();

	while (1) {
		// The following code sequence wakes up the device,
		// issues a MAC command in mode 0
		// using the Command Marshaling layer, and puts
		// the device to sleep.

		for (i = 0; i < sizeof(response); i++)
			response[i] = 0;

		// Wake up the device.
		ret_code = sha204c_wakeup(&response[0]);
		if (ret_code != SHA204_SUCCESS)
			continue;

		// Mac command with mode = 0.
		ret_code = sha204m_execute(SHA204_MAC, 0, 0, MAC_CHALLENGE_SIZE, (uint8_t *) challenge,
					0, NULL, 0, NULL, sizeof(command), &command[0], sizeof(response), &response[0]);
		if (ret_code != SHA204_SUCCESS) {
			evaluate_ret_code(ret_code);
			continue;
		}

		// Put device to sleep.
		ret_code = sha204p_sleep();

		// Compare returned MAC with expected one.
		ret_code = SHA204_SUCCESS;
		for (i = 0; i < SHA204_RSP_SIZE_MAX; i++) {
			if (response[i] != mac_mode0_response_expected[i])
				ret_code = SHA204_GEN_FAIL;
		}
	}

	return (int) ret_code;
}
