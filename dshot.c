#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "dshot.pio.h"

int main() {


    PIO pio;
    uint sm;
    uint offset;
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&dshot_program, &pio,
        &sm, &offset, 0, 1, true);
    hard_assert(success);

    dshot_program_init(pio, sm, offset, 0);

    uint16_t data = 0xC16B;
    uint32_t packet = (uint32_t)data << 16;

    for (int i = 0;i < 8000;i++) {
        pio_sm_put_blocking(pio, sm, 0x00000000);
        sleep_us(250);
    }
    while (true) {
        pio_sm_put_blocking(pio, sm, packet);
        sleep_us(250);
    }
}
