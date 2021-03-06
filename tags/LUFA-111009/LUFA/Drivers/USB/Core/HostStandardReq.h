/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *  \brief USB host standard request management.
 *
 *  This file contains the function prototypes necessary for the issuing of outgoing standard control requests
 *  when the library is in USB host mode.
 *
 *  \note This file should not be included directly. It is automatically included as needed by the USB driver
 *        dispatch header located in LUFA/Drivers/USB/USB.h.
 */

#ifndef __HOSTSTDREQ_H__
#define __HOSTSTDREQ_H__

	/* Includes: */
		#include "../../../Common/Common.h"
		#include "USBMode.h"
		#include "StdRequestType.h"
		#include "USBController.h"

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_USB_DRIVER)
			#error Do not include this file directly. Include LUFA/Drivers/USB/USB.h instead.
		#endif

	/* Public Interface - May be used in end-application: */
		/* Enums: */
			/** Enum for the \ref USB_Host_SendControlRequest() return code, indicating the reason for the error
			 *  if the transfer of the request is unsuccessful.
			 *
			 *  \ingroup Group_PipeControlReq
			 */
			enum USB_Host_SendControlErrorCodes_t
			{
				HOST_SENDCONTROL_Successful         = 0, /**< No error occurred in the request transfer. */
				HOST_SENDCONTROL_DeviceDisconnected = 1, /**< The attached device was disconnected during the
				                                        *   request transfer.
				                                        */
				HOST_SENDCONTROL_PipeError          = 2, /**< An error occurred in the pipe while sending the request. */
				HOST_SENDCONTROL_SetupStalled       = 3, /**< The attached device stalled the request, usually
				                                        *   indicating that the request is unsupported on the device.
				                                        */
				HOST_SENDCONTROL_SoftwareTimeOut    = 4, /**< The request or data transfer timed out. */
			};

		/* Global Variables: */
			/** Indicates the currently set configuration number of the attached device. This indicates the currently
			 *  selected configuration value if one has been set sucessfully, or 0 if no configuration has been selected.
			 *
			 *  To set a device configuration, call the \ref USB_Host_SetDeviceConfiguration() function.
			 *
			 *  \note This variable should be treated as read-only in the user application, and never manually
			 *        changed in value.
			 *
			 *  \ingroup Group_Host
			 */
			extern uint8_t USB_Host_ConfigurationNumber;
			
		/* Function Prototypes: */
			/** Sends the request stored in the \ref USB_ControlRequest global structure to the attached device,
			 *  and transfers the data stored in the buffer to the device, or from the device to the buffer
			 *  as requested. The transfer is made on the currently selected pipe.
			 *
			 *  \ingroup Group_PipeControlReq
			 *
			 *  \param[in] BufferPtr  Pointer to the start of the data buffer if the request has a data stage, or
			 *                        \c NULL if the request transfers no data to or from the device.
			 *
			 *  \return A value from the \ref USB_Host_SendControlErrorCodes_t enum to indicate the result.
			 */
			uint8_t USB_Host_SendControlRequest(void* const BufferPtr);

			/** Sends a SET CONFIGURATION standard request to the attached device, with the given configuration index.
			 *
			 *  This routine will automatically update the \ref USB_HostState and \ref USB_Host_ConfigurationNumber
			 *  state variables according to the given function parameters and the result of the request.
			 *
			 *  \note After this routine returns, the control pipe will be selected.
			 *
			 *  \ingroup Group_PipeControlReq
			 *
			 *  \param[in] ConfigNumber  Configuration index to send to the device.
			 *
			 *  \return A value from the \ref USB_Host_SendControlErrorCodes_t enum to indicate the result.
			 */
			uint8_t USB_Host_SetDeviceConfiguration(const uint8_t ConfigNumber);

			/** Sends a GET DESCRIPTOR standard request to the attached device, requesting the device descriptor.
			 *  This can be used to easily retrieve information about the device such as its VID, PID and power
			 *  requirements.
			 *
			 *  \note After this routine returns, the control pipe will be selected.
			 *
			 *  \ingroup Group_PipeControlReq
			 *
			 *  \param[out] DeviceDescriptorPtr  Pointer to the destination device descriptor structure where
			 *                                   the read data is to be stored.
			 *
			 *  \return A value from the \ref USB_Host_SendControlErrorCodes_t enum to indicate the result.
			 */
			uint8_t USB_Host_GetDeviceDescriptor(void* const DeviceDescriptorPtr) ATTR_NON_NULL_PTR_ARG(1);

			/** Sends a GET DESCRIPTOR standard request to the attached device, requesting the string descriptor
			 *  of the specified index. This can be used to easily retrieve string descriptors from the device by
			 *  index, after the index is obtained from the Device or Configuration descriptors.
			 *
			 *  \note After this routine returns, the control pipe will be selected.
			 *
			 *  \ingroup Group_PipeControlReq
			 *
			 *  \param[in]  Index        Index of the string index to retrieve.
			 *  \param[out] Buffer       Pointer to the destination buffer where the retrieved string descriptor is
			 *                           to be stored.
			 *  \param[in] BufferLength  Maximum size of the string descriptor which can be stored into the buffer.
			 *
			 *  \return A value from the \ref USB_Host_SendControlErrorCodes_t enum to indicate the result.
			 */
			uint8_t USB_Host_GetDeviceStringDescriptor(const uint8_t Index,
			                                           void* const Buffer,
			                                           const uint8_t BufferLength) ATTR_NON_NULL_PTR_ARG(2);

			/** Retrieves the current feature status of the attached device, via a GET STATUS standard request. The
			 *  retrieved feature status can then be examined by masking the retrieved value with the various
			 *  FEATURE_* masks for bus/self power information and remote wakeup support.
			 *
			 *  \note After this routine returns, the control pipe will be selected.
			 *
			 *  \ingroup Group_PipeControlReq
			 *
			 *  \param[out]  FeatureStatus  Location where the retrieved feature status should be stored.
			 *
			 *  \return A value from the \ref USB_Host_SendControlErrorCodes_t enum to indicate the result.
			 */
			uint8_t USB_Host_GetDeviceStatus(uint8_t* const FeatureStatus) ATTR_NON_NULL_PTR_ARG(1);

			/** Clears a stall condition on the given pipe, via a CLEAR FEATURE standard request to the attached device.
			 *
			 *  \note After this routine returns, the control pipe will be selected.
			 *
			 *  \ingroup Group_PipeControlReq
			 *
			 *  \param[in] EndpointAddress  Address of the endpoint to clear, including the endpoint's direction.
			 *
			 *  \return A value from the \ref USB_Host_SendControlErrorCodes_t enum to indicate the result.
			 */
			uint8_t USB_Host_ClearPipeStall(const uint8_t EndpointAddress);

			/** Selects a given alternative setting for the specified interface, via a SET INTERFACE standard request to
			 *  the attached device.
			 *
			 *  \note After this routine returns, the control pipe will be selected.
			 *
			 *  \ingroup Group_PipeControlReq
			 *
			 *  \param[in] InterfaceIndex  Index of the interface whose alternative setting is to be altered.
			 *  \param[in] AltSetting      Index of the interface's alternative setting which is to be selected.
			 *
			 *  \return A value from the \ref USB_Host_SendControlErrorCodes_t enum to indicate the result.
			 */
			uint8_t USB_Host_SetInterfaceAltSetting(const uint8_t InterfaceIndex,
													const uint8_t AltSetting);

	/* Private Interface - For use in library only: */
	#if !defined(__DOXYGEN__)
		/* Enums: */
			enum USB_WaitForTypes_t
			{
				USB_HOST_WAITFOR_SetupSent,
				USB_HOST_WAITFOR_InReceived,
				USB_HOST_WAITFOR_OutReady,
			};

		/* Function Prototypes: */
			#if defined(__INCLUDE_FROM_HOSTSTDREQ_C)
				static uint8_t USB_Host_WaitForIOS(const uint8_t WaitType);
			#endif
	#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

