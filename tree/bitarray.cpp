/* ---------------------------------------------------------------------------
  Function:	BitArray class

  Include:	bitarray.h

  Description:	The bit array class can be used as a giant bit array to
		maintain various state settings.

  Copyright (C) 1993 Michael D. Lore
  All Rights Reserved.
--------------------------------------------------------------------------- */

#include <string.h>
#include "tree/bitarray.h"

BitArray::BitArray(unsigned numberOfBits, bool SetAll)
{
  numBits = numberOfBits;
  numInts = numberOfBits / (sizeof(unsigned) * 8);
  if (numberOfBits % (sizeof(unsigned) * 8))
    numInts++;
  try {
      bitArray = new unsigned [numInts];
  }
  catch (...) {
    bitArray = NULL;
  }
  if (bitArray == NULL)
    throw AppException (WHERE, ERR_OUT_OF_MEMORY);

  if (SetAll)
    setAll();
  else
    clearAll();
}

BitArray::~BitArray()
{
  if (bitArray)
    delete [] bitArray;
}

void BitArray::clearAll()
{
  memset(bitArray, 0, numInts * sizeof(unsigned));
}

void BitArray::setAll()
{
  memset(bitArray, 0xFF, numInts * sizeof(unsigned));
}

unsigned BitArray::setNext()
{
  for (unsigned i = 0; i < numInts; i++)
  {
    if (bitArray[i] != BITARR_ALLSET)
    {
      unsigned freebit = 0;
      for (unsigned j = 0; j < (sizeof(unsigned) * 8); j++)
      {
	unsigned msk = 1 << j;
	if (!(bitArray[i] & msk))
	{
	  freebit = j;
	  break;
	}
      }
      unsigned rval = i * (sizeof(unsigned) * 8) + freebit;
      set(rval);
      return rval;
    }
  }

  throw AppException (WHERE, ERR_NOT_FOUND);	// bitmap full!
  // return 0;
}

unsigned BitArray::clearNext()
{
  for (unsigned i = 0; i < numInts; i++)
  {
    if (bitArray[i] != 0)
    {
      unsigned freebit = 0;
      for (unsigned j = 0; j < (sizeof(unsigned) * 8); j++)
      {
	unsigned msk = 1 << j;
	if (bitArray[i] & msk)
	{
	  freebit = j;
	  break;
	}
      }
      unsigned rval = i * (sizeof(unsigned) * 8) + freebit;
      clear(rval);
      return rval;
    }
  }

  throw AppException (WHERE, ERR_NOT_FOUND);	// bitmap empty!
  // return 0;
}

bool BitArray::set(unsigned bitNumber)
{
  if (bitNumber >= numBits)
	throw AppException (WHERE, ERR_RANGE);
  bool rval = isSet(bitNumber);
  bitArray[index(bitNumber)] |= mask(bitNumber);
  return rval;
}

bool BitArray::clear(unsigned bitNumber)
{
  if (bitNumber >= numBits)
	throw AppException (WHERE, ERR_RANGE);
  bool rval = isSet(bitNumber);
  bitArray[index(bitNumber)] &= ~mask(bitNumber);
  return rval;
}

bool BitArray::isSet(unsigned bitNumber) const
{
  if (bitNumber < numBits) {
    if (bitArray[index(bitNumber)] & mask(bitNumber))
      return true;
  }
  return false;
}


// End

