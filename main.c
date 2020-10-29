// mcc生成
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c_master_example.h"

#include <stdio.h>

// ADXL345のアドレス
#define ADXL345_ADDRESS 0x53

// XYZレジスタ用のテーブル(6Byte)
uint8_t RegTbl[6];

// 低レベル入出力関数のオーバーライド //
void putch(char Data) 
{
    EUSART_Write(Data);
}
char getch(void)
{
    return (EUSART_Read());
}

// ADXL345の初期化 //
void ADXL345_Initialize()
{
    // デバイスの DATA_FORMATのアドレスに 「最大分解能モード」および「+-16g」を設定
    I2C_Write1ByteRegister(ADXL345_ADDRESS, 0x31, 0x0b);
    // デバイスの POWER_CTLのアドレスに 「測定モード」を設定
    I2C_Write1ByteRegister(ADXL345_ADDRESS, 0x2d, 0x08);
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    ADXL345_Initialize();
    
    while (1)
    {
        if (eusartRxCount > 0)
        {
            char data = getch();
            __delay_ms(5000);
        }
        I2C_Write1ByteRegister(ADXL345_ADDRESS, 0x32, 0);
        I2C_ReadNBytes(ADXL345_ADDRESS, RegTbl, 6);
        
        // データを各XYZの値に変換する(LSB単位)
        int16_t x = (((int16_t)RegTbl[1]) << 8) | RegTbl[0];
        int16_t y = (((int16_t)RegTbl[3]) << 8) | RegTbl[2];
        int16_t z = (((int16_t)RegTbl[5]) << 8) | RegTbl[4];
        
        // 各XYZ軸の加速度(m/s^2)を出力する
        printf("\r\n%lf %lf %lf", x * 0.0392266, y * 0.0392266, z * 0.0392266);
        
        __delay_ms(200);
    }
}
/**
 End of File
*/