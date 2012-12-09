#include "../remote.h"

int REMOTE_active(void)
{
  register u08 i = 50;
  while (i--)
  if (bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT))
    return 1;
  return 0;
}
