#ifndef __DATACALIB_H__
#define __DATACALIB_H__
#include "main.h"

#define PWR_CAL_BASE 0xF000
#define BAT_CAL_BASE 0xF200

typedef struct PowerCalibrationTables {
    PowerCalibration_t high;
    PowerCalibration_t med;
    PowerCalibration_t low;
} PowerCalibrationTables_t;

typedef struct BatteryCalibrationTables_t
{
    uint16_t BAT_Prohibited;
    uint16_t BAT_Empty;
    uint16_t BAT_Level1;
    uint16_t BAT_Level2;
    uint16_t BAT_Level3;
    uint16_t BAT_Full;
}BatteryCalibrationTables_t;


extern PowerCalibrationTables_t PwrCalData;
extern BatteryCalibrationTables_t BatCalData;



void nvm_readCalibData(void);

static void nvm_read_Tx_Pwr_CalibData(void *buf);

static void nvm_read_BAT_CalibData(void *buf);

#endif