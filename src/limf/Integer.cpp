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
  validation_validateInteger (this, VALIDATION_BEFORE);
  validation_validateInteger (other, VALIDATION_BEFORE);
#ifdef DEBUG_MODE
  if (m_size != other->m_size) {
    errors_printMessageAndExit ("this->m_size should be equal to other->m_size.");
  }
#endif

  bool otherMaxGreaterThanThisMax = other->m_max > m_max;
  int max = otherMaxGreaterThanThisMax ? m_max : other->m_max;
  bool carry = false;
  int i;

  for (i = 0; i < max; ++i) {
    if (carry) {
      m_buf[i - 1] &= CAL_LMASK[0];
      ++m_buf[i];
    }
    m_buf[i] += other->m_buf[i];
    carry = (m_buf[i] & CAL_SMASK[CAL_B]) != 0;
  }

  if (otherMaxGreaterThanThisMax) {

    if (carry) {
      m_buf[i - 1] &= CAL_LMASK[0];
      while (i < other->m_max && other->m_buf[i] == CAL_LMASK[0]) {
        ++i;
      }
      carry = i == m_size;
      if (carry) {
        setMax (m_max - 1);
      } else {
        m_buf[i] = other->m_buf[i] + 1;
        ++i;
        while (i < other->m_max) {
          m_buf[i] = other->m_buf[i];
          ++i;
        }
        m_max = i;
      }
    } else {
      while (i < other->m_max) {
        m_buf[i] = other->m_buf[i];
        ++i;
      }
      m_max = i;
    }

  } else if (carry) {

    m_buf[i - 1] &= CAL_LMASK[0];
    while (i < m_max && m_buf[i] == CAL_LMASK[0]) {
      m_buf[i] = 0;
      ++i;
    }
    carry = i == m_size;
    if (carry) {
      setMax (other->m_max - 1);
    } else {
      ++m_buf[i];
      if (i == m_max) {
        ++m_max;
      }
    }

  }

  validation_validateInteger (this, VALIDATION_AFTER);
  ___CBTPOP;
  return carry;
}

bool Integer::absDec (void) {
  ___CBTPUSH;
  validation_validateInteger (this, VALIDATION_BEFORE);
#ifdef DEBUG_MODE
  if (m_max == 0) {
    errors_printMessageAndExit ("This function may never be called when m_max is 0.");
  }
#endif

  int i = 0;
  while (m_buf[i] == 0) {
    m_buf[i] = CAL_LMASK[0];
    ++i;
  }
  --m_buf[i];
  if (m_buf[i] == 0 && i == m_max - 1) {
    --m_max;
    if (m_max == 0) {
      m_sign = false;
    }
  }

  validation_validateInteger (this, VALIDATION_AFTER);
  ___CBTPOP;
  return false;
}

bool Integer::absInc (void) {
  ___CBTPUSH;
  validation_validateInteger (this, VALIDATION_BEFORE);

  int i = 0;
  while (i < m_size && m_buf[i] == CAL_LMASK[0]) {
    m_buf[i] = 0;
    ++i;
  }
  bool carry = i == m_size;
  if (carry) {
    m_max = 0;
  } else {
    ++m_buf[i];
    if (i == m_max) {
      ++m_max;
    }
  }

  validation_validateInteger (this, VALIDATION_AFTER);
  ___CBTPOP;
  return carry;
}

