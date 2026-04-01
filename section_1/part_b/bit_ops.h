#include <systemc.h>
#ifndef BIT_OPS_GUARD
#define BIT_OPS_GUARD
bool checkBit(uint64_t N, uint64_t K) {
      return ((N) & (1UL<<(K)));
}

uint64_t clearBit(uint64_t N, uint64_t K) {
      return (N & (~(1UL << (K))));
}

uint64_t setBit(uint64_t N, uint64_t K) {
      return (N | (1UL << (K)));
}
#endif 
