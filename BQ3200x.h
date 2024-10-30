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
 * 
 *  Features: Real-Time Clock/Calendar (RTCC):- Hours, minutes, seconds, 
 *   day of week, date, month, year
 * Leap year compensated.
 * 24-hour mode
 * Oscillator for 32.768 kHz Crystals:- Nominal 12 pF load capacitance
 * On-Chip Digital Trimming/Calibration:
 *   ±2/4 ppm resolution
 *   -63 to +126 ppm range
 * 
 * IIC to 400 kHz
 * 
 */

#ifndef BQ3200x_H
#define	BQ3200x_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define RTC_IIC_Address  0xD0
    
// RTCC Register map
#define RTC_REG_SECONDS     0x00
// D7 STOP oscillator
// D6:D4 BCD tens of secs, 0:5
// D3:D0 BCD ones of secs, 0:9
    
#define RTC_REG_MINUTES     0x01
// D7 OF Oscillator Fail flag
// D6:D4 BCD tens of mins, 0:5
// D3:D0 BCD ones of mins, 0:9    
    
#define RTC_REG_CENT_HOURS  0x02
// D7 CENT_EN Century Enable bit.
// D6 CENT Century when CENT_EN ena
// D5:D4 BCD tens of hrs, 0:2   Hours are 24-hour
// D3:D0 BCD ones of hrs, 0:9   
    
#define RTC_REG_DAY         0x03
// D2:D0 BCD Day of week, 1 (Sun):7 (Sat)
    
#define RTC_REG_DATE        0x04  
// D7:D6 Reserved
// D5:D4 BCD tens of date, 0:3   Date (of month) is 01:31
// D3:D0 BCD ones of date, 0:9      
    
#define RTC_REG_MONTH       0x05
// D7:D5 Reserved
// D4 BCD tens of month, 0:1   Month is 01:12
// D3:D0 BCD ones of month, 0:9  
    
#define RTC_REG_YEARS       0x06
// D7:D4 BCD tens of year, 0:9   
// D3:D0 BCD ones of year, 0:9      
    
#define RTC_REG_CAL_CFG1    0x07
// D7 OUT when FT = 0, /IRQ pin reflects OUT
// D6 FT Freq Test, if FT = 1 sq wave emit on /IRQ, freq depends on SFR reg
    // Default is 512 Hz
// D5 S Calibr Sign, S = 0 slows RTC, S = 1 speeds up.
// D4:D0 CAL, if S = 0, CAL = 31 -> +63 ppm; if S = 1, CAL = 31 -> -126 ppm; 
    
#define RTC_REG_CFG2        0x09
// All reserved
    
// --------- Prototypes --------------------
    
// Read h:m:s and print
void rtcReadTest(void);  

// Write hour, minute, second based on Timers RTC struct. Presumably that has just 
// been written with accurate values.
void rtcTimeStructWr(void);

// Start oscill, set 24 hr mode, ena battery.  We will set time from fw RTC later.
SYS_RtnCode rtcInit(void);

SYS_RtnCode rtcTimeRead(uint8_t* hour, uint8_t* minute, uint8_t* second);


#ifdef	__cplusplus
}
#endif

#endif	/* BQ3200x_H */

