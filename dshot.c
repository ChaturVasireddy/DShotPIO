#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "dshot.pio.h"

int main() {
    gpio_init(15);
    gpio_set_dir(15, GPIO_OUT);
    gpio_put(15, 0);

    PIO pio;
    uint sm;
    uint offset;
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&dshot_program, &pio,
        &sm, &offset, 0, 1, true);
    hard_assert(success);

    dshot_program_init(pio, sm, offset, 0);

    for (int i = 0;i < 2000;i++) {
        pio_sm_put_blocking(pio, sm, 0x00000000);
        sleep_ms(1);
    }
    for (int i = 0;i < 10;i++) {
        pio_sm_put_blocking(pio, sm, (uint32_t)0x0145 << 16);
        sleep_ms(1);
    }
    while (true) {
        for (int i = 0;i < 10000;i++) {
            gpio_put(15, 1);
            pio_sm_put_blocking(pio, sm, (uint32_t)0x4466 << 16);
            sleep_ms(1);
        }
        for (int i = 0;i < 10000;i++) {
            gpio_put(15, 1);
            pio_sm_put_blocking(pio, sm, (uint32_t)0xC16B << 16);
            sleep_ms(1);
        }
    }
}
