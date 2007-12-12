/*
             MyUSB Library
     Copyright (C) Dean Camera, 2007.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com

 Released under the GPL Licence, Version 3
*/

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

	/* Includes: */
		#include <MyUSB/Drivers/USB/USB.h>

	/* Macros: */
		#define DTYPE_AudioInterface        0x24
		#define DTYPE_AudioEndpoint         0x25

		#define DSUBTYPE_Header             0x01
		#define DSUBTYPE_InputTerminal      0x02
		#define DSUBTYPE_OutputTerminal     0x03
		#define DSUBTYPE_FeatureUnit        0x06

		#define DSUBTYPE_General            0x01
		#define DSUBTYPE_Format             0x02
		
		#define CHANNEL_LEFT_FRONT          (1 << 0)
		#define CHANNEL_RIGHT_FRONT         (1 << 1)
		#define CHANNEL_CENTER_FRONT        (1 << 2)
		#define CHANNEL_LOW_FREQ_ENHANCE    (1 << 3)
		#define CHANNEL_LEFT_SURROUND       (1 << 4)
		#define CHANNEL_RIGHT_SURROUND      (1 << 5)
		#define CHANNEL_LEFT_OF_CENTER      (1 << 6)
		#define CHANNEL_RIGHT_OF_CENTER     (1 << 7)
		#define CHANNEL_SURROUND            (1 << 8)
		#define CHANNEL_SIDE_LEFT           (1 << 9)
		#define CHANNEL_SIDE_RIGHT          (1 << 10)
		#define CHANNEL_TOP                 (1 << 11)

		#define FEATURE_MUTE                (1 << 0)
		#define FEATURE_VOLUME              (1 << 1)
		#define FEATURE_BASS                (1 << 2)
		#define FEATURE_MID                 (1 << 3)
		#define FEATURE_TREBLE              (1 << 4)
		#define FEATURE_GRAPHIC_EQUALIZER   (1 << 5)
		#define FEATURE_AUTOMATIC_GAIN      (1 << 6)
		#define FEATURE_DELAY               (1 << 7)
		#define FEATURE_BASS_BOOST          (1 << 8)
		#define FEATURE_BASS_LOUDNESS       (1 << 9)

		#define AUDIO_STREAM_EPNUM          0x01
		#define AUDIO_STREAM_EPSIZE         256
		
	/* Type Defines: */
		typedef struct
		{
			USB_Descriptor_Header_t   Header;
			uint8_t                   Subtype;

			uint16_t                  ACSpecification;
			uint16_t                  TotalLength;
			
			uint8_t                   InCollection;
			uint8_t                   InterfaceNumbers[1];			
		} USB_AudioInterface_AC_t;
		
		typedef struct
		{
			USB_Descriptor_Header_t   Header;
			uint8_t                   Subtype;
			
			uint8_t                   UnitID;
			uint8_t                   SourceID;
			
			uint8_t                   ControlSize;
			uint8_t                   ChannelControls[3];
			
			uint8_t                   FeatureUnitStrIndex;  
		} USB_AudioFeatureUnit_t;

		typedef struct
		{
			USB_Descriptor_Header_t   Header;
			uint8_t                   Subtype;
		
			uint8_t                   TerminalID;
			uint16_t                  TerminalType;
			uint8_t                   AssociatedOutputTerminal;
			
			uint8_t                   TotalChannels;
			uint16_t                  ChannelConfig;
			
			uint8_t                   ChannelStrIndex;
			uint8_t                   TerminalStrIndex;
		} USB_AudioInputTerminal_t;

		typedef struct
		{
			USB_Descriptor_Header_t   Header;
			uint8_t                   Subtype;
		
			uint8_t                   TerminalID;
			uint16_t                  TerminalType;
			uint8_t                   AssociatedInputTerminal;
			
			uint8_t                   SourceID;
			
			uint8_t                   TerminalStrIndex;
		} USB_AudioOutputTerminal_t;
		
		typedef struct
		{
			USB_Descriptor_Header_t   Header;
			uint8_t                   Subtype;
			
			uint8_t                   TerminalLink;
			
			uint8_t                   FrameDelay;
			uint16_t                  AudioFormat;
		} USB_AudioInterface_AS_t;
		
		typedef struct
		{
			USB_Descriptor_Header_t   Header;
			uint8_t                   Subtype;

			uint8_t                   FormatType;
			uint8_t                   Channels;
			
			uint8_t                   SubFrameSize;
			uint8_t                   BitResolution;
			uint8_t                   SampleFrequencyType;
			
			uint16_t                  SampleFreq_LS;
			uint8_t                   SampleFreq_MS;
		} USB_AudioFormat_t;
		
		typedef struct
		{
			USB_Descriptor_Endpoint_t Endpoint;

			uint8_t                   Refresh;
			uint8_t                   SyncEndpointNumber;
		} USB_AudioStreamEndpoint_Std_t;
					
		typedef struct
		{
			USB_Descriptor_Header_t   Header;
			uint8_t                   Subtype;
			
			uint8_t                   Attributes;

			uint8_t                   LockDelayUnits;
			uint16_t                  LockDelay;
		} USB_AudioStreamEndpoint_Spc_t;	

		typedef struct
		{
			USB_Descriptor_Configuration_Header_t Config;
			USB_Descriptor_Interface_t            AudioControlInterface;
			USB_AudioInterface_AC_t               AudioControlInterface_SPC;
			USB_AudioInputTerminal_t              InputTerminal;
			USB_AudioFeatureUnit_t                FeatureUnit;
			USB_AudioOutputTerminal_t             OutputTerminal;
			USB_Descriptor_Interface_t            AudioStreamInterface_Alt0;
			USB_Descriptor_Interface_t            AudioStreamInterface_Alt1;
			USB_AudioInterface_AS_t               AudioStreamInterface_SPC;
			USB_AudioFormat_t                     AudioFormat;
			USB_AudioStreamEndpoint_Std_t         AudioEndpoint;
			USB_AudioStreamEndpoint_Spc_t         AudioEndpoint_SPC;
		} USB_Descriptor_Configuration_t;
		
	/* Function Prototypes: */
		bool USB_GetDescriptor(const uint8_t Type, const uint8_t Index,
							   void** const DescriptorAddr, uint16_t* const Size)
							   ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3, 4);

#endif
