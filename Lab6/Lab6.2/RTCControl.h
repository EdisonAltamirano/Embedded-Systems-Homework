/*
 * RTCControl.h
 *
 * Created: 4/28/2022 12:18:15 AM
 *  Author: riky_
 */ 


#ifndef RTCCONTROL_H_
#define RTCCONTROL_H_

#define SEG 0x00
typedef union {
	struct {
		uint8_t un_seg :4,
		dec_seg :3,
		RESERVED :1;
	} field;
	uint8_t reg;
} seg_type;

#define MIN 0x01
typedef union {
	struct {
		uint8_t un_min :4,
		dec_min :3,
		RESERVED :1;
	} field;
	uint8_t reg;
} min_type;

#define HOUR 0x02
typedef union {
	struct {
		uint8_t un_hour :4,
		dec_hour :1,
		am_pm_hour :1,
		format_hour: 1,
		RESERVED :1;
	} field;
	uint8_t reg;
} hour_type;

#define DATE 0x04
typedef union {
	struct {
		uint8_t un_date :4,
		dec_date :2,
		RESERVED :2;
	} field;
	uint8_t reg;
} date_type;

#define MONTH 0x05
typedef union {
	struct {
		uint8_t un_month :4,
		dec_month :1,
		RESERVED :3;
	} field;
	uint8_t reg;
} month_type;

#define YEAR 0x06
typedef union {
	struct {
		uint8_t un_year :4,
		dec_year :4;
	} field;
	uint8_t reg;
} year_type;




#endif /* RTCCONTROL_H_ */