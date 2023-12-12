#include <board.h>
#include "drv_config.h"
#include <rtthread.h>
#include <rtdevice.h>

#ifdef RT_USING_I2C

#define DRV_DEBUG
#define LOG_TAG "drv.i2c"
#define LOG_LVL LOG_LVL_DBG
#include <drv_log.h>

#if !defined(BSP_USING_I2C4)
#error "Please define at least one BSP_USING_I2Cx"
#endif

enum
{
#ifdef BSP_USING_I2C4
    I2C4_INDEX,
#endif
};

static I2C_HandleTypeDef i2c_config[] =
{
#ifdef BSP_USING_I2C4
    I2C4_CONFIG,
#endif
};

struct stm32_i2c
{
    I2C_HandleTypeDef handler;
    struct rt_i2c_bus_device bus;
};

#define I2C_OBJ_LEN sizeof(i2c_config) / sizeof(i2c_config[0])

static struct stm32_i2c stm32_i2c_obj[I2C_OBJ_LEN];

static rt_size_t stm32_i2c_xfer(struct rt_i2c_bus_device *bus, struct rt_i2c_msg msgs[], rt_uint32_t num)
{
    struct rt_i2c_msg *msg;
    rt_uint32_t i;
    rt_err_t ret = -RT_ERROR;

    for (i = 0; i < num; i++)
    {
        msg = &msgs[i];

        if (msg->flags & RT_I2C_RD)
        {
            if (HAL_OK != HAL_I2C_Master_Receive_IT(&stm32_i2c_obj[i].handler, msg->addr, msg->buf, msg->len))
            {
                LOG_E("i2c bus read failed, i2c bus stop!");
            }
            else
            {
                while (HAL_I2C_GetState(&stm32_i2c_obj[i].handler) != HAL_I2C_STATE_READY);
            }
        }
        else
        {
            if (HAL_OK != HAL_I2C_Master_Transmit_IT(&stm32_i2c_obj[i].handler, msg->addr, msg->buf, msg->len))
            {
                LOG_E("i2c bus write failed, i2c bus stop!");
            }
            else
            {
                while (HAL_I2C_GetState(&stm32_i2c_obj[i].handler) != HAL_I2C_STATE_READY);
            }
        }
    }

    ret = i;

    return ret;
}

static const struct rt_i2c_bus_device_ops i2c_ops =
{
    stm32_i2c_xfer,
    RT_NULL,
    RT_NULL
};

static int stm32_i2c_init(void)
{
    char name_buf[5] = {'i', '2', 'c', '0', 0};

    for (int i = 0; i < I2C_OBJ_LEN; i++)
    {
        /* init i2c */
        stm32_i2c_obj[i].handler = i2c_config[i];
#if defined(I2C4)
        if (stm32_i2c_obj[i].handler.Instance == I2C4)
        {
            name_buf[3] = '4';
        }
#endif
        if (HAL_I2C_Init(&stm32_i2c_obj[i].handler) != HAL_OK)
        {
            LOG_E("%s init failed", name_buf);
            return -RT_ERROR;
        }
        if (HAL_I2CEx_ConfigAnalogFilter(&stm32_i2c_obj[i].handler, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
        {
            LOG_E("%s init failed", name_buf);
            return -RT_ERROR;
        }
        if (HAL_I2CEx_ConfigDigitalFilter(&stm32_i2c_obj[i].handler, 0) != HAL_OK)
        {
            LOG_E("%s init failed", name_buf);
            return -RT_ERROR;
        }
        else
        {
            stm32_i2c_obj[i].bus.ops = &i2c_ops;

            /* register i2c device */
            if (rt_i2c_bus_device_register(&stm32_i2c_obj[i].bus, name_buf) == RT_EOK)
            {
                LOG_D("%s init success", name_buf);
            }
            else
            {
                LOG_E("%s register failed", name_buf);
                return -RT_ERROR;
            }
        }
    }

    return RT_EOK;
}

void I2C4_EV_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_I2C_EV_IRQHandler(&stm32_i2c_obj[I2C4_INDEX].handler);
    rt_interrupt_leave();
}

void I2C4_ER_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_I2C_ER_IRQHandler(&stm32_i2c_obj[I2C4_INDEX].handler);
    rt_interrupt_leave();
}

INIT_BOARD_EXPORT(stm32_i2c_init);

#endif /* RT_USING_I2C */
