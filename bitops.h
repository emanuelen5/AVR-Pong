

#ifndef _BITOPS_H_
#define _BITOPS_H_ 1

#define BV(BIT)                             (1 << (BIT))                            // Bit Value
#define set_bit(BYTE, BIT)                  BYTE |= BV(BIT)                         // Set a bit
#define clear_bit(BYTE, BIT)                BYTE &= ~BV(BIT)                        // Clear a bit
#define toggle_bit(BYTE, BIT)               BYTE ^= BV(BIT)                         // Toggle a bit
#define bit_is_set(BYTE, BIT)               (BYTE & BV(BIT))                        // Test if bit is set
#define bit_is_clear(BYTE, BIT)             !(bit_is_set(BYTE, BIT))                // Test if bit is clear
#define get_bit(BYTE, BIT)                  (bit_is_set(BYTE, BIT) >> (BIT))        // Get the value of a bit
#define mask_set_bits(BYTE, clear, set)     BYTE = (BYTE & ~clear) | (clear & set)  // 

#endif