#include "DataCalib.h"

PowerCalibrationTables_t PwrCalData;
BatteryCalibrationTables_t BatCalData;


static const uint32_t baseAddress = 0x000A1000;     // 0x000A1000;

void nvm_readCalibData(void)
{
    nvm_read_Tx_Pwr_CalibData(&PwrCalData);
    nvm_read_BAT_CalibData(&BatCalData);
}


/**
 * @brief Read TX_Power from SPI-flash
 * 
 * @param buf PowerCalibrationTables_t
 */
static void nvm_read_Tx_Pwr_CalibData(void *buf)
{
    // W25Qx_wakeup();
    // delayUs(5);

    PowerCalibrationTables_t *calib = ((PowerCalibrationTables_t *) buf);

    // Load calibration data
    uint32_t addr = PWR_CAL_BASE;

    uint8_t *ptr = (uint8_t*)calib;  


    for(int i = 0;i < sizeof(PowerCalibration_t); i++)
    {
        w25q16jv_read_num(addr + i, ptr + i, 1);
    }
    addr += 64;
    ptr += sizeof(PowerCalibration_t);


    for(int i = 0;i < sizeof(PowerCalibration_t); i++)
    {
        w25q16jv_read_num(addr + i, ptr + i, 1);
    }
    addr += 64;
    ptr += sizeof(PowerCalibration_t);


    for(int i = 0;i < sizeof(PowerCalibration_t); i++)
    {
        w25q16jv_read_num(addr + i, ptr + i, 1);
    }

}


/**
 * @brief Read Battery calibration data from SPI-flash
 * 
 * @param buf BatteryCalibrationTables_t
 */
static void nvm_read_BAT_CalibData(void *buf)
{
    BatteryCalibrationTables_t *calib = ((BatteryCalibrationTables_t *) buf);
    uint32_t addr = BAT_CAL_BASE;
    uint8_t data; 
    uint16_t *ptr = (uint16_t*)calib;  

    for(int i = 0;i < sizeof(BatteryCalibrationTables_t); i++)
    {
        w25q16jv_read_num(addr + i, &data, 1);
        *ptr = (uint16_t)data * 16;
        ptr += sizeof(uint8_t);
    }

    // printf("BatCalData:\n%d\n%d\n%d\n%d\n%d\n%d\n", BatCalData.BAT_Prohibited, BatCalData.BAT_Empty, BatCalData.BAT_Level1, BatCalData.BAT_Level2, BatCalData.BAT_Level3, BatCalData.BAT_Full);
}


