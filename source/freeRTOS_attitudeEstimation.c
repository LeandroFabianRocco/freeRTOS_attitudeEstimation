/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    freeRTOS_attitudeEstimation.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "FXOS8700CQ.h"

/* FreeRTOS kernel includes. */

#include "FreeRTOS.h"
#include "task.h"



static void my_task(void *pvParameters);
/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void my_task(void *pvParameters)
{
	int16_t acc_xyz[3];
	int16_t mag_xyz[3];
    for (;;)
    {
    	FXOS8700CQ_Read_Accel_Data(I2C0, FXOS8700CQ_DEVICE_ADDRESS, acc_xyz);
    	FXOS8700CQ_Read_Magnet_Data(I2C0, FXOS8700CQ_DEVICE_ADDRESS, mag_xyz);
        PRINTF("accel = (%d, %d, %d); magnet = (%d, %d, %d)\r\n", acc_xyz[0], acc_xyz[1], acc_xyz[2], mag_xyz[0], mag_xyz[1], mag_xyz[2]);
    }
}
#define my_task_PRIORITY (configMAX_PRIORITIES - 1)

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    FXOS8700CQ_Init();
    FXOS8700CQ_Configure_Device();
    bool fxosFlag = FXOS8700CQ_ReadSensorWhoAmI();
    if (fxosFlag == true)
    	PRINTF("FXOS8700CQ found!!");
    else
    	PRINTF("FXOS8700CQ not found!!");

    if (xTaskCreate(my_task, "my_task", configMINIMAL_STACK_SIZE + 10, NULL, my_task_PRIORITY, NULL) != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	vTaskStartScheduler();
	/* Enter an infinite loop, just incrementing a counter. */
	while(1) {
	}
	return 0;
}
