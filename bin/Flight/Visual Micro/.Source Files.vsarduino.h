/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Due (Programming Port), Platform=sam, Package=arduino
*/

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define __SAM3X8E__
#define USB_VID 0x2341
#define USB_PID 0x003e
#define USBCON
#define USB_MANUFACTURER "\"Unknown\""
#define USB_PRODUCT "\"Arduino Due\""
#define ARDUINO 156
#define ARDUINO_MAIN
#define printf iprintf
#define __SAM__
#define __sam__
#define F_CPU 84000000L
#define __cplusplus
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define __ICCARM__
#define __ASM
#define __INLINE
#define __GNUC__ 0
#define __ICCARM__
#define __ARMCC_VERSION 400678
#define __attribute__(noinline)

#define prog_void
#define PGM_VOID_P int
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}




//
//
void set_defaults();
void set_output_pins();
void set_load_shed_mod();
void process_camera_function();
void get_gps_data();
static void smartDelay(unsigned long ms);
static void printdouble(double val, bool valid, int len, int prec);
static void printInt(unsigned long val, bool valid, int len);
static void printDateTime(TinyGPSDate &d, TinyGPSTime &t);
static void printStr(const char *str, int len);
void mpuInit();
boolean duePoll();
void gyro_setup();
void get_gyro_data();
void magCalStart(void);
void magCalLoop();
void accelCalStart(void);
void accelCalLoop();
void process_satellite_data();
int read_satellite_data();
void format_telemetry_data_for_sd(struct telemetry_data_struct* telemetry_data, char* string_buffer);
void write_telemetry_data_to_sd(char* string_buffer);
void format_satellite_data_for_sd(struct satellite_data_struct* sattelite_data, char* string_buffer);
void write_satallite_data_to_sd(char* string_buffer);
void colect_telemetry();
void process_telemetry();
void print_telemetry();

#include "C:\Program Files (x86)\Arduino\hardware\arduino\sam\cores\arduino\arduino.h"
#include "C:\Program Files (x86)\Arduino\hardware\arduino\sam\variants\arduino_due_x\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\sam\variants\arduino_due_x\variant.h" 
#include "c:\Users\751696\Documents\GitHub\Flight1\bin\Flight\Source Files\Flight.ino"
#include "c:\Users\751696\Documents\GitHub\Flight1\bin\Flight\Source Files\camera_module.ino"
#include "c:\Users\751696\Documents\GitHub\Flight1\bin\Flight\Source Files\definitions.h"
#include "c:\Users\751696\Documents\GitHub\Flight1\bin\Flight\Source Files\gps_module.ino"
#include "c:\Users\751696\Documents\GitHub\Flight1\bin\Flight\Source Files\gyro_module.ino"
#include "c:\Users\751696\Documents\GitHub\Flight1\bin\Flight\Source Files\rockblock_module.ino"
#include "c:\Users\751696\Documents\GitHub\Flight1\bin\Flight\Source Files\sd_module.ino"
#include "c:\Users\751696\Documents\GitHub\Flight1\bin\Flight\Source Files\telemetry_module.ino"
#endif
