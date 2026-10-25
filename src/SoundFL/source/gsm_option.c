// FUNCTION: SWBF2 0x0074e040
void __cdecl gsm_option(void* pGsm, int iOption, unsigned char* pValue)
{
    char* const pThis = (char*)pGsm;

    if (iOption - 1 != 0)
    {
        return;
    }

    if (pValue != 0)
    {
        pThis[0x286] = (char)*pValue;
    }
}