bool Integer::absSub (Integer* other) {
  ___CBTPUSH;
  validation_validateInteger (this, VALIDATION_BEFORE);
  validation_validateInteger (other, VALIDATION_BEFORE);
#ifdef DEBUG_MODE
  if (m_size != other->m_size) {
    errors_printMessageAndExit ("this->m_size should be equal to other->m_size.");
  }
#endif

  int i;
  bool carry = false;

  if (m_max > other->m_max) {

    for (i = 0; i < other->m_max; ++i) {
      if (carry) {
        m_buf[i - 1] &= CAL_LMASK[0];
        --m_buf[i];
      }
      m_buf[i] -= other->m_buf[i];
      carry = (m_buf[i] & CAL_SMASK[CAL_B]) != 0;
    }
    if (carry) {
      m_buf[i - 1] &= CAL_LMASK[0];
      while (m_buf[i] == 0) {
        m_buf[i] = CAL_LMASK[0];
        ++i;
      }
      --m_buf[i];
      if (i == m_max-1 && m_buf[i] == 0) {
        setMax (i - 1);
      }
    }

  } else if (m_max == other->m_max) {

    for (i = 0; i < m_max; ++i) {
      if (carry) {
        m_buf[i - 1] &= CAL_LMASK[0];
        --m_buf[i];
      }
      m_buf[i] -= other->m_buf[i];
      carry = (m_buf[i] & CAL_SMASK[CAL_B]) != 0;
    }
    if (carry) {
      i = 0;
      m_sign = !m_sign;
      while (m_buf[i] == 0) {
        ++i;
      }
      m_buf[i] ^= CAL_LMASK[0];
      m_buf[i] &= CAL_LMASK[0];
      ++m_buf[i];
      ++i;
      while (i < m_max) {
        m_buf[i] ^= CAL_LMASK[0];
        m_buf[i] &= CAL_LMASK[0];
        ++i;
      }
    }
    setMax (i - 1);
    if (m_max == 0) {
      m_sign = false;
    }

  } else {

    m_sign = !m_sign;
    for (i = 0; i < m_max; ++i) {
      if (carry) {
        m_buf[i - 1] &= CAL_LMASK[0];
        ++m_buf[i];
      }
      m_buf[i] = other->m_buf[i] - m_buf[i];
      carry = (m_buf[i] & CAL_SMASK[CAL_B]) != 0;
    }
    m_max = other->m_max;
    if (carry) {
      m_buf[i - 1] &= CAL_LMASK[0];
      while (other->m_buf[i] == 0) {
        m_buf[i] = CAL_LMASK[0];
        ++i;
      }
      m_buf[i] = other->m_buf[i] - 1;
      if (i == other->m_max - 1) {
        if (m_buf[i] == 0) {
          setMax (i - 1);
        }
      } else {
        ++i;
        while (i < other->m_max) {
          m_buf[i] = other->m_buf[i];
          ++i;
        }
      }
    } else {
      while (i < other->m_max) {
        m_buf[i] = other->m_buf[i];
        ++i;
      }
    }

  }

  validation_validateInteger (this, VALIDATION_AFTER);
  ___CBTPOP;
  return false;
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
  validation_validateInteger (this, VALIDATION_BEFORE);

  int r = bitNo & CAL_R;
  int q = bitNo >> CAL_Q;
#ifdef DEBUG_MODE
  if (q >= m_size) {
    errors_printMessageAndExit ("Argument bitNo too large.");
  }
#endif
  bool result = (m_buf[q] & CAL_SMASK[r]) != 0;

  ___CBTPOP;
  return result;
}

void Integer::lshl (Integer* incomingBits, int x) {
  ___CBTPUSH;
  validation_validateInteger (this, VALIDATION_BEFORE);
  validation_validateInteger (incomingBits, VALIDATION_BEFORE);
#ifdef DEBUG_MODE
  if (m_size != incomingBits->m_size) {
    errors_printMessageAndExit ("m_size should be equal to incomingBits->m_size.");
  }
#endif

  if (x > 0) {
    const int n = m_size - 1;
    const int q = x >> CAL_Q;
    const int r = x & CAL_R;
    int ih, il, zh, zl;

    if (r > 0) {

      const int k = CAL_B - r;
      if (m_max > n - q) {
        zh = n;
      } else {
        zh = q + m_max;
      }

      if (m_max > 0) {
        for (ih = zh; ih > q; --ih) {
          m_buf[ih] = m_buf[ih - q] << r;
          m_buf[ih] |= m_buf[ih - q - 1] >> k;
          m_buf[ih] &= CAL_RMASK[CAL_B];
        }
        m_buf[q] = m_buf[0] << r;
        m_buf[q] &= CAL_RMASK[CAL_B];
        --ih;
      } else {
        ih = -1;
      }

      if (incomingBits->m_max > 0) {

        il = n;
        m_buf[q] |= incomingBits->m_buf[n] >> k;
        for (ih = q - 1; ih > -1; --ih) {
          m_buf[ih] = incomingBits->m_buf[il] << r;
          --il;
          m_buf[ih] |= incomingBits->m_buf[il] >> k;
          m_buf[ih] &= CAL_RMASK[CAL_B];
        }

        if (incomingBits->m_max > n - q) {
          zl = n;
        } else {
          zl = q + incomingBits->m_max;
        }
        for (il = zl; il > q; --il) {
          incomingBits->m_buf[il] = incomingBits->m_buf[il - q] << r;
          incomingBits->m_buf[il] |= incomingBits->m_buf[il - q - 1] >> k;
          incomingBits->m_buf[il] &= CAL_RMASK[CAL_B];
        }
        incomingBits->m_buf[q] = incomingBits->m_buf[0] << r;
        incomingBits->m_buf[q] &= CAL_RMASK[CAL_B];
        --il;

        while (il > -1) {
          incomingBits->m_buf[il] = 0;
          --il;
        }
        incomingBits->setMax (zl);

      } else {

        while (ih > -1) {
          m_buf[ih] = 0;
          --ih;
        }

      }

    } else { /* r = 0 */

      if (m_max > 0) {
        if (m_max > n - q) {
          zh = n;
        } else {
          zh = q + m_max - 1;
        }
        for (ih = zh; ih >= q; --ih) {
          m_buf[ih] = m_buf[ih - q];
        }
      } else {
        ih = -1;
        zh = q - 1;
      }

      if (incomingBits->m_max > 0) {

        il = n;
        for (ih = q - 1; ih > -1; --ih) {
          m_buf[ih] = incomingBits->m_buf[il];
          --il;
        }

        if (incomingBits->m_max > n - q) {
          zl = n;
        } else {
          zl = q + incomingBits->m_max - 1;
        }
        for (il = zl; il >= q; --il) {
          incomingBits->m_buf[il] = incomingBits->m_buf[il - q];
        }

        while (il > -1) {
          incomingBits->m_buf[il] = 0;
          --il;
        }
        incomingBits->setMax (zl);

      } else {

        while (ih > -1) {
          m_buf[ih] = 0;
          --ih;
        }

      }

    }
    setMax (zh);
  }

  validation_validateInteger (incomingBits, VALIDATION_AFTER);
  validation_validateInteger (this, VALIDATION_AFTER);
  ___CBTPOP;
}

