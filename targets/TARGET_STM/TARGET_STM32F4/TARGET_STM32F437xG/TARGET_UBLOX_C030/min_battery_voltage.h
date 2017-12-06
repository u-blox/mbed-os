#ifndef MIN_BATTERY_VOLTAGE_H
#define MIN_BATTERY_VOLTAGE_H

#ifdef __cplusplus
extern"C"{
#endif

#define BQ24295_I2C_ADDRESS (0x6B << 1)
#define MIN_BATTERY_VOLTAGE_MASK (0x87)

/** Initializes an instance of class BatteryChargerI2c which is using the STM HAL I2C APIs
* to interface with the battery charger BQ24295. This sets the Input Control Source Register
* of the Battery Charger chip to its minimum voltage level
*/
void set_minimum_battery_voltage(void);

#ifdef __cplusplus
}
#endif

#endif	// MIN_BATTERY_VOLTAGE_H
