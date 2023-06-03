/*
 * QEMU System Emulator
 *
 * Copyright (c) 2003-2020 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "qemu/osdep.h"
#include "qemu-main.h"
#include "sysemu/sysemu.h"

#ifdef CONFIG_SDL
#include <SDL.h>
#endif

#include "exec/memory.h"
#include "exec/address-spaces.h"

#include <stdint.h>

void cemu_mmio(unsigned long addr, void *buf, unsigned long len, bool is_write);

void cemu_mmio(unsigned long addr, void *buf, unsigned long len, bool is_write) {
    //fprintf(stderr, "mmio addr %ld len %ld is_write %d\n", addr, len, is_write);
    MemTxAttrs attrs = MEMTXATTRS_UNSPECIFIED;
    address_space_rw(&address_space_memory, addr,
                            attrs, buf,
                            len, is_write);
    return;
}

void cemu_main(const char *);

static void *cemu_thread_fn(void *arg)
{
    cemu_main("/home/zenithal/fw_payload_qemu_uart.bin");
    return arg;
}

static void cemu_thread(void)
{
    QemuThread q;
    qemu_thread_create(&q, "cemu", cemu_thread_fn,
                       &q, QEMU_THREAD_JOINABLE);
}

int qemu_default_main(void)
{
    int status;

    cemu_thread();

    status = qemu_main_loop();
    qemu_cleanup();

    return status;
}

int (*qemu_main)(void) = qemu_default_main;

int main(int argc, char **argv)
{
    qemu_init(argc, argv);
    return qemu_main();
}
