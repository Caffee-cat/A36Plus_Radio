#include "fmc.h"
uint16_t TempData[512] = {0};




static void FMC_unlock(void)
{
    fmc_unlock();
    ob_unlock();

    fmc_flag_clear(FMC_FLAG_END);
    fmc_flag_clear(FMC_FLAG_WPERR);
    fmc_flag_clear(FMC_FLAG_PGERR);
}


uint32_t channel_A_get(void)
{
    return *(__IO uint32_t *)(CHANNEL_A_KHZ_ADDR) * 100 + *(__IO uint16_t *)(CHANNEL_A_HZ_ADDR);
}

uint32_t channel_B_get(void)
{
    return *(__IO uint32_t *)(CHANNEL_B_KHZ_ADDR) * 100 + *(__IO uint16_t *)(CHANNEL_B_HZ_ADDR);
}




void channel_A_store(uint32_t channel_val)
{

    for (uint16_t i = 0; i < 512; i++)
        TempData[i] = *(__IO uint16_t *)(CHANNEL_PAGE_ADDR + i * 2);
    FMC_unlock();

    while (1)
    {
        fmc_state_enum state = fmc_page_erase(0x0800c800); // page 50
        if (state == FMC_READY)
            break;
    }

    TempData[(CHANNEL_A_KHZ_ADDR - CHANNEL_PAGE_ADDR) / 2] = (channel_val / 100) % (256 * 256);
    TempData[(CHANNEL_A_KHZ_ADDR - CHANNEL_PAGE_ADDR) / 2 + 1] = (channel_val / 100) / 256 / 256;
    TempData[(CHANNEL_A_KHZ_ADDR - CHANNEL_PAGE_ADDR) / 2 + 2] = channel_val % 100;

    for (uint16_t i = 0; i < 512; i++)
        fmc_halfword_program(CHANNEL_PAGE_ADDR + i * 2, TempData[i]);
    fmc_lock();
}




void channel_B_store(uint32_t channel_val)
{
    for (uint16_t i = 0; i < 512; i++)
        TempData[i] = *(__IO uint16_t *)(CHANNEL_PAGE_ADDR + i * 2);

    FMC_unlock();

    while (1)
    {
        fmc_state_enum state = fmc_page_erase(0x0800c800); // page 50
        if (state == FMC_READY)
            break;
    }

    TempData[(CHANNEL_B_KHZ_ADDR - CHANNEL_PAGE_ADDR) / 2] = (channel_val / 100) % (256 * 256);
    TempData[(CHANNEL_B_KHZ_ADDR - CHANNEL_PAGE_ADDR) / 2 + 1] = (channel_val / 100) / 256 / 256;
    TempData[(CHANNEL_B_KHZ_ADDR - CHANNEL_PAGE_ADDR) / 2 + 2] = channel_val % 100;


    for (uint16_t i = 0; i < 512; i++)
        fmc_halfword_program(CHANNEL_PAGE_ADDR + i * 2, TempData[i]);
    fmc_lock();
}




void FMC_channel_init(void)
{
    uint32_t OutData = (*(__IO uint32_t *)(0x0800c800));
    if (OutData == CAHNNEL_FMC_INIT_FALG)
        return;

    FMC_unlock();

    while (1)
    {
        fmc_state_enum state = fmc_page_erase(0x0800c800); // page 50
        if (state == FMC_READY)
            break;
    }

    fmc_word_program(CHANNEL_FMC_INIT_FALG_ADDR, CAHNNEL_FMC_INIT_FALG);

    fmc_word_program(CHANNEL_A_KHZ_ADDR, 487625);

    fmc_halfword_program(CHANNEL_A_HZ_ADDR, 00);

    fmc_word_program(CHANNEL_B_KHZ_ADDR, 439460);

    fmc_halfword_program(CHANNEL_B_HZ_ADDR, 00);

    fmc_lock();
}

void FMC_data_init(void)
{
    FMC_channel_init();
}