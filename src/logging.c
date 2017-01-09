#include "logging.h"
#include "TM_lib/tm_stm32_rtc.h"
#include "fatfs.h"
#include "init.h"

extern currentFileName[];

void openLogFile() {
	//getting timestamp for file
	TM_RTC_t timeBuffer;
	TM_RTC_GetDateTime(&timeBuffer,TM_RTC_Format_BIN);

	sprintf(currentFileName,"%d-%d-%d.txt\n",timeBuffer.Year,timeBuffer.Month,timeBuffer.Day);

	 if (f_open(&log1, currentFileName, FA_OPEN_EXISTING |FA_READ | FA_WRITE) == FR_OK) {
			//try to open existing file
		  	 //however we need to append to it!
		  	f_lseek(&log1, f_size(&log1));
			f_sync(&log1);
		  } else
		  	 //has to create file
		  	if ( initStatus!=ERROR_RTC_NOT_SET) {
		  		if (f_open(&log1, currentFileName, FA_CREATE_ALWAYS|FA_READ | FA_WRITE) == FR_OK) {
		  				//add header
		  				f_printf(&log1,"File creation at: %u-%u-%u %u:%u:%u\n",timeBuffer.Year,timeBuffer.Month,timeBuffer.Day,timeBuffer.Hours,timeBuffer.Minutes,timeBuffer.Seconds);
		  				f_puts(MANTIS_MEAS_HEADER,&log1);
		  				f_sync(&log1);
		  		}
			 } else { //ERROR_RTC_NOT_SET
				 f_open(&log1, "default.txt", FA_CREATE_ALWAYS|FA_READ | FA_WRITE);
			 }
}

void openDebugFile() {
	 if (f_open(&log_debug, "debug.txt", FA_OPEN_EXISTING |FA_READ | FA_WRITE) == FR_OK) {
		 //try to open existing file
		 //however we need to append to it!
		 f_lseek(&log_debug, f_size(&log1));
		 f_sync(&log_debug);
	 } else {
		 TM_RTC_t timeBuffer;
		 TM_RTC_GetDateTime(&timeBuffer,TM_RTC_Format_BIN);

		 if (f_open(&log_debug, "debug.txt", FA_CREATE_ALWAYS|FA_READ | FA_WRITE) == FR_OK) {
			 //add header
			 f_printf(&log1,"File creation at: %u-%u-%u %u:%u:%u\n",timeBuffer.Year,timeBuffer.Month,timeBuffer.Day,timeBuffer.Hours,timeBuffer.Minutes,timeBuffer.Seconds);
			 f_sync(&log_debug);
		 }
	 }
}

void checkLogging() {
	//getting current date and time
	TM_RTC_t timeBuffer;
	TM_RTC_GetDateTime(&timeBuffer,TM_RTC_Format_BIN);

	//creating actual fileName
	char rightName[100];
	sprintf(rightName,"%d-%d-%d.txt\n",timeBuffer.Year,timeBuffer.Month,timeBuffer.Day);

	//if they are not equal, need to create new file
	if (initStatus!=ERROR_RTC_NOT_SET&&strcmp(rightName,currentFileName)) {
		//close old file
		f_close(&log1);
		openLogFile(timeBuffer);
	}
}

void writeLogEntry (FIL* fil, int type) {
	//format: TIMESTAMP;VIBRATION_AVG;MAX_ACC;MAX_GYRO
	//specified in init.c: initSD();


	//snprintf(buffer,200,"%u:%u:%u;%d;%d;%d",datetime.Hours,datetime.Minutes,datetime.Seconds, IMU_data->accel[0], IMU_data->accel[1], (int)vibration_data);
	//TM_USART_Puts(USART3, buffer);
	//f_printf(fil,"%s\n",buffer);

	//needs to actually write to SD card
	f_sync(fil);
}
