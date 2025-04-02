#include <scan_code.h>

scan_code_t GetKey(void)
{
    int code;

    while (true)
    {
        code = _getch();

        switch (code)
        {
            case UP_C:
                printf("symbol = up\n");
                break;
            case DOWN_C:
                printf("symbol = down\n");
                break;
            case LEFT_C:
                printf("symbol = left\n");
                break;
            case RIGHT_C:
                printf("symbol = right\n");
                break;
            case ESC_C:
                printf("escaping...\n");
                break;
            default:
                printf("some else code: %d \n", code);
                break;
        }
        return (scan_code_t) code;
    }

    return ERROR_C;
}
