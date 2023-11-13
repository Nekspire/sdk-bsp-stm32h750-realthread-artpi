#ifndef __I2C_CONFIG_H__
#define __I2C_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef BSP_USING_I2C4
#ifndef I2C4_CONFIG
#define I2C4_CONFIG                                                \
    {                                                              \
       .Instance = I2C4,                                           \
       .Init.Timing = 0x307075B1,                                  \
       .Init.OwnAddress1 = 0,                                      \
       .Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,             \
       .Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,            \
       .Init.OwnAddress2 = 0,                                      \
       .Init.OwnAddress2Masks = I2C_OA2_NOMASK,                    \
       .Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,            \
       .Init.NoStretchMode = I2C_NOSTRETCH_DISABLE,                \
    }                                                               
#endif /* I2C4_CONFIG */
#endif /* BSP_USING_I2C4 */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_CONFIG_H__ */
