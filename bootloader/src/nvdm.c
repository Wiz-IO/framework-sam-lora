#include "cfg.h"

typedef enum
{
    CMD_ERASE_MAIN = 0x02U,
    CMD_WRITE_MAIN = 0x04U,

    CMD_ERASE_AUX = 0x05U,
    CMD_WRITE_AUX = 0x06U,

    CMD_ERASE_EE = 0x1AU,
    CMD_WRITE_EE = 0x1CU,

    CMD_LOCK_REGION = 0x40U,
    CMD_UNLOCK_REGION = 0x41U,

    CMD_ENTER_POWERDOWN = 0x42U,
    CMD_EXIT_POWERDOWN = 0x43U,

    CMD_BUFFER_CLEAR = 0x44U, // NVMCTRL_CTRLA_CMD_PBC
    CMD_SET_SECURITY = 0x45U,
    CMD_INVALIDATE = 0x46U
} NVDM_CMD_E;

static int nvdm_exec(NVDM_CMD_E cmd, uint32_t addr)
{
    uint32_t backup = NVMCTRL->CTRLB.reg;
    /* Clear status and int flags */
    NVMCTRL->STATUS.reg = 0x1E;
    NVMCTRL->INTFLAG.reg = 2;
    while (NVMCTRL->INTFLAG.bit.READY == 0)
    {
    }
    /* Possible EERATA bug? */
    NVMCTRL->CTRLB.bit.CACHEDIS = 1;
    /* Address is 16bit based */
    NVMCTRL->ADDR.reg = addr >> 1;
    /* execute command */
    NVMCTRL->CTRLA.reg = cmd | NVMCTRL_CTRLA_CMDEX_KEY;
    while (NVMCTRL->INTFLAG.bit.READY == 0)
    {
    }
    /* Restore ctrlb */
    NVMCTRL->CTRLB.reg = backup;
    /* Return execute status */
    return NVMCTRL->STATUS.reg & 0x1C;
}

/* Address must be aligned 64 */
static int nvdm_write(NVDM_CMD_E cmd, uint32_t addr, uint8_t data[64])
{
    /* clear NVM buffer on no error */
    int status = nvdm_exec(CMD_BUFFER_CLEAR, addr);
    /* Write NVM to flash on no error */
    if (status == 0)
    {
        /* copy to memory */
        __IO uint16_t *mem = (__IO uint16_t *)addr;
        uint16_t *dat = (uint16_t *)data;
        for (uint32_t i = 0u; i < 64u; i += 2u)
            *mem++ = *dat++;
        status = nvdm_exec(cmd, addr);
    }
    return status;
}

int nvdm_flash_erase(uint32_t addr)
{
    //if (addr % 256) return -1; // sure?
    return nvdm_exec(CMD_ERASE_MAIN, addr);
}

int nvdm_flash_write(uint32_t addr, uint8_t data[64])
{
    //if (addr % 64) return -1; // sure?
    return nvdm_write(CMD_WRITE_MAIN, addr, data);
}
