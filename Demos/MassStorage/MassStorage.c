/*
             MyUSB Library
     Copyright (C) Dean Camera, 2007.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com

 Released under the GPL Licence, Version 3
*/

/*
	Mass Storage demonstration application. This gives a simple reference
	application for implementing a USB Mass Storage device using the basic
	USB UFI drivers in all modern OSes (i.e. no special drivers required).
	
	On startup the system will automatically enumerate and function as an
	external mass storage device which may be formatted and used in the
	same manner as commercial USB Mass Storage devices.
	
	Only one Logical Unit (LUN) is currently supported by this example,
	allowing for one external storage device to be enumerated by the host.
	
	The two USB status LEDs indicate the status of the device. The first
	LED is lit in green when the device may be removed from the host, and
	red when the host has requested that it not be removed (i.e., while
	the host I/O cache is non-empty or the device is busy). The second LED
	is lit in green when idling, orange when executing a command from the
	host and red when the host send an invalid USB command.
*/

/*
                            _,.-------.,_
                        ,;~'             '~;, 
                      ,;                     ;,
                     ;                         ;
                    ,'                         ',
                   ,;                           ;,
                   ; ;      .           .      ; ;
                   | ;   ______       ______   ; | 
                   |  `/~"     ~" . "~     "~\'  |
                   |  ~  ,-~~~^~, | ,~^~~~-,  ~  |
                    |   |        }:{        |   | 
                    |   l       / | \       !   |
                    .~  (__,.--" .^. "--.,__)  ~. 
                    |     ---;' / | \ `;---     |  
                     \__.       \/^\/       .__/  
                      V| \                 / |V  
                       | |T~\___!___!___/~T| |  
                       | |`IIII_I_I_I_IIII'| |  
                       |  \,III I I I III,/  |  
                        \   `~~~~~~~~~~'    /
                          \   .       .   /     -dcau (4/15/95)
                            \.    ^    ./   
                              ^~~~^~~~^   

                           **** DANGER *****

                     UNFINISHED AND NON-OPERATIONAL

	This USB device is incomplete, and may cause system instability including
	blue-screen, driver failure or host freezes if used. For development only!
*/

#include "MassStorage.h"

/* Project Tags, for reading out using the ButtLoad project */
BUTTLOADTAG(ProjName,  "MyUSB MassStore App");
BUTTLOADTAG(BuildTime, __TIME__);
BUTTLOADTAG(BuildDate, __DATE__);

/* Scheduler Task ID list */
TASK_ID_LIST
{
	USB_USBTask_ID,
	USB_MassStorage_ID,
};

/* Scheduler Task List */
TASK_LIST
{
	{ TaskID: USB_USBTask_ID          , TaskName: USB_USBTask          , TaskStatus: TASK_RUN  },
	{ TaskID: USB_MassStorage_ID      , TaskName: USB_MassStorage      , TaskStatus: TASK_RUN  },
};

/* Global Variables */
CommandBlockWrapper_t  CommandBlock;
CommandStatusWrapper_t CommandStatus = { Signature: CSW_SIGNATURE };

int main(void)
{
	/* Disable Clock Division */
	CLKPR = (1 << CLKPCE);
	CLKPR = 0;

	/* Hardware Initialization */
	Bicolour_Init();
	Dataflash_Init();
	SerialStream_Init(9600); // DEBUG
	
	/* Initial LED colour - Double red to indicate USB not ready */
	Bicolour_SetLeds(BICOLOUR_LED1_RED | BICOLOUR_LED2_RED);
	
	/* Initialize USB Subsystem */
	USB_Init(USB_MODE_DEVICE, USB_DEV_OPT_HIGHSPEED | USB_OPT_REG_ENABLED);

	/* Scheduling - routine never returns, so put this last in the main function */
	Scheduler_Start();
}

EVENT_HANDLER(USB_CreateEndpoints)
{
	/* Setup Mass Storage In and Out Endpoints */
	Endpoint_ConfigureEndpoint(MASS_STORAGE_IN_EPNUM, ENDPOINT_TYPE_BULK,
		                       ENDPOINT_DIR_IN, MASS_STORAGE_IO_EPSIZE,
	                           ENDPOINT_BANK_DOUBLE);

	Endpoint_ConfigureEndpoint(MASS_STORAGE_OUT_EPNUM, ENDPOINT_TYPE_BULK,
		                       ENDPOINT_DIR_OUT, MASS_STORAGE_IO_EPSIZE,
	                           ENDPOINT_BANK_DOUBLE);

	/* Double green to indicate USB connected and ready */
	Bicolour_SetLeds(BICOLOUR_LED1_GREEN | BICOLOUR_LED2_GREEN);
}

