/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2015 Gerard Visser.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <stdlib.h>
#include <string.h>
#include <limf/Integer.h>
#include "defs.h"
#include "validation.h"

#define MAX_SIZE 16384
#define MIN_SIZE 2

___CLASS (Integer)

Integer::Integer (int size) : m_size (size), m_bsize (size << 3) , m_max (0), m_sign (false) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (size < MIN_SIZE | size > MAX_SIZE) {
    errors_printMessageAndExit ("The specified size (%d) is out of range. It should be: %d < size < %d.", size, MIN_SIZE - 1, MAX_SIZE + 1);
  }
#endif

  m_buf = (uint64_t*) malloc (m_bsize);
  memset (m_buf, 0, m_bsize);

  ___CBTPOP;
}

Integer::~Integer (void) {
  ___CBTPUSH;

  free (m_buf);

  ___CBTPOP;
}

bool Integer::absAdd (Integer* other) {
  ___CBTPUSH;
  ___CBTPOP;
}

bool Integer::absDec (void) {
  ___CBTPUSH;
  ___CBTPOP;
}

bool Integer::absInc (void) {
  ___CBTPUSH;
  ___CBTPOP;
}

bool Integer::absSub (Integer* other) {
  ___CBTPUSH;
  ___CBTPOP;
}

int Integer::bsf (void) {
  ___CBTPUSH;

  validation_validateInteger (this, VALIDATION_BEFORE);
  int result = 0;
  if (m_max > 0) {
    int i = 0;
    int64_t mask = 1;
    while (m_buf[i] == 0) {
      ++i;
    }
    while ((m_buf[i] & mask) == 0) {
      mask <<= 1;
      ++result;
    }
    result += CAL_B * i;
  }

  ___CBTPOP;
  return result;
}

int Integer::bsr (void) {
  ___CBTPUSH;

  int result;
  validation_validateInteger (this, VALIDATION_BEFORE);
  if (m_max > 0) {
    int m = m_max - 1;
    int64_t mask = CAL_SMASK[CAL_B - 1];
    result = CAL_B * m_max;
    while ((m_buf[m] & mask) == 0) {
      mask >>= 1;
      --result;
    }
  } else {
    result = 0;
  }

  ___CBTPOP;
  return result;
}

void Integer::clear (void) {
  ___CBTPUSH;

  validation_validateInteger (this, VALIDATION_BEFORE);
  memset (m_buf, 0, m_bsize); /* Is dit goed genoeg? */
  m_sign = false;
  m_max = 0;

  ___CBTPOP;
}

bool Integer::getBit (int bitNo) {
  ___CBTPUSH;
  ___CBTPOP;
}

void Integer::lshl (Integer* incomingBits, int x) {
  ___CBTPUSH;
  ___CBTPOP;
}

void Integer::rcl (bool carry) {
  ___CBTPUSH;
  ___CBTPOP;
}

void Integer::set (Integer* other) {
  ___CBTPUSH;
  validation_validateInteger (this, VALIDATION_BEFORE);
  validation_validateInteger (other, VALIDATION_BEFORE);
#ifdef DEBUG_MODE
  if (m_size < other->m_max) {
    errors_printMessageAndExit ("The other integer is too large.");
  }
#endif

  memcpy (m_buf, other->m_buf, other->m_max << 3);
  if (m_max > other->m_max) {
    memset (m_buf + other->m_max, 0, m_max - other->m_max << 3);
  }
  m_sign = other->m_sign;
  m_max = other->m_max;

  validation_validateInteger (this, VALIDATION_AFTER);
  ___CBTPOP;
}

void Integer::set (int64_t val) {
  ___CBTPUSH;
  validation_validateInteger (this, VALIDATION_BEFORE);

  m_sign = val < 0;
  if (m_sign) {
    val = -val;
  }

#ifdef DEBUG_MODE
  if (val >> CAL_B * m_size > 0) {
    errors_printMessageAndExit ("The magnitude of argument `val' is too large.");
  }
#endif

  int i = 0;
  while (val != 0) {
    m_buf[i] = val & CAL_LMASK[0];
    val >>= CAL_B;
    ++i;
  }
  if (i < m_max) {
    memset (m_buf + i, 0, m_max - i << 3);
  }
  m_max = i;

  validation_validateInteger (this, VALIDATION_AFTER);
  ___CBTPOP;
}

void Integer::setMax (int fromIndex) {
  ___CBTPUSH;

  while (fromIndex > -1 && m_buf[fromIndex] == 0) {
    --fromIndex;
  }
  m_max = fromIndex + 1;
  validation_validateInteger (this, VALIDATION_AFTER);

  ___CBTPOP;
}

void Integer::shl (int x) {
  ___CBTPUSH;
  ___CBTPOP;
}

void Integer::shr (int x) {
  ___CBTPUSH;
  ___CBTPOP;
}

bool Integer::sign (void) {
  return m_sign;
}

int Integer::toInt (void) {
  ___CBTPUSH;
  validation_validateInteger (this, VALIDATION_BEFORE);
  int result;

#if CAL_B == 32

  if (m_max < 2 & (m_buf[0] & CAL_SMASK[31]) == 0) {
    result = (int) m_buf[0];

#else

  if (bsr () < 32) {
    int i;
    result = 0;
    for (i = m_max - 1; i > -1; --i) {
      result <<= CAL_B;
      result |= (int) m_buf[i];
    }

#endif

    if (m_sign) {
      result = -result;
    }
  } else {
    result = 0x80000000;
  }

  ___CBTPOP;
  return result;
}

#ifdef DEBUG_MODE

const uint64_t* Integer::buf (void) {
  return m_buf;
}

int Integer::max (void) {
  return m_max;
}

int Integer::size (void) {
  return m_size;
}

#endif
