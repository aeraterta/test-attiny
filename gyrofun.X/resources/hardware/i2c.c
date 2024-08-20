
#include "i2c.h"

/****************************************************************/
static twi0_operations_t wr1RegCompleteHandler_example(void *ptr)
{
    I2C0_SetBuffer(ptr,1);
    I2C0_SetDataCompleteCallback(NULL,NULL);
    return I2C0_CONTINUE;
}

/****************************************************************/
static twi0_operations_t rd1RegCompleteHandler_example(void *ptr)
{
    I2C0_SetBuffer(ptr,1);
    I2C0_SetDataCompleteCallback(NULL,NULL);
    return I2C0_RESTART_READ;
}

uint8_t I2C0_readByteRegister(twi0_address_t address, uint8_t reg)
{
    uint8_t    d2=42;
    twi0_error_t e;
    int x;

    for(x = 2; x != 0; x--)
    {
        while(!I2C0_Open(address)); // sit here until we get the bus..
        I2C0_SetDataCompleteCallback(rd1RegCompleteHandler_example,&d2);
        I2C0_SetBuffer(&reg,1);
        I2C0_SetAddressNackCallback(I2C0_SetRestartWriteCallback,NULL); //NACK polling?
        I2C0_MasterWrite();
        while(I2C0_BUSY == (e = I2C0_Close())); // sit here until finished.
        if(e==I2C0_NOERR) break;
    }
    

    return d2;
}

int I2C0_writeByteRegister(twi0_address_t address, uint8_t reg, uint8_t data)
{
    twi0_error_t e;
    while(!I2C0_Open(address)); // sit here until we get the bus..
    I2C0_SetDataCompleteCallback(wr1RegCompleteHandler_example,&data);
    I2C0_SetBuffer(&reg,1);
    I2C0_SetAddressNackCallback(I2C0_SetRestartWriteCallback,NULL); //NACK polling?
    I2C0_MasterWrite();
    while(I2C0_BUSY == (e = I2C0_Close())); // sit here until finished.
    
    return e;
}