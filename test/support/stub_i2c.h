#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdio.h>
#include "stub_twi0_master.h"

uint8_t I2C0_readByteRegister(twi0_address_t address, uint8_t reg);
int I2C0_writeByteRegister(twi0_address_t address, uint8_t reg, uint8_t data);

#endif /* I2C_H */