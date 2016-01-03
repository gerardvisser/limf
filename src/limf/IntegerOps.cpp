/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2016 Gerard Visser.

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

#include <limf/IntegerOps.h>
#include "defs.h"
#include "validation.h"

#define MAX_SIZE 8192
#define MIN_SIZE 2

___CLASS (IntegerOps)

IntegerOps::IntegerOps (int size) {
  ___CBTPUSH;

  if (size < MIN_SIZE) {
    size = MIN_SIZE;
  } else if (size > MAX_SIZE) {
    size = MAX_SIZE;
  }
  m_mulResult = new Integer (2 * size);
  m_remainder = new Integer (size);
  m_numerator = new Integer (size);
  m_aux = new Integer (size);
  m_size = size;

  ___CBTPOP;
}

IntegerOps::~IntegerOps (void) {
  ___CBTPUSH;

  delete m_mulResult;
  delete m_remainder;
  delete m_numerator;
  delete m_aux;

  ___CBTPOP;
}

bool IntegerOps::add (Integer* dst, Integer* src) {
  ___CBTPUSH;
  validation_validateInteger (dst, VALIDATION_BEFORE);
  validation_validateInteger (src, VALIDATION_BEFORE);
#ifdef DEBUG_MODE
  if (!(dst->m_size == m_size & src->m_size == m_size)) {
    errors_printMessageAndExit ("The two arguments `dst' and `src' need to be of size %d.", m_size);
  }
#endif

  bool carry;
  if (src->m_max > 0) {
    if (dst->m_max > 0) {
      if (dst->m_sign ^ src->m_sign) {
        carry = dst->absSub (src);
      } else {
        carry = dst->absAdd (src);
      }
    } else {
      dst->set (src);
      carry = false;
    }
  } else {
    carry = false;
  }

  validation_validateInteger (dst, VALIDATION_AFTER);
  ___CBTPOP;
  return carry;
}

bool IntegerOps::add (Integer* dst, int value) {
  ___CBTPUSH;
  validation_validateInteger (dst, VALIDATION_BEFORE);
#ifdef DEBUG_MODE
  if (dst->m_size != m_size) {
    errors_printMessageAndExit ("Argument `dst' needs to be of size %d.", m_size);
  }
#endif

  bool carry;
#if CAL_B == 4
  if (value < -8 | value > 7) {
    m_aux->set (value);
    carry = add (dst, m_aux);
  } else {
#endif

    if (value != 0) {
      if (dst->m_max > 0) {

        bool valueSign = value < 0;
        if (valueSign) {
          value = -value;
        }
        if (dst->m_sign ^ valueSign) {

          dst->m_buf[0] -= value;
          carry = (dst->m_buf[0] & CAL_SMASK[CAL_B]) != 0;
          if (dst->m_max == 1) {
            if (carry) {
              dst->m_sign = !dst->m_sign;
              dst->m_buf[0] ^= CAL_LMASK[0];
              dst->m_buf[0] &= CAL_LMASK[0];
              ++dst->m_buf[0];
              carry = false;
            } else if (dst->m_buf[0] == 0) {
              dst->m_sign = false;
              dst->m_max = 0;
            }
          } else if (carry) {
            int i = 1;
            dst->m_buf[0] &= CAL_LMASK[0];
            while (dst->m_buf[i] == 0) {
              dst->m_buf[i] = CAL_LMASK[0];
              ++i;
            }
            --dst->m_buf[i];
            if (i == dst->m_max - 1 & dst->m_buf[i] == 0) {
              --dst->m_max;
            }
            carry = false;
          }

        } else {

          dst->m_buf[0] += value;
          carry = (dst->m_buf[0] & CAL_SMASK[CAL_B]) != 0;
          if (carry) {
            int i = 1;
            dst->m_buf[0] &= CAL_LMASK[0];
            while (i < dst->m_size && dst->m_buf[i] == CAL_LMASK[0]) {
              dst->m_buf[i] = 0;
              ++i;
            }
            carry = i == dst->m_size;
            if (carry) {
              dst->m_max = dst->m_buf[0] != 0;
            } else {
              ++dst->m_buf[i];
              if (i == dst->m_max) {
                ++dst->m_max;
              }
            }
          }

        }

      } else {
        dst->set (value);
        carry = false;
      }
    } else {
      carry = false;
    }

#if CAL_B == 4
  }
#endif

  validation_validateInteger (dst, VALIDATION_AFTER);
  ___CBTPOP;
  return carry;
}

void IntegerOps::baseDiv (Integer* result, const Integer* denominator, int denomBsr, int total) {
  ___CBTPUSH;

  const int integerWidthBits = m_size << CAL_Q; /* Hier een member van maken?  */

  while (total > 0) {
    int remainderBsr = m_remainder->bsr ();
    if (remainderBsr == 0) {
      int i = integerWidthBits - m_numerator->bsr ();
      if (i > total) {
        i = total;
      }
      result->lshl (m_numerator, i);
      total -= i;
    }
    if (total > 0) {
      int i;
      if (remainderBsr < denomBsr) {
        i = denomBsr - remainderBsr;
      } else {
        i = 1;
      }
      if (i <= total) {
        result->shl (i - 1);
        m_remainder->lshl (m_numerator, i);
        bool carry = subtractFromRemainder (denominator, denomBsr, remainderBsr + i);
        result->rcl (carry);
        total -= i;
      } else {
        result->shl (total);
        m_remainder->lshl (m_numerator, total);
        total = 0;
      }
    }
  }

  ___CBTPOP;
}

void IntegerOps::baseMul (Integer* srcA, Integer* srcB) {
  ___CBTPUSH;
  ___CBTPOP;
}

