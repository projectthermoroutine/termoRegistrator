//*****************************************************************************
// Interface for IR-Data-acquisition
// Version 3.0
// InfraTec GmbH
// 26.10.2007
//*****************************************************************************

#pragma once
#define IRBDLL_NO_ERROR			 1
#define IRBDLL_CHANGED_PARAMS	 2
#define IRBDLL_RET_ERROR        -1
#define IRBDLL_NO_IMAGE         -2
#define IRBDLL_NO_IMAGE_CHANGED_PARAMS  -3
#define IRBDLL_NODLL_XCAM       -4
#define IRBDLL_NODLL_VIRTGRAB   -5
#define IRBDLL_ERROR_TIMEOUT    -6
#define IRBDLL_ERROR_NOCAMERA   -7
#define IRBDLL_NOT_SUPPORTED    -8

#define IRBDLL_TRIGGED          1
#define IRBDLL_TRIGGED_NONE     0

#define IRBDLLTRIG_NONE             0
#define IRBDLLTRIG_MARKERSYNC       1
#define IRBDLLTRIG_MARKED           2
#define IRBDLLTRIG_EXTERNALSYNC     4
#define IRBDLLTRIG_SYNCOUT          8

#define IRBDLL_HEADER_NONE          0
#define IRBDLL_HEADER_IRB100        1
#define IRBDLL_HEADER_IRB101        2
#define IRBDLL_HEADER_BMP           3

#define DATATYPE_NONE               0
#define DATATYPE_BITMAP             1    // complette Bitmap inclusive Header (54 Byte)
#define DATATYPE_BITMAP32DATA       2    // Bitmap Pixel Data (32Bit Farbtiefe) without Header
#define DATATYPE_IRBFRAME           3    // Header (1728 Byte) + IRB-Data (Digital Data - one Word per Pixel)
#define DATATYPE_IRBDATA            4    // IRB-Data (Digital-Data - one Word per Pixel) without Header
#define DATATYPE_TEMPERATURES       5    // Temperature-Data (Single/Float-Values)
#define DATATYPE_8BITDATA           6    // 8Bit-Data
#define DATATYPE_TEMPERATUREFRAME   7    // IRBHeader (1728 Byte) + Temperature-Data (Single-Werte)

#define GRABMODE_LATEST               -2
#define GRABMODE_INORDER              -3   // Ring-Buffer-Mode
#define GRABMODE_INORDERWAITFORGRAB   -10

#define ROTATEFLIP_FLIPH            1
#define ROTATEFLIP_FLIPV            2
#define ROTATEFLIP_ROTATE           4

#define DATAMODE_NONE               0
#define DATAMODE_ONLINE16           1
#define DATAMODE_SCREEN8            2
#define DATAMODE_SCREEN16           3
#define DATAMODE_SCREEN24           4
#define DATAMODE_SCREEN32           5
#define DATAMODE_RESENH             6
#define DATAMODE_VISUAL             7

#define EVENTNUCACTIVE              100  // Wenn NUC von inaktiv zu aktiv wechselt
#define EVENTNUCINATIVE             101  // Wenn NUC von activ zu inaktiv wechselt

//Parameter
#define DLLPARAM_FramesInBuf      9    // Count of Frames in the Buffer/Ringbuffer
#define DLLPARAM_FrameRate        10   // Framerate of the Camera
#define DLLPARAM_IntegTime        11   // IntegrationIdx/Channel * 10000  + IntegrationTime
#define DLLPARAM_Emiss            20   // Emmissivity
#define DLLPARAM_EnvTemp          21   // Enviromenttemperature
#define DLLPARAM_Tau              22   // Transmission
#define DLLPARAM_PathTemp         23   // Pathtemperature
#define DLLPARAM_CalibMin         26   // Lower Bound of the Calibration
#define DLLPARAM_CalibMax         27   // Upper Bound of the Calibartion
#define DLLPARAM_Level            28   // Temperaturelevel from the Camera
#define DLLPARAM_Range            29   // Temperaturerange from the Kamera
#define DLLPARAM_Distance         30   // Distance of the Camera
#define DLLPARAM_Autofocus        31   // perform an autofocus
#define DLLPARAM_FocSteps         32   // Position of Focusmotor (VarioCAM)
#define DLLPARAM_Smooth           33   // strange from the Filter
#define DLLPARAM_DateTime         34   // DateTimea form the Camera
#define DLLPARAM_CamTemp          35   // Cameratemperature
#define DLLPARAM_ShutterTemp      36   // Shuttertemperature
#define DLLPARAM_NucIntv          37   // Shutterinterval
#define DLLPARAM_PerfNUC          38   // perform a Shutter
#define DLLPARAM_Palette          39   // get/set Palette-Index
#define DLLPARAM_TrigExt          40   // Trigger, Value =  (Channal (start at 0) shl 16) or IRBDLLTRIG_xxx
#define DLLPARAM_Zoom             41   // used Zoom in the Camera
#define DLLPARAM_CamSN            45   // Serial number of the Camera
#define DLLPARAM_MaxFramesInBuf   70   // maximum frame Count in the Buffer/Ringbuffer (the size of the Buffer/RingBuffer)
#define DLLPARAM_ShowOnlyTriggedFrames    71 // shows only trigged Frames in the Window from ShowWindow
#define DLLPARAM_ProcessOnlyTriggedFrames 72 // calculate only trigged frames (DLLPARAM_ShowOnlyTriggedFrames is implizit)
#define DLLPARAM_RotateFlip       73   // Rotate and/or Flip the Frame
#define DLLPARAM_TempFrom         74   // Set/Get the lower Temperature in the viewrange (Software)
#define DLLPARAM_TempTill         75   // Set/get the upper Temperature in the viewrange (Software)
#define DLLPARAM_ObjectTemp       76   // Set/Reset/Get Objecttemperature
#define DLLPARAM_DataMode         77   // Set/Get the DataMode

#pragma pack(push,1)
typedef struct
{
	int SrcID;
	int DataType;
	void* Buf;
	int   Bufsize;
	int   Options;
	int   Timeout;
	int    GrabMode;
	int    Reserved;
} TGrabInfoIn, *PGrabInfoIn;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	int Width;
	int Height;

	int SrcID;
	int DataType;
	void* Buf;
	int Bufsize;

	int ImgCounter;
	int Headsize;
	int Headversion;
	int Trigger;
	int CombinedCnt;

	//irb-Sezifisch
	float IRBmin;
	float IRBavg;
	float IRBmax;
	float IRBstddev;

	double Timestamp;
	_SYSTEMTIME Systime;   //16Byte

	int Reserved[11];
}TGrabInfoOut, *PGrabInfoOut;
#pragma pack(pop)


#ifndef __BCPLUSPLUS__
  typedef unsigned __int32	THandle; 
  typedef float				Tfloat;
#else
  typedef Single			Tfloat;
#endif
  typedef unsigned char		BYTE;//-V677
  typedef __int32			I32 ;
  typedef unsigned __int32	UI32 ;
  
  
  typedef int ( __stdcall *Tirbg_GetSources)( char* pCharBuffer, UI32* SrcCnt) ;
  typedef int ( __stdcall *Tirbg_InitSource)( const UI32 SrcID) ;
  typedef int ( __stdcall *Tirbg_CloseSource)( const UI32 SrcID) ;
  
  typedef int ( __stdcall *Tirbg_AcqInterval_uSecs)( const UI32 uSecs) ;
  // max length of answer from SendCommand  
#define MaxAnswerLength 100
  
  typedef int ( __stdcall *Tirbg_SendCommand)( const UI32 SrcID, const char* cmd, char*  answer, const UI32 timeout ) ;    
  typedef int ( __stdcall *Tirbg_ShowWindow)( const UI32 SrcID, const UI32 Mode ) ;
  typedef int ( __stdcall *Tirbg_GrabIRBLUT)( const UI32 SrcID, void* pbuf ) ;
  typedef int ( __stdcall *Tirbg_RegisterWndMsgNewPict)( const UI32 SrcID, const THandle WndHandle, const DWORD NewPictMsg) ;
  typedef int ( __stdcall *Tirbg_Grab)( TGrabInfoIn& FrameInfoIn, TGrabInfoOut& FrameInfoOut ) ;


  typedef struct irbgrabDllFunctions
  {
	Tirbg_GetSources				GetSources ;
	Tirbg_InitSource				InitSource ;
	Tirbg_CloseSource				CloseSource ;
	Tirbg_Grab						Grab ;
	Tirbg_AcqInterval_uSecs			AcqInterval_uSecs ;
	Tirbg_SendCommand				SendCommand ;
	Tirbg_ShowWindow				ShowWindow ;
	Tirbg_GrabIRBLUT				GrabIRBLUT ;
	Tirbg_RegisterWndMsgNewPict		RegisterWndMsgNewPict ;

  }	TirbgrabFunctions ;










