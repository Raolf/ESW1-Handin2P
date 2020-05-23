/* Standard includes. */
#include <stdio.h>
#include <conio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "LoraDriver.h"
#include "param.h"
#include "task.h"
#include "appController.h"
#include "timers.h"
#include "semphr.h"
//#include <event.xsd>
#include <event_groups.h>
#include "Sensor.h"
#include "message_buffer.h"


/*defining eventgroups*/
#define BIT_0 ( 1<<0 )
#define BIT_4 ( 1<<4 )

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The times are converted from
milliseconds to ticks using the pdMS_TO_TICKS() macro. */
#define mainTASK_SEND_FREQUENCY_MS			pdMS_TO_TICKS( 200UL )
#define mainTIMER_SEND_FREQUENCY_MS			pdMS_TO_TICKS( 2000UL )

/* The number of items the queue can hold at once. */
#define mainQUEUE_LENGTH					( 2 )

/* The values sent to the queue receive task from the queue send task and the
queue send software timer respectively. */
#define mainVALUE_SENT_FROM_TASK			( 100UL )
#define mainVALUE_SENT_FROM_TIMER			( 200UL )

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );

/*
 * The callback function executed when the software timer expires.
 */
static void prvQueueSendTimerCallback( TimerHandle_t xTimerHandle );

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/* A software timer that is started from the tick hook. */
static TimerHandle_t xTimer = NULL;
EventGroupHandle_t xEventGroup;

/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/

void main_blinky( void )
{
const TickType_t xTimerPeriod = mainTIMER_SEND_FREQUENCY_MS;	

	Sensor sensor1 = createSensor();
	Sensor sensor2 = createSensor();
	Param param1 = createParam();
	Param param2 = createParam();
	Param param3 = createParam();
	Param param4 = createParam();
	MessageBufferHandle_t xMessageBuffer1;
	xMessageBuffer1 = xMessageBufferCreate(100);
	SemaphoreHandle_t semaphore1 = xSemaphoreCreateBinary();

	param1->sensor1 = sensor1;
	param2->sensor1 = sensor2;

	param3->xMessageBuffer1 = xMessageBuffer1;
	param3->semaphore1 = semaphore1;
	
	param4->sensor1 = sensor1;
	param4->sensor2 = sensor2;
	param4->xMessageBuffer1 = xMessageBuffer1;
	param4->semaphore1 = semaphore1;

	xEventGroup = xEventGroupCreate();
	param1->eventGroup1 = xEventGroup;
	param2->eventGroup1 = xEventGroup;
	param3->eventGroup1 = xEventGroup;

	xEventGroupSetBits(xEventGroup, BIT_0 | BIT_4);


	xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

	if( xQueue != NULL )
	{
		xTaskCreate(LoraDriverTask,
			"LDT",
			configMINIMAL_STACK_SIZE,
			param3,
			1,
			NULL);

		xTaskCreate(controllerTask,
			"CT",
			configMINIMAL_STACK_SIZE,
			param4,
			1,
			NULL);

		xTaskCreate(LoraDriverTask,
			"S1",
			configMINIMAL_STACK_SIZE,
			param3,
			1,
			NULL);

		xTaskCreate(LoraDriverTask,
			"S2",
			configMINIMAL_STACK_SIZE,
			param3,
			1,
			NULL);
		
		xTimerStart( xTimer, 0 ); /* The scheduler has not started so use a block time of 0. */
		
		vTaskStartScheduler();
	}

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details. */
	for( ;; );
}


