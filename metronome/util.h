#ifndef __util_h__
#define __util_h__

#define bool uint8_t
#define true 1u
#define false 0u

#define bv(bit) (1 << (bit))
#define setBit(byte, bit) ((byte) |= bv(bit))
#define clearBit(byte, bit) ((byte) &= ~bv(bit))
#define toggleBit(byte, bit) ((byte) ^= bv(bit))

#define setInputPullup(ddr, port, pin) clearBit((ddr),(pin));setBit((port),(pin))

#define cursorVisible(cursorPos, currentPos, symbol) (((cursorPos) == (currentPos)) ? (symbol) : ' ')

#define CLK_NO_DIVIDE 0

#define within(param, beg, end) (((param)>=(beg))&&((param)<=(end)))
#define between(param, beg, end) (((param) > (beg)) && ((param) < (end)))

#define signature_lower(signature) ((signature) & 0x0f)
#define signature_upper(signature) ((signature) >> 4)

#endif //__util_h__