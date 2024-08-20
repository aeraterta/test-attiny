/**
  ******************************************************************************
  * @file    LSM303A.h
  * @author  - Anthony E.Raterta
  *          - Timothy Gorbunov
  * @version V1.0.0
  * @date    09-February-2024
  * @brief   Contains all the prototypes for the LSM303A.C
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

#ifndef LSM303A_H
#define LSM303A_H

#include "../hardware/i2c.h"

typedef enum {
    LSM303A_SUCCESS,
    LSM303A_FAILURE,
    LSM303A_INVALID_INPUT,
} LSM303A_STATUS;

typedef enum {
  ACC_RESOLUTION_10BIT,                   
  ACC_RESOLUTION_12BIT,
  ACC_RESOLUTION_14BIT,                       
} ACC_RESOLUTION_CONFIG;

typedef enum {
  ACC_SCALE_2G,
  ACC_SCALE_16G,
  ACC_SCALE_4G,
  ACC_SCALE_8G,
} ACC_SCALE_CONFIG;

typedef enum {
  ACC_ODR_1HZ,
  ACC_ODR_12_5HZ,
  ACC_ODR_25HZ,
  ACC_ODR_50HZ,
  ACC_ODR_100HZ,
  ACC_ODR_200HZ,
  ACC_ODR_400HZ,
  ACC_ODR_800HZ,
  ACC_ODR_1600HZ,
  ACC_ODR_3200HZ,
  ACC_ODR_6400HZ,
} ACC_ODR_CONFIG;

typedef enum {
  MAG_ODR_10HZ,
  MAG_ODR_20HZ,
  MAG_ODR_50HZ,
  MAG_ODR_100HZ,
} MAG_ODR_CONFIG;

typedef enum {
  MAG_MODE_CONTINUOUS,
  MAG_MODE_SINGLE,
  MAG_MODE_IDLE,
} MAG_MODE_CONFIG;

/*****************************ID REGISTERS*************************************/
#define ACC_I2C_ADDRESS                     0x32
#define MAG_I2C_ADDRESS                     0x3C

#define WHO_AM_I_A_REG                      0x0F
#define WHO_AM_I_A_REG_VAL                  0x43
#define WHO_AM_I_M_REG                      0x4F
#define WHO_AM_I_M_REG_VAL                  0x40

/************************TEMPERATURE REGISTERS*********************************/
#define OUT_T_A_REG                         0X26

/***********************ACCELEROMETER REGISTERS********************************/
#define CTRL1_A_REG                         0x20  
#define CTRL2_A_REG                         0x21  
#define CTRL3_A_REG                         0x22  
#define CTRL4_A_REG                         0x23 
#define CTRL5_A_REG                         0x24
#define FIFO_CTRL_A_REG                     0x25
#define STATUS_A_REG                        0x27

#define LSM303AGR_OUT_X_L_A                 0x28                                /* Output Register X acceleration */
#define LSM303AGR_OUT_X_H_A                 0x29                                /* Output Register X acceleration */
#define LSM303AGR_OUT_Y_L_A                 0x2A                                /* Output Register Y acceleration */
#define LSM303AGR_OUT_Y_H_A                 0x2B                                /* Output Register Y acceleration */
#define LSM303AGR_OUT_Z_L_A                 0x2C                                /* Output Register Z acceleration */
#define LSM303AGR_OUT_Z_H_A                 0x2D                                /* Output Register Z acceleration */ 
#define LSM303AGR_FIFO_CTRL_REG_A           0x2E                                /* Fifo control Register acceleration */
#define LSM303AGR_FIFO_SRC_REG_A            0x2F                                /* Fifo src Register acceleration */

#define LSM303AGR_INT1_CFG_A                0x30                                /* Interrupt 1 configuration Register acceleration */
#define LSM303AGR_INT1_SOURCE_A             0x31                                /* Interrupt 1 source Register acceleration */
#define LSM303AGR_INT1_THS_A                0x32                                /* Interrupt 1 Threshold register acceleration */
#define LSM303AGR_INT1_DURATION_A           0x33                                /* Interrupt 1 DURATION register acceleration */

#define LSM303AGR_INT2_CFG_A                0x34                                /* Interrupt 2 configuration Register acceleration */
#define LSM303AGR_INT2_SOURCE_A             0x35                                /* Interrupt 2 source Register acceleration */
#define LSM303AGR_INT2_THS_A                0x36                                /* Interrupt 2 Threshold register acceleration */
#define LSM303AGR_INT2_DURATION_A           0x37                                /* Interrupt 2 DURATION register acceleration */

#define LSM303AGR_CLICK_CFG_A               0x38                                /* Click configuration Register acceleration */
#define LSM303AGR_CLICK_SOURCE_A            0x39                                /* Click 2 source Register acceleration */
#define LSM303AGR_CLICK_THS_A               0x3A                                /* Click 2 Threshold register acceleration */

#define LSM303AGR_TIME_LIMIT_A              0x3B                                /* Time Limit Register acceleration */
#define LSM303AGR_TIME_LATENCY_A            0x3C                                /* Time Latency Register acceleration */
#define LSM303AGR_TIME_WINDOW_A             0x3D                                /* Time window register acceleration */

/* System Registers(New vs lsm303dlhc.h) */
#define LSM303AGR_Act_THS_A                 0x3E                                /* return to sleep activation threshold register */
#define LSM303AGR_Act_DUR_A                 0x3F                                /* return to sleep duration register */
/* Magnetometer */
#define LSM303AGR_X_REG_L_M                 0x45                                /* Hard-iron X magnetic field */
#define LSM303AGR_X_REG_H_M                 0x46                                /* Hard-iron X magnetic field */
#define LSM303AGR_Y_REG_L_M                 0x47                                /* Hard-iron Y magnetic field */
#define LSM303AGR_Y_REG_H_M                 0x48                                /* Hard-iron Y magnetic field */
#define LSM303AGR_Z_REG_L_M                 0x49                                /* Hard-iron Z magnetic field */
#define LSM303AGR_Z_REH_H_M                 0x4A                                /* Hard-iron Z magnetic field */
#define LSM303AGR_WHO_AM_I_M                0x4F                                /* Who am i register magnetic field (0x40) */
#define LSM303AGR_CFG_REG_A_M               0x60                                /* Configuration register A magnetic field */
#define LSM303AGR_CFG_REG_B_M               0x61                                /* Configuration register B magnetic field */
#define LSM303AGR_CFG_REG_C_M               0x62                                /* Configuration register C magnetic field */
#define LSM303AGR_INT_CTRL_REG_M            0x63                                /* interrupt control register magnetic field */
#define LSM303AGR_INT_SOURCE_REG_M          0x64                                /* interrupt source register magnetic field */
#define LSM303AGR_INT_THS_L_REG_M           0x65                                /* interrupt threshold register magnetic field */
#define LSM303AGR_INT_THS_H_REG_M           0x66                                /* interrupt threshold register magnetic field*/
#define LSM303AGR_STATUS_REG_M              0x67                                /* Status Register magnetic field */
#define LSM303AGR_OUTX_L_REG_M              0x68                                /* Output Register X magnetic field */
#define LSM303AGR_OUTX_H_REG_M              0x69                                /* Output Register X magnetic field */
#define LSM303AGR_OUTY_L_REG_M              0x6A                                /* Output Register X magnetic field */
#define LSM303AGR_OUTY_H_REG_M              0x6B                                /* Output Register X magnetic field */
#define LSM303AGR_OUTZ_L_REG_M              0x6C                                /* Output Register X magnetic field */
#define LSM303AGR_OUTZ_H_REG_M              0x6D                                /* Output Register X magnetic field */

/*******************************REGISTERS**************************************/


int init_acc_lsm303a(ACC_SCALE_CONFIG scale, 
                    ACC_RESOLUTION_CONFIG resolution, 
                    ACC_ODR_CONFIG odr);
int init_mag_lsm303a(MAG_MODE_CONFIG mode, MAG_ODR_CONFIG odr);
int is_acc_online (void);
int is_mag_online (void);
int get_lsm303a_temperature(void);
int lsm303a_register_to_int(uint16_t num, int bits);
void get_lsm303a_accXYZ(float* pData, ACC_SCALE_CONFIG scale);



/*
                  Resolution        
ODR       10-bit   12-bit   14-bit
-          0000              
1          1000              
12.5       1001              0001
25         1010              0010
50         1011              0011
100        1100              0100
200        1101              0101
400        1110              0110
800        1111              0111
1600                 0101     
3200                 0110     
6400                 0111     
*/

#endif /* LSM303A_H */
