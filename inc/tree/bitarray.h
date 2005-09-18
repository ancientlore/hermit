/* ---------------------------------------------------------------------------
  BitArray class

        The bit array class can be used as a giant bit array to
		maintain various state settings.

  Copyright (C) 1993 Michael D. Lore
  All Rights Reserved.
--------------------------------------------------------------------------- */

#ifndef BITARRAY_H
#define BITARRAY_H

#include "common.h"

const unsigned BITARR_ALLSET = unsigned (-1);


// ABOUT BIT VALUES:
//	Bit values are always bool (true or false)

// ABOUT BIT NUMBERS:
//	Bit numbers range from zero to numBits - 1.

class BitArray
{
  unsigned *bitArray;
  unsigned numInts;
  unsigned numBits;

  unsigned index(unsigned bitNumber) const
  {
    return (bitNumber / (sizeof(unsigned) * 8));
  }

  unsigned mask(unsigned bitNumber) const
  {
    return (1 << (bitNumber % (sizeof(unsigned) * 8)));
  }

  // No copy of a bit array
  BitArray (const BitArray&);
  BitArray& operator = (const BitArray&);

public:

  BitArray(unsigned numberOfBits, bool setAll = false); // throws ERR_OUT_OF_MEMORY
  ~BitArray();

  void clearAll();
  void setAll();

  // These return the bit number (not necessarily in order):

  unsigned setNext();		// throws ERR_NOT_FOUND, ERR_RANGE
  unsigned clearNext();		// throws ERR_NOT_FOUND, ERR_RANGE

  // These return previous value of bit

  bool set(unsigned bitNumber);		// throws ERR_RANGE
  bool clear(unsigned bitNumber);	// throws ERR_RANGE

  // These return the bit

  bool isSet(unsigned bitNumber) const;

  unsigned size () const { return numBits; }
};

#endif

/* End */
