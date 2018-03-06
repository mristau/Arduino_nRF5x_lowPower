/**************************************************************************/
/*!
    @file       Arduino_nRF5x_lowPower.cpp
    @author     Marc-Oliver Ristau <marc.ristau@mristau.eu>
    @version    0.1.1
	
    Arduino library for nRF5x lowPower modes
	
    @section    HISTORY
	
    v0.1.0      - First Release
    v0.1.1      - updated comments
*/
/**************************************************************************/

#include <nrf_sdm.h>
#include <nrf_soc.h>

#include "Arduino_nRF5x_lowPower.h"

/**************************************************************************/
/*!
    @brief Sends nRF5x to desired powermode
*/
/**************************************************************************/
void Arduino_nRF5x_lowPower::powerMode(nRF5x_powermodes_t mode) {
    switch(mode) {
        case POWER_MODE_OFF:
            powerOff();
            break;
        case POWER_MODE_LOW_POWER:
            lowPower();
            break;
        case POWER_MODE_CONSTANT_LATENCY:
            constLat();
            break;
    }
}

/**************************************************************************/
/*!
    @brief enables the builtin DCDC Converter
*/
/**************************************************************************/
void Arduino_nRF5x_lowPower::enableDCDC() {
    // NRF_POWER_DCDC_ENABLE            /**< The DCDC is enabled.  */
    if (checkForSoftDevice() == 1) {
        sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
    } else {
        NRF_POWER->DCDCEN = 1;
    }
}

/**************************************************************************/
/*!
    @brief disables the builtin DCDC Converter
*/
/**************************************************************************/
void Arduino_nRF5x_lowPower::disableDCDC() {
    // NRF_POWER_DCDC_DISABLE,          /**< The DCDC is disabled. */
    if (checkForSoftDevice() == 1) {
        sd_power_dcdc_mode_set(NRF_POWER_DCDC_DISABLE);
    } else {
        NRF_POWER->DCDCEN = 0;
    }
}

/**************************************************************************/
/*!
    @brief enable Sense mode on selected pin, so it can wake the nRF5x from power off
*/
/**************************************************************************/
void Arduino_nRF5x_lowPower::enableWakeupByInterrupt(uint32_t pin, uint32_t mode) {
    NRF_GPIO->PIN_CNF[pin] &= ~((uint32_t)GPIO_PIN_CNF_SENSE_Msk);
    switch (mode) {
        case HIGH:
        case RISING:
            NRF_GPIO->PIN_CNF[pin] |= ((uint32_t)GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos);
            break;
        case LOW:
        case FALLING:
            NRF_GPIO->PIN_CNF[pin] |= ((uint32_t)GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos);
            break;
        default:
            return;
    }
}

/**************************************************************************/
/*!
    @brief disable Sense mode on selected pin
*/
/**************************************************************************/
void Arduino_nRF5x_lowPower::disableWakeupByInterrupt(uint32_t pin) {
    NRF_GPIO->PIN_CNF[pin] &= ~((uint32_t)GPIO_PIN_CNF_SENSE_Msk);
    NRF_GPIO->PIN_CNF[pin] |= ((uint32_t)GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
}

/**************************************************************************/
/*!
    @brief Sends nRF5x to power off mode
*/
/**************************************************************************/
void Arduino_nRF5x_lowPower::powerOff(void) {
    if (checkForSoftDevice() == 1) {
        // SoftDevice enabled
        sd_power_system_off();
    } else {
        // No SoftDevice
        NRF_POWER->SYSTEMOFF = 1;
    }
}

/**************************************************************************/
/*!
    @brief Sends nRF5x to constant latency mode
*/
/**************************************************************************/
void Arduino_nRF5x_lowPower::constLat(void) {
    //NRF_POWER_MODE_CONSTLAT,  /**< Constant latency mode. See power management in the reference manual. */
    if (checkForSoftDevice() == 1) {
        // SoftDevice enabled
        sd_power_mode_set(NRF_POWER_MODE_CONSTLAT);
    } else {
        // No SoftDevice
        NRF_POWER->TASKS_CONSTLAT = 1;
    }
}

/**************************************************************************/
/*!
    @brief Sends nRF5x to low power mode
*/
/**************************************************************************/
void Arduino_nRF5x_lowPower::lowPower(void) {
    //NRF_POWER_MODE_LOWPWR     /**< Low power mode. See power management in the reference manual. */
    if (checkForSoftDevice() == 1) {
        // SoftDevice enabled
        sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
    } else {
        // No SoftDevice
        NRF_POWER->TASKS_LOWPWR = 1;
    }
}

/**************************************************************************/
/*!
    @brief Checks if a softdevice is present
    @return 1=softdevice, 0=no softdevice
*/
/**************************************************************************/
uint8_t Arduino_nRF5x_lowPower::checkForSoftDevice(void) {
    uint8_t check;
    sd_softdevice_is_enabled(&check);

    return check;
}

Arduino_nRF5x_lowPower nRF5x_lowPower;