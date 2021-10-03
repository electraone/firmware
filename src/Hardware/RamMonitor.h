// Teensy 3.x RAM Monitor
// Copyright by Adrian Hunt (c) 2015 - 2016
//
// This file is licensed under the MIT license.
// See the LICENSE file for the full copyright text.

#ifndef RAMMONITOR_H
#define RAMMONITOR_H "1.0"

#include <malloc.h>
#include <inttypes.h>

extern int *__brkval; // top of heap (dynamic ram): grows up towards stack
extern char
    _estack; // bottom of stack, top of ram: stack grows down towards heap

class RamMonitor
{
private:
    typedef uint32_t MemMarker;
    typedef uint8_t MemState;

    // user defined consts
    static const uint16_t STACKALLOCATION =
        1024; // stack allocation step size: must be 32bit boundries, div'able by 4
    static const uint16_t LOWMEM =
        4096; // low memory warning: 4kb (less than between stack and heap)

    // internal consts
    static const uint32_t HWADDRESS_RAMSTART =
#if defined(__MK64FX512__) || defined(__MK66FX1M0__)
        0x1FFF0000; // teensy 3.5, 3.6
#elif defined(__MK20DX256__)
        0x1FFF8000; // teensy 3.1, 3.2
#elif defined(__MKL26Z64__)
        0x1FFFF800; // teensy LC
#else
        0x1FFFE000; // teensy 3.0
#endif
    static const MemMarker MEMMARKER =
        0x524D6D6D; // chars RMmm ... Ram Monitor memory marker
    static const uint16_t MARKER_STEP = STACKALLOCATION / sizeof(MemMarker);

    static const MemState msOk = 0;
    static const MemState msLow = 1;
    static const MemState msCrash = 2;

    MemMarker *_mlastmarker; // last uncorrupted memory marker
    MemState _mstate; // detected memory state

    void _check_stack()
    {
        int32_t free;

        // skip markers already comsumed by the stack
        free = ((char *)&free) - ((char *)_mlastmarker);
        if (free < 0) {
            int32_t steps;

            steps = free / STACKALLOCATION; // note steps will be negitive
            if (free % STACKALLOCATION)
                --steps;

            _mlastmarker += MARKER_STEP * steps;
        };

        // check last marker and move if corrupted
        while ((*_mlastmarker != MEMMARKER)
               && (_mlastmarker >= (MemMarker *)__brkval))
            _mlastmarker -= MARKER_STEP;
    };

public:
    int32_t unallocated() const
    {
        char tos;
        return &tos - (char *)__brkval;
    }; // calcs space between heap and stack (current): will be negitive if heap/stack crash
    uint32_t stack_used() const
    {
        char tos;
        return &_estack - &tos;
    }; // calcs stack size (current): grows into unallocated
    uint32_t heap_total() const
    {
        return mallinfo().arena;
    }; // returns heap size: grows into unallocated
    uint32_t heap_used() const
    {
        return mallinfo().uordblks;
    }; // returns heap allocated
    uint32_t heap_free() const
    {
        return mallinfo().fordblks;
    }; // returns free heap

    int32_t free() const
    {
        return unallocated() + heap_free();
    }; // free ram: unallocated and unused heap
    uint32_t total() const
    {
        return &_estack - (char *)HWADDRESS_RAMSTART;
    }; // physical ram

    // these functions (along with initialize and run)
    // create the ellusion of stack allocation.
    uint32_t stack_total()
    { // uses memory markers to "alloc" unallocated
        _check_stack();
        return &_estack - (char *)_mlastmarker;
    };

    int32_t stack_free()
    { // calc stack usage before next marker corruption
        char tos;

        _check_stack();
        return &tos - (char *)_mlastmarker;
    };

    int32_t adj_unallocd()
    { // calcs space between heap and "alloc'd" stack: will be negitive if heap/stack crash
        _check_stack();
        return ((char *)_mlastmarker) - (char *)__brkval;
    };

    int32_t adj_free()
    {
        return adj_unallocd() + heap_free();
    }; // free ram: unallocated and unused heap

    bool warning_lowmem() const
    {
        return (_mstate & msLow);
    }; // returns true when unallocated memory is < LOWMEM
    bool warning_crash() const
    {
        return (_mstate & msCrash);
    }; // returns true when stack is in danger of overwriting heap

    void initialize()
    {
        MemMarker *marker = (MemMarker *)&_estack; // top of memory
        int32_t size;
        int32_t steps;

        // skip current stack;
        size = &_estack
               - (char *)&marker; // current stack size: marker address is tos
        steps = size / STACKALLOCATION;
        if (size % STACKALLOCATION)
            ++steps;

        marker -= MARKER_STEP * steps;

        // record current top of stack
        _mlastmarker = marker;
        _mstate = msOk;

        // mark unused ram between top of stack and top of heap
        while (marker >= (MemMarker *)__brkval) {
            *marker = MEMMARKER; // write memory marker
            marker -= MARKER_STEP;
        };
    };

    void run()
    {
        int32_t unallocd = adj_unallocd(); // calls _check_stack() internally

        if (unallocd < 0)
            _mstate = msCrash | msLow;
        else if (unallocd < LOWMEM)
            _mstate = msLow;
        else
            _mstate = msOk;
    };

    #define MAX_RAM 210000 // Idealy calculate out of BSS size

    size_t getFreeRam(void)
    {
        int freeRam = adj_free();

        if (freeRam < 0) {
            freeRam = 0;
        }

        return (freeRam / (MAX_RAM / 100));
    }
};

#endif
