/**
  ******************************************************************************
  * @file    LSM303A.c
  * @author  - Anthony E.Raterta
  *          - Timothy Gorbunov
  * @version V1.0.0
  * @date    09-February-2024
  * @brief   Contains all the functionalities to control the LSM303A
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Gyrofun.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */ 
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "LSM303A.h"

int init_acc_lsm303a(ACC_SCALE_CONFIG scale, 
                    ACC_RESOLUTION_CONFIG resolution, 
                    ACC_ODR_CONFIG odr){
    
    uint8_t val = 0x00;

    switch (resolution){
        case ACC_RESOLUTION_10BIT:
            if (odr > ACC_ODR_800HZ) {
                return LSM303A_INVALID_INPUT;
            }
            else {
                val = (8 | odr) << 4;                                           //CTRL1_A[7:4] for ODR       
                val = val | scale << 2;                                         //CTRL1_A[3:2] for FS   
                val = val | 0 << 1;                                             //CTRL1_A[1] for HF_ODR
            }
            break;

        case ACC_RESOLUTION_12BIT:
            if (odr < ACC_ODR_1600HZ) {
                return LSM303A_INVALID_INPUT;
            }
            else {
                val = (odr - 3) << 4;                                           //CTRL1_A[7:4] for ODR       
                val = val | scale << 2;                                         //CTRL1_A[3:2] for FS   
                val = val | 1 << 1;                                             //CTRL1_A[1] for HF_ODR
            }
            break;
            
        case ACC_RESOLUTION_14BIT: 
            if (odr > ACC_ODR_800HZ) {
                return LSM303A_INVALID_INPUT;
            }
            else {
                val = odr << 4;                                                 //CTRL1_A[7:4] for ODR       
                val = val | scale << 2;                                         //CTRL1_A[3:2] for FS   
                val = val | 0 << 1;                                             //CTRL1_A[1] for HF_ODR
            }
            break;
    }

    return I2C0_writeByteRegister(ACC_I2C_ADDRESS, CTRL1_A_REG, val);
}

int init_mag_lsm303a(MAG_MODE_CONFIG mode, MAG_ODR_CONFIG odr){
    uint8_t val = 0x00;

    val = 1 << 7;                                                               //CFG_REG_A_M[7] for temperature compensation
    val = val | 0 << 4;                                                         //CFG_REG_A_M[4] for high-resolution mode
    val = val | odr << 2;                                                       //CFG_REG_A_M[4] for high-resolution mode
    val = val | mode;                                                           //CFG_REG_A_M[4] for high-resolution mode

    return I2C0_writeByteRegister(MAG_I2C_ADDRESS, LSM303AGR_CFG_REG_A_M, val);
}

int is_acc_online (void) {
    uint8_t ret_Val = I2C0_readByteRegister(ACC_I2C_ADDRESS, WHO_AM_I_A_REG);

    if (ret_Val == WHO_AM_I_A_REG_VAL){
        return LSM303A_SUCCESS;
    }
    else {
        return LSM303A_FAILURE;
    }
}

int is_mag_online (void) {
    uint8_t ret_Val = I2C0_readByteRegister(MAG_I2C_ADDRESS, WHO_AM_I_M_REG);

    if (ret_Val == WHO_AM_I_M_REG_VAL){
        return LSM303A_SUCCESS;
    }
    else {
        return LSM303A_FAILURE;
    }
}

int get_lsm303a_temperature(void) {
    uint8_t ret_Val = I2C0_readByteRegister(MAG_I2C_ADDRESS, OUT_T_A_REG);
    int ret = lsm303a_register_to_int(ret_Val, 8);
    ret += 25;

    return ret;
}

int lsm303a_register_to_int(uint16_t num, int bits) {
    
    int msb_mask = 1 << (bits - 1);                                             // Create a mask for the most significant bit
    if (num & msb_mask) {                                                       // If the number is negative
        return (num | ~(msb_mask - 1));                                         // Convert to two's complement
    }

    return num;                                                                 // Positive numbers remain unchanged
}

void get_lsm303a_accXYZ(float* pData, ACC_SCALE_CONFIG scale){

    uint8_t val[6];
    uint16_t rawValue[3];
    float sensitivity = 0;


    switch (scale){
        case ACC_SCALE_2G:
            sensitivity = 0.061;
            break;

        case ACC_SCALE_16G:
            sensitivity = 0.488;
            break;

        case ACC_SCALE_4G:
            sensitivity = 0.122;
            break;
    
        case ACC_SCALE_8G:
            sensitivity = 0.244;
            break;
    };

    /* Read output register X, Y & Z acceleration */
    val[0] = I2C0_readByteRegister(ACC_I2C_ADDRESS, LSM303AGR_OUT_X_L_A); 
    val[1] = I2C0_readByteRegister(ACC_I2C_ADDRESS, LSM303AGR_OUT_X_H_A);
    val[2] = I2C0_readByteRegister(ACC_I2C_ADDRESS, LSM303AGR_OUT_Y_L_A);
    val[3] = I2C0_readByteRegister(ACC_I2C_ADDRESS, LSM303AGR_OUT_Y_H_A);
    val[4] = I2C0_readByteRegister(ACC_I2C_ADDRESS, LSM303AGR_OUT_Z_L_A);
    val[5] = I2C0_readByteRegister(ACC_I2C_ADDRESS, LSM303AGR_OUT_Z_H_A);

    for(uint8_t i = 0; i < 3; i++){
        rawValue[i] = (int16_t)((uint16_t)(val[(2 * i) + 1] << 8) + val[2 * i]);
        //pData[i] = (lsm303a_register_to_int(rawValue[i], 16) * sensitivity) / 1000;
        pData[i] = round((lsm303a_register_to_int(rawValue[i], 16) * sensitivity) / 10.0) / 100.0;
    }
}