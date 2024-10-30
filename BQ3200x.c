/*
 * @file    BQ3200x.c
 * @brief   TI battery backed RTC with IIC interface
 * @version 0.1
 * @ref     BlueJay PIC32MZ MultiBoard
 * @BLUJ    Platform = PIC32MZ2048EFM100
 * @author  Prabir Roy
 * Company: Black Oak Engineering
 * Created: 16Jun23
 * @license License_BOE_MIT.txt included herewith
 * All information herein derived from TO 2016 datasheet.
 */

#include "BlueJay.h"  
#include "IIC.h"
#include "BQ3200x.h"
#include "Serial.h"
#include "Timers.h"     // For debug

// Read h:m:s and print
void rtcReadTest(void)  
{
    uint8_t hour, minute, second;
    if (rtcTimeRead(&hour, &minute, &second) != RTN_Executed) putStr("\tRTC read fail\r\n");
    else 
    { 
        sprintf(ioBfr, "\tRTC: %02d:%02d:%02d\r\n", hour, minute, second); 
        putStr(ioBfr); 
    }
}

// Write hour, minute, second based on Timers RTC struct. Presumably that has just 
// been written with accurate values. See BQ3200x.h for exegesis.
void rtcTimeStructWr(void)  
{
    uint8_t pkg, subpkg;
    iicStart();
    iicWrite(RTC_IIC_Address & 0xFE);   // Enforce write mode
    iicAwaitAck();
    iicWrite(RTC_REG_SECONDS);  // Will autoincr
    iicAwaitAck();
    
    pkg = 0x00;     // D7 is STOP osc
    subpkg = rtc.second / 10;
    pkg += (subpkg << 6);
    pkg += rtc.second % 10;
    iicWrite(pkg);   
    iicAwaitAck();
    
    pkg = 0x00;     // D7 is OF flag
    subpkg = rtc.minute / 10;
    pkg += (subpkg << 6);
    pkg += rtc.minute % 10;
    iicWrite(pkg);   
    iicAwaitAck();

    pkg = 0x00;    
    subpkg = rtc.hour / 10;
    pkg += (subpkg << 5);
    pkg += rtc.hour % 10;
    iicWrite(pkg);   
    iicAwaitAck();
    iicStop();     
}

// Start oscill, set 24 hr mode, ena battery.  We will set time from fw RTC later.
SYS_RtnCode rtcInit(void) 
{
    SYS_RtnCode retVal = RTN_Executed;
    iicStart();
    iicWrite(RTC_IIC_Address & 0xFE);   // Enforce write mode
    iicAwaitAck();
    iicWrite(RTC_REG_SECONDS); 
    iicAwaitAck();
    iicWrite(0x00);     // D7 is STOP osc
    iicAwaitAck();
    iicStop();
    return retVal;
}


SYS_RtnCode rtcTimeRead(uint8_t* hour, uint8_t* minute, uint8_t* second)
{   
    SYS_RtnCode retVal = RTN_Executed;
    uint8_t pkg;
    
    iicStart();
    iicWrite(RTC_IIC_Address & 0xFE);   // Enforce write mode
    iicAwaitAck();
    iicWrite(RTC_REG_SECONDS); 
    iicAwaitAck();
    iicStart();
    iicWrite(RTC_IIC_Address | 0x01);     // Read mode
    iicAwaitAck();
    pkg = iicRead();
    *second = 10 * (pkg >> 6);
    *second += 0x0F & pkg;
    iicSendAck();
    
    pkg = iicRead();
    *minute = 10 * (pkg >> 6);
    *minute += 0x0F & pkg;
    iicSendAck();
    
    pkg = iicRead();
    *hour = 10 * (pkg >> 5);
    *hour += 0x0F & pkg;
    iicSendNack();
    iicStop();
    return retVal;
}