void Integer::rcl (bool carry) {
  ___CBTPUSH;
  validation_validateIntegerLast0 (this, VALIDATION_BEFORE);

  if (m_max > 0) {

    const int m = m_max - 1;
    if ((m_buf[m] & CAL_SMASK[CAL_R]) != 0) {
      m_buf[m_max] = 1;
      ++m_max;
    }
    int i;
    for (i = m; i > 0; --i) {
      m_buf[i] <<= 1;
      m_buf[i] &= CAL_LMASK[0];
      m_buf[i] |= (m_buf[i - 1] & CAL_SMASK[CAL_R]) != 0;
    }
    m_buf[0] <<= 1;
    m_buf[0] &= CAL_LMASK[0];
    m_buf[0] |= carry;

  } else if (carry) {

    m_buf[0] = 1;
    m_max = 1;

  }

  validation_validateInteger (this, VALIDATION_AFTER);
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
  validation_validateInteger (this, VALIDATION_BEFORE);

  if (m_max > 0 & x > 0) {
    const int n = m_size - 1;
    const int q = x >> CAL_Q;
    const int r = x & CAL_R;
    int i, z;

    if (r > 0) {

      const int k = CAL_B - r;
      if (m_max > n - q) {
        z = n;
      } else {
        z = q + m_max;
      }
      for (i = z; i > q; --i) {
        m_buf[i] = m_buf[i - q] << r;
        m_buf[i] |= m_buf[i - q - 1] >> k;
        m_buf[i] &= CAL_RMASK[CAL_B];
      }
      m_buf[q] = m_buf[0] << r;
      m_buf[q] &= CAL_RMASK[CAL_B];
      --i;

    } else {

      if (m_max > n - q) {
        z = n;
      } else {
        z = q + m_max - 1;
      }
      for (i = z; i >= q; --i) {
        m_buf[i] = m_buf[i - q];
      }

    }

    while (i > -1) {
      m_buf[i] = 0;
      --i;
    }
    setMax (z);
  }

  validation_validateInteger (this, VALIDATION_AFTER);
  ___CBTPOP;
}

void Integer::shr (int x) {
  ___CBTPUSH;
  validation_validateInteger (this, VALIDATION_BEFORE);

  if (m_max > 0 & x > 0) {
    const int oldMax = m_max;
    const int q = x >> CAL_Q;
    const int r = x & CAL_R;
    int z = m_max - q;
    int i;

    if (z > 0) {

      if (r > 0) {

        --z;
        const int k = CAL_B - r;
        for (i = 0; i < z; ++i) {
          m_buf[i] = m_buf[i + q] >> r;
          m_buf[i] |= m_buf[i + q + 1] << k;
          m_buf[i] &= CAL_RMASK[CAL_B];
        }
        m_buf[z] = m_buf[z + q] >> r;
        if (m_buf[z] > 0) {
          m_max = z + 1;
        } else {
          m_max = z;
        }
        ++i;

      } else {

        for (i = 0; i < z; ++i) {
          m_buf[i] = m_buf[i + q];
        }
        m_max = z;

      }
      while (i < oldMax) {
        m_buf[i] = 0;
        ++i;
      }

    } else {

      for (i = 0; i < oldMax; ++i) {
        m_buf[i] = 0;
      }
      m_max = 0;

    }
  }

  validation_validateInteger (this, VALIDATION_AFTER);
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