Integer* IntegerOps::createInteger (void) {
  ___CBTPUSH;

  Integer* result = new Integer (m_size);

  ___CBTPOP;
  return result;
}

bool IntegerOps::dec (Integer* dst) {
  ___CBTPUSH;
  validation_validateInteger (dst, VALIDATION_BEFORE);
#ifdef DEBUG_MODE
  if (dst->m_size != m_size) {
    errors_printMessageAndExit ("Argument `dst' needs to be of size %d.", m_size);
  }
#endif

  bool carry;
  if (dst->m_max > 0) {
    if (dst->m_sign) {
      carry = dst->absInc ();
    } else {
      carry = dst->absDec ();
    }
  } else {
    dst->m_max = 1;
    dst->m_buf[0] = 1;
    dst->m_sign = true;
    carry = false;
  }

  validation_validateInteger (dst, VALIDATION_AFTER);
  ___CBTPOP;
  return carry;
}

Integer* IntegerOps::div (Integer* dst, Integer* src) {
  ___CBTPUSH;
  ___CBTPOP;
  return m_remainder;
}

bool IntegerOps::inc (Integer* dst) {
  ___CBTPUSH;
  validation_validateInteger (dst, VALIDATION_BEFORE);
#ifdef DEBUG_MODE
  if (dst->m_size != m_size) {
    errors_printMessageAndExit ("Argument `dst' needs to be of size %d.", m_size);
  }
#endif

  bool carry;
  if (dst->m_max > 0) {
    if (dst->m_sign) {
      carry = dst->absDec ();
    } else {
      carry = dst->absInc ();
    }
  } else {
    dst->m_max = 1;
    dst->m_buf[0] = 1;
    dst->m_sign = false;
    carry = false;
  }

  validation_validateInteger (dst, VALIDATION_AFTER);
  ___CBTPOP;
  return carry;
}

Integer* IntegerOps::mul (Integer* srcA, Integer* srcB) {
  ___CBTPUSH;
  ___CBTPOP;
  return m_mulResult;
}

bool IntegerOps::sub (Integer* dst, Integer* src) {
  ___CBTPUSH;
  validation_validateInteger (dst, VALIDATION_BEFORE);
  validation_validateInteger (src, VALIDATION_BEFORE);
#ifdef DEBUG_MODE
  if (!(dst->m_size == m_size & src->m_size == m_size)) {
    errors_printMessageAndExit ("The two arguments `dst' and `src' need to be of size %d.", m_size);
  }
#endif

  bool carry;
  if (src->m_max > 0) {
    if (dst->m_max > 0) {
      if (dst->m_sign ^ src->m_sign) {
        carry = dst->absAdd (src);
      } else {
        carry = dst->absSub (src);
      }
    } else {
      dst->set (src);
      dst->m_sign = !src->m_sign;
      carry = false;
    }
  } else {
    carry = false;
  }

  validation_validateInteger (dst, VALIDATION_AFTER);
  ___CBTPOP;
  return carry;
}

/* Precondition: denomBsr ≤ remainderBsr < denomBsr + 2.  */
bool IntegerOps::subtractFromRemainder (const Integer* denominator, int denomBsr, int remainderBsr) {
  ___CBTPUSH;
  validation_validateIntegerLast0 (denominator, VALIDATION_BEFORE);
  validation_validateInteger (m_remainder, VALIDATION_BEFORE);
  validation_validateInteger (m_aux, VALIDATION_BEFORE);
#ifdef DEBUG_MODE
  if (denomBsr != denominator->bsr ()) {
    errors_printMessageAndExit ("denomBsr != denominator->bsr ().");
  }
  if (remainderBsr != m_remainder->bsr ()) {
    errors_printMessageAndExit ("remainderBsr != m_remainder->bsr ().");
  }
  if (!(remainderBsr == denomBsr | remainderBsr == denomBsr + 1)) {
    errors_printMessageAndExit ("!(remainderBsr == denomBsr | remainderBsr == denomBsr + 1).");
  }
#endif

  int i;
  bool carry = false;
  if (denomBsr == remainderBsr) {

    for (i = 0; i < denominator->m_max; ++i) {
      m_aux->m_buf[i] = m_remainder->m_buf[i];
      if (carry) {
        m_remainder->m_buf[i - 1] &= CAL_LMASK[0];
        --m_remainder->m_buf[i];
      }
      m_remainder->m_buf[i] -= denominator->m_buf[i];
      carry = (m_remainder->m_buf[i] & CAL_SMASK[CAL_B]) != 0;
    }
    if (m_aux->m_max < denominator->m_max) {
      m_aux->m_max = denominator->m_max;
    }
    if (carry) {
      /* Misschien sneller met memcpy */
      for (i = 0; i < denominator->m_max; ++i) {
        m_remainder->m_buf[i] = m_aux->m_buf[i];
      }
    } else {
      m_remainder->setMax (i - 1);
    }
    carry = !carry;

  } else {

    for (i = 0; i < denominator->m_max; ++i) {
      if (carry) {
        m_remainder->m_buf[i - 1] &= CAL_LMASK[0];
        --m_remainder->m_buf[i];
      }
      m_remainder->m_buf[i] -= denominator->m_buf[i];
      carry = (m_remainder->m_buf[i] & CAL_SMASK[CAL_B]) != 0;
    }
    if (carry) {
        m_remainder->m_buf[i - 1] &= CAL_LMASK[0];
        m_remainder->m_buf[i] = 0;
    } else {
      carry = true;
    }
    m_remainder->setMax (i - 1);

  }

  validation_validateIntegerLast0 (m_remainder, VALIDATION_AFTER);
  validation_validateInteger (m_aux, VALIDATION_AFTER);
  ___CBTPOP;
  return carry;
}