EVENT_HANDLER(USB_UnhandledControlPacket)
{
	Endpoint_Ignore_Word();

	/* Process UFI specific control requests */
	switch (Request)
	{
		case MASS_STORAGE_RESET:
			Endpoint_ClearSetupRecieved();
			Endpoint_In_Clear();

			break;
		case GET_NUMBER_OF_LUNS:
			Endpoint_ClearSetupRecieved();			
			Endpoint_Write_Byte(0x00);			
			Endpoint_In_Clear();

			break;
	}
}
	
TASK(USB_MassStorage)
{
	/* Check if the USB System is connected to a Host */
	if (USB_IsConnected)
	{
		/* Select the Data Out Endpoint */
		Endpoint_SelectEndpoint(MASS_STORAGE_OUT_EPNUM);
		
		/* Check to see if a command from the host has been issued */
		if (Endpoint_Out_IsRecieved())
		{	
			/* Set LED2 orange - busy */
			Bicolour_SetLed(BICOLOUR_LED2, BICOLOUR_LED2_ORANGE);

			/* Process sent command block from the host */
			ProcessCommandBlock();

			/* Load in the CBW tag into the CSW to link them together */
			CommandStatus.Tag = CommandBlock.Header.Tag;

			/* Return command status block to the host */
			ReturnCommandStatus();
		}
	}
}

void ProcessCommandBlock(void)
{
	uint8_t* CommandBlockPtr = (uint8_t*)&CommandBlock;

	/* Select the Data Out endpoint */
	Endpoint_SelectEndpoint(MASS_STORAGE_OUT_EPNUM);

	/* Read in command block header */
	for (uint8_t i = 0; i < sizeof(CommandBlock.Header); i++)
	  *(CommandBlockPtr++) = Endpoint_Read_Byte();

	/* Verify the command block - abort if invalid */
	if ((CommandBlock.Header.Signature != CBW_SIGNATURE) ||
	    (CommandBlock.Header.LUN != 0x00) ||
		(CommandBlock.Header.SCSICommandLength > MAX_SCSI_COMMAND_LENGTH))
	{
		/* Bicolour LED2 to red - error */
			Bicolour_SetLed(BICOLOUR_LED2, BICOLOUR_LED2_RED);

		/* Stall both data pipes until reset by host */
		Endpoint_Stall_Transaction();
		Endpoint_SelectEndpoint(MASS_STORAGE_IN_EPNUM);
		Endpoint_Stall_Transaction();
		
		return;
	}

	/* Read in command block command data */
	for (uint8_t b = 0; b < CommandBlock.Header.SCSICommandLength; b++)
	  *(CommandBlockPtr++) = Endpoint_Read_Byte();
	  
	/* Clear the endpoint */
	Endpoint_Out_Clear();

	/* Check direction of command, select Data IN endpoint if command is to the device */
	if (CommandBlock.Header.Flags & (1 << 7))
	  Endpoint_SelectEndpoint(MASS_STORAGE_IN_EPNUM);

	/* Decode the recieved SCSI command */
	SCSI_DecodeSCSICommand();

	/* Load in the Command Data residue into the CSW */
	CommandStatus.SCSICommandResidue = CommandBlock.Header.DataTransferLength;

	/* Stall data pipe if command failed */
	if ((CommandStatus.Status == Command_Fail) &&
	    (CommandStatus.SCSICommandResidue))
	{
		Endpoint_Stall_Transaction();
	}
}

void ReturnCommandStatus(void)
{
	uint8_t* CommandStatusPtr = (uint8_t*)&CommandStatus;

	/* Select the Data Out endpoint */
	Endpoint_SelectEndpoint(MASS_STORAGE_OUT_EPNUM);

	/* While data pipe is stalled, process control requests */
	while (Endpoint_IsStalled())
	{
		USB_USBTask();
		Endpoint_SelectEndpoint(MASS_STORAGE_OUT_EPNUM);
	}

	/* Select the Data In endpoint */
	Endpoint_SelectEndpoint(MASS_STORAGE_IN_EPNUM);

	/* While data pipe is stalled, process control requests */
	while (Endpoint_IsStalled())
	{
		USB_USBTask();
		Endpoint_SelectEndpoint(MASS_STORAGE_IN_EPNUM);
	}
	
	/* Wait until read/write to IN data endpoint allowed */
	while (!(Endpoint_ReadWriteAllowed()));

	/* Write the CSW to the endpoint */
	for (uint8_t i = 0; i < sizeof(CommandStatus); i++)
	  Endpoint_Write_Byte(*(CommandStatusPtr++));
	
	/* Send the CSW */
	Endpoint_In_Clear();

	/* Bicolour LED2 to green - ready */
	Bicolour_SetLed(BICOLOUR_LED2, BICOLOUR_LED2_GREEN);
}