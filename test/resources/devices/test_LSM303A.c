#include "unity.h"
#include "cmock.h"
#include <stdint.h>
#include <stdio.h>
#include "stub_LSM303A.h"
#include "mock_stub_i2c.h"

void test_init_acc_lsm303a(void) {
    I2C0_writeByteRegister_ExpectAndReturn(ACC_I2C_ADDRESS, CTRL1_A_REG, 0x5C, 0);
    TEST_ASSERT_EQUAL(LSM303A_SUCCESS, init_acc_lsm303a(ACC_SCALE_8G, ACC_RESOLUTION_14BIT, ACC_ODR_200HZ));

    I2C0_writeByteRegister_ExpectAndReturn(ACC_I2C_ADDRESS, CTRL1_A_REG, 0xDC, 0);
    TEST_ASSERT_EQUAL(LSM303A_SUCCESS, init_acc_lsm303a(ACC_SCALE_8G, ACC_RESOLUTION_10BIT, ACC_ODR_200HZ));

    I2C0_writeByteRegister_ExpectAndReturn(ACC_I2C_ADDRESS, CTRL1_A_REG, 0x5E, 0);
    TEST_ASSERT_EQUAL(LSM303A_SUCCESS, init_acc_lsm303a(ACC_SCALE_8G, ACC_RESOLUTION_12BIT, ACC_ODR_1600HZ));

    I2C0_writeByteRegister_ExpectAndReturn(ACC_I2C_ADDRESS, CTRL1_A_REG, 0x5E, 1);
    TEST_ASSERT_EQUAL(LSM303A_FAILURE, init_acc_lsm303a(ACC_SCALE_8G, ACC_RESOLUTION_12BIT, ACC_ODR_1600HZ));

    TEST_ASSERT_EQUAL(LSM303A_INVALID_INPUT, init_acc_lsm303a(ACC_SCALE_8G, ACC_RESOLUTION_12BIT, ACC_ODR_200HZ));
}

void test_init_mag_lsm303a(void) {
    I2C0_writeByteRegister_ExpectAndReturn(MAG_I2C_ADDRESS, LSM303AGR_CFG_REG_A_M, 0x8C, 0);
    TEST_ASSERT_EQUAL(LSM303A_SUCCESS, init_mag_lsm303a(MAG_MODE_CONTINUOUS, MAG_ODR_100HZ));

    I2C0_writeByteRegister_ExpectAndReturn(MAG_I2C_ADDRESS, LSM303AGR_CFG_REG_A_M, 0x8D, 0);
    TEST_ASSERT_EQUAL(LSM303A_SUCCESS, init_mag_lsm303a(MAG_MODE_SINGLE, MAG_ODR_100HZ));

    I2C0_writeByteRegister_ExpectAndReturn(MAG_I2C_ADDRESS, LSM303AGR_CFG_REG_A_M, 0x8E, 0);
    TEST_ASSERT_EQUAL(LSM303A_SUCCESS, init_mag_lsm303a(MAG_MODE_IDLE, MAG_ODR_100HZ));

    I2C0_writeByteRegister_ExpectAndReturn(MAG_I2C_ADDRESS, LSM303AGR_CFG_REG_A_M, 0x8E, 1);
    TEST_ASSERT_EQUAL(LSM303A_FAILURE, init_mag_lsm303a(MAG_MODE_IDLE, MAG_ODR_100HZ));

}

void test_is_acc_online(void) {
    I2C0_readByteRegister_ExpectAndReturn(ACC_I2C_ADDRESS, WHO_AM_I_A_REG, WHO_AM_I_A_REG_VAL);
    TEST_ASSERT_EQUAL(LSM303A_SUCCESS, is_acc_online());

    I2C0_readByteRegister_ExpectAndReturn(ACC_I2C_ADDRESS, WHO_AM_I_A_REG, 0x39);
    TEST_ASSERT_EQUAL(LSM303A_FAILURE, is_acc_online());
}

void test_is_mag_online(void) {
    I2C0_readByteRegister_ExpectAndReturn(MAG_I2C_ADDRESS, WHO_AM_I_M_REG, WHO_AM_I_M_REG_VAL);
    TEST_ASSERT_EQUAL(LSM303A_SUCCESS, is_mag_online());

    I2C0_readByteRegister_ExpectAndReturn(MAG_I2C_ADDRESS, WHO_AM_I_M_REG, 0x39);
    TEST_ASSERT_EQUAL(LSM303A_FAILURE, is_mag_online());
}

void test_get_lsm303a_temperature(void) {
    I2C0_readByteRegister_ExpectAndReturn(MAG_I2C_ADDRESS, OUT_T_A_REG, 0x00);
    TEST_ASSERT_EQUAL(25, get_lsm303a_temperature());

    I2C0_readByteRegister_ExpectAndReturn(MAG_I2C_ADDRESS, OUT_T_A_REG, 0xFF);
    TEST_ASSERT_EQUAL(24, get_lsm303a_temperature());

    I2C0_readByteRegister_ExpectAndReturn(MAG_I2C_ADDRESS, OUT_T_A_REG, 0x7F);
    TEST_ASSERT_EQUAL(152, get_lsm303a_temperature());

    I2C0_readByteRegister_ExpectAndReturn(MAG_I2C_ADDRESS, OUT_T_A_REG, 0x80);
    TEST_ASSERT_EQUAL(-103, get_lsm303a_temperature());
}

void test_get_lsm303a_accXYZ(void){
    float rawValue[3];

    I2C0_readByteRegister_ExpectAndReturn(ACC_I2C_ADDRESS, LSM303AGR_OUT_X_L_A, 0xFC);
    I2C0_readByteRegister_ExpectAndReturn(ACC_I2C_ADDRESS, LSM303AGR_OUT_X_H_A, 0x7F);
    
    I2C0_readByteRegister_ExpectAndReturn(ACC_I2C_ADDRESS, LSM303AGR_OUT_Y_L_A, 0x00);
    I2C0_readByteRegister_ExpectAndReturn(ACC_I2C_ADDRESS, LSM303AGR_OUT_Y_H_A, 0x00);

    I2C0_readByteRegister_ExpectAndReturn(ACC_I2C_ADDRESS, LSM303AGR_OUT_Z_L_A, 0x00);
    I2C0_readByteRegister_ExpectAndReturn(ACC_I2C_ADDRESS, LSM303AGR_OUT_Z_H_A, 0x80);
    
    get_lsm303a_accXYZ(rawValue, ACC_SCALE_16G);

    TEST_ASSERT_EQUAL(15.99, rawValue[0]);
    TEST_ASSERT_EQUAL(0, rawValue[1]);
    TEST_ASSERT_EQUAL(-15.99, rawValue[2]);
}
