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
#include <stdio.h>
#include <limf/errors.h>
#include <limf/Integer.h>
#include <testutils/ErrorExamples.h>
#include <testutils/progressionBar.h>
#include "integerTests.h"

#define RANDOM_BIT        (RANDOM_DOUBLE > 0.5)
#define RANDOM_DOUBLE     (rand () / (RAND_MAX + 1.0))
#define RANDOM_INT(bound) ((int) (bound * RANDOM_DOUBLE))

class TestInteger {
private:
  char m_buffer[8];
  bool m_sign;

public:
  TestInteger (void) {
  }

  ~TestInteger (void) {
  }

  bool equals (Integer* n) {
    if (n->sign () != m_sign) {
      return false;
    }
    const int size = n->size ();
    const uint64_t* buf = n->buf ();
    int i = 0;
    while (i < size && buf[i] == m_buffer[i]) {
      ++i;
    }
    return i == size;
  }

  void set (int value) {
    m_sign = value < 0;
    if (m_sign) {
      value = -value;
    }
    for (int i = 0; i < 8; ++i) {
      m_buffer[i] = value & 0xF;
      value >>= 4;
    }
  }
};

static int bsf (int value) {
  if (value != 0) {
    int mask = 1;
    int result = 0;
    while ((value & mask) == 0) {
      mask <<= 1;
      ++result;
    }
    return result;
  } else {
    return 0;
  }
}

static int bsr (int value) {
  if (value != 0) {
    int result = 32;
    int mask = 0x80000000;
    while ((value & mask) == 0) {
      mask >>= 1;
      --result;
    }
    return result;
  } else {
    return 0;
  }
}

static int64_t randomBits (int count) {
  int64_t result = 0;
  for (int i = 0; i < count; ++i) {
    result <<= 1;
    result |= RANDOM_BIT;
  }
  return result;
}

static void testAbsAdd (void) {
  ___BTPUSH;

  const int max = 4096;
  Integer* bigintA = new Integer (7);
  Integer* bigintB = new Integer (7);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: bigintA = %ld (valA = 0x%07lX),  bigintB = %ld (valB = 0x%07lX)\n", 4);
  progressionBar_init ("Integer::absAdd", 28 * 28 * max);
  for (int i = 1; i <= 28; ++i) {
    for (int j = 1; j <= 28; ++j) {
      for (int k = 0; k < max; ++k) {
        int64_t valA = randomBits (i);
        int64_t valB = randomBits (j);
        int64_t signA = valA != 0 ? RANDOM_BIT ? -1 : 1 : 1;
        int64_t signB = RANDOM_BIT ? -1 : 1;
        bigintA->set (signA * valA);
        bigintB->set (signB * valB);

        int64_t sum = valA + valB;
        bool carryExpected = (sum & 0x10000000) != 0;
        sum &= 0xFFFFFFF;
        bool carry = bigintA->absAdd (bigintB);

        bool error = bigintA->sign () != signA < 0;
        error |= bigintB->toInt () != signB * valB;
        error |= bigintA->toInt () != signA * sum;
        error |= carry != carryExpected;

        if (error) {
          errorExamples->add (signA * valA, valA, signB * valB, valB);
        }
        progressionBar_update (error);
      }
    }
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigintB;
  delete bigintA;

  ___BTPOP;
}

static void testAbsDec (void) {
  ___BTPUSH;

  int i;
  const int max = 0xFFFF;
  Integer* bigint = new Integer (4);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: %ld\n", 1);
  progressionBar_init ("Integer::absDec", max);
  for (i = 1; i <= max; ++i) {
    int64_t sign = RANDOM_BIT ? -1 : 1;
    bigint->set (sign * i);

    bool carry = bigint->absDec ();
    int64_t expected = sign * (i - 1);

    bool error = bigint->sign () != expected < 0;
    error |= bigint->toInt () != expected;
    error |= carry;

    if (error) {
      errorExamples->add (sign * i);
    }
    progressionBar_update (error);
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigint;

  ___BTPOP;
}

static void testAbsInc (void) {
  ___BTPUSH;

  int i;
  const int max = 0x10000;
  Integer* bigint = new Integer (4);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: %ld\n", 1);
  progressionBar_init ("Integer::absInc", max);
  for (i = 0; i < max; ++i) {
    int64_t sign = i != 0 ? RANDOM_BIT ? -1 : 1 : 1;
    bigint->set (sign * i);

    bool carry = bigint->absInc ();
    int64_t expected = sign * (i + 1 & 0xFFFF);

    bool error = bigint->sign () != sign < 0;
    error |= bigint->toInt () != expected;
    error |= carry != (i == 0xFFFF);

    if (error) {
      errorExamples->add (sign * i);
    }
    progressionBar_update (error);
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigint;

  ___BTPOP;
}

static void testAbsSub (void) {
  ___BTPUSH;

  const int max = 4096;
  Integer* bigintA = new Integer (7);
  Integer* bigintB = new Integer (7);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: bigintA = %ld (valA = 0x%07lX),  bigintB = %ld (valB = 0x%07lX)\n", 4);
  progressionBar_init ("Integer::absSub", 28 * 28 * max);
  for (int i = 1; i <= 28; ++i) {
    for (int j = 1; j <= 28; ++j) {
      for (int k = 0; k < max; ++k) {
        int64_t valA = randomBits (i);
        int64_t valB = randomBits (j);
        int64_t signA = valA != 0 ? RANDOM_BIT ? -1 : 1 : 1;
        int64_t signB = RANDOM_BIT ? -1 : 1;
        bigintA->set (signA * valA);
        bigintB->set (signB * valB);

        int64_t diff = valA - valB;
        int signResult = diff <= 0 ? diff < 0 ? -signA : 1 : signA;
        bool carry = bigintA->absSub (bigintB);

        bool error = bigintA->sign () != signResult < 0;
        error |= bigintB->toInt () != signB * valB;
        error |= bigintA->toInt () != signA * diff;
        error |= carry;

        if (error) {
          errorExamples->add (signA * valA, valA, signB * valB, valB);
        }
        progressionBar_update (error);
      }
    }
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigintB;
  delete bigintA;

  ___BTPOP;
}

static void testBsf (void) {
  ___BTPUSH;

  int i;
  const int max = 0x100000;
  Integer* bigint = new Integer (5);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: %ld\n", 1);
  progressionBar_init ("Integer::bsf", max);
  for (i = 0; i < max; ++i) {
    bigint->set (i);
    bool error = bigint->bsf () != bsf (i);
    if (error) {
      errorExamples->add (i);
    }
    progressionBar_update (error);
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigint;

  ___BTPOP;
}

static void testBsr (void) {
  ___BTPUSH;

  int i;
  const int max = 0x100000;
  Integer* bigint = new Integer (5);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: %ld\n", 1);
  progressionBar_init ("Integer::bsr", max);
  for (i = 0; i < max; ++i) {
    bigint->set (i);
    bool error = bigint->bsr () != bsr (i);
    if (error) {
      errorExamples->add (i);
    }
    progressionBar_update (error);
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigint;

  ___BTPOP;
}

static void testClear (void) {
  ___BTPUSH;

  int i;
  const int max = 0x10000;
  TestInteger testInt;
  testInt.set (0);
  Integer* bigint = new Integer (5);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: %ld\n", 1);
  progressionBar_init ("Integer::clear", max);
  for (i = 0; i < max; ++i) {
    int val = (rand () % 0x1FFFFF) - 0xFFFFF;
    bigint->set (val);

    bigint->clear ();

    bool error = !testInt.equals (bigint);
    if (error) {
      errorExamples->add (val);
    }
    progressionBar_update (error);
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigint;

  ___BTPOP;
}

static void testGetBit (void) {
  ___BTPUSH;

  int i;
  const int max = 0x10000;
  Integer* bigint = new Integer (15);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: 0x%015lX, bitNo: %ld\n", 2);
  progressionBar_init ("Integer::getBit", max);
  for (i = 0; i < max; ++i) {
    int bitNo = rand () % 60;
    int64_t mask = 1L << bitNo;
    int64_t val = randomBits (60);
    bigint->set (val);

    bool expected = (val & mask) != 0;
    bool actual = bigint->getBit (bitNo);

    bool error = actual != expected;
    if (error) {
      errorExamples->add (val, (int64_t) bitNo);
    }
    progressionBar_update (error);
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigint;

  ___BTPOP;
}

static void testLshl (void) {
  ___BTPUSH;

  int i;
  const int max = 0x1800000;
  Integer* bigintA = new Integer (5);
  Integer* bigintB = new Integer (5);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: bigintA = 0x%05lX,  bigintB = 0x%05lX, bits to shift: %ld\n", 3);
  progressionBar_init ("Integer::lshl", max);
  for (i = 0; i < max; ++i) {
    int x = RANDOM_INT (20) + 1;
    int64_t valA = RANDOM_INT (0x100000);
    int64_t valB = RANDOM_INT (0x100000);
    bigintA->set (valA);
    bigintB->set (valB);

    int64_t expected = valA << 20 | valB;
    expected <<= x;
    expected &= 0xFFFFFFFFFF;

    bigintA->lshl (bigintB, x);
    int64_t actual = (int64_t) bigintA->toInt () << 20 | bigintB->toInt ();

    bool error = actual != expected;
    if (error) {
      errorExamples->add (valA, valB, (int64_t) x);
    }
    progressionBar_update (error);
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigintB;
  delete bigintA;

  ___BTPOP;
}

static void testRcl (void) {
  ___BTPUSH;

  int i, carry;
  const int max = 0x100000;
  Integer* bigint = new Integer (5);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: 0x%05lX, carry: %ld\n", 2);
  progressionBar_init ("Integer::rcl", max);
  for (carry = 0; carry < 2; ++carry) {
    for (i = 0; i < 0x80000; ++i) {
      bigint->set (i);

      int expected = i << 1 | carry;
      bigint->rcl (carry);

      bool error = bigint->toInt () != expected;
      if (error) {
        errorExamples->add (i, (int64_t) carry);
      }
      progressionBar_update (error);
    }
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigint;

  ___BTPOP;
}

static void testSet (void) {
  ___BTPUSH;

  int i;
  const int max = 0x400000;
  TestInteger testInt;
  Integer* bigint = new Integer (5);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: %ld\n", 1);
  progressionBar_init ("Integer::set(int64_t)", max);
  for (i = 0; i < max; ++i) {
    int val = (rand () % 0x1FFFFF) - 0xFFFFF;

    testInt.set (val);
    bigint->set (val);

    bool error = !testInt.equals (bigint);
    if (error) {
      errorExamples->add (val);
    }
    progressionBar_update (error);
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigint;

  ___BTPOP;
}

static void testSet2 (void) {
  ___BTPUSH;

  int i;
  const int max = 0x400000;
  TestInteger testInt;
  Integer* bigintA = new Integer (5);
  Integer* bigintB = new Integer (5);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: %ld\n", 1);
  progressionBar_init ("Integer::set(Integer*)", max);
  for (i = 0; i < max; ++i) {
    int val = (rand () % 0x1FFFFF) - 0xFFFFF;

    testInt.set (val);
    bigintA->set (val);
    bigintB->set (bigintA);

    bool error = !testInt.equals (bigintB);
    if (error) {
      errorExamples->add (val);
    }
    progressionBar_update (error);
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigintB;
  delete bigintA;

  ___BTPOP;
}

/* TODO: Wat als aantal te verschuiven bits veel groter is dan aantal bits van het getal zelf?  */
static void testShl (void) {
  ___BTPUSH;

  int i;
  const int max = 0x200000;
  Integer* bigint = new Integer (7);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: 0x%07lX, bits to shift: %ld\n", 2);
  progressionBar_init ("Integer::shl", max);
  for (i = 0; i < max; ++i) {
    int x = rand () % 29;
    int64_t val = randomBits (28);
    bigint->set (val);

    bigint->shl (x);
    int64_t expected = val << x & 0xFFFFFFF;
    int64_t actual = bigint->toInt ();

    bool error = actual != expected;
    if (error) {
      errorExamples->add (val, (int64_t) x);
    }
    progressionBar_update (error);
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigint;

  ___BTPOP;
}

static void testShr (void) {
  ___BTPUSH;

  int i;
  const int max = 0x200000;
  Integer* bigint = new Integer (7);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: 0x%07lX, bits to shift: %ld\n", 2);
  progressionBar_init ("Integer::shr", max);
  for (i = 0; i < max; ++i) {
    int x = rand () % 29;
    int64_t val = randomBits (28);
    bigint->set (val);

    bigint->shr (x);
    int64_t expected = val >> x;
    int64_t actual = bigint->toInt ();

    bool error = actual != expected;
    if (error) {
      errorExamples->add (val, (int64_t) x);
    }
    progressionBar_update (error);
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigint;

  ___BTPOP;
}

static void testToInt (void) {
  ___BTPUSH;

  int i;
  const int max = 0x100000;
  Integer* bigint = new Integer (9);
  ErrorExamples* errorExamples = new ErrorExamples ("Error for: %ld\n", 1);
  progressionBar_init ("Integer::toInt", 2 * max);
  for (i = 0; i < max; ++i) {
    int64_t val = i;
    if (RANDOM_BIT) {
      val = -val;
    }
    bigint->set (val);

    bool error = bigint->toInt () != val;
    if (error) {
      errorExamples->add (val);
    }
    progressionBar_update (error);
  }
  for (i = 0; i < max; ++i) {
    int64_t val = randomBits (32);
    if (RANDOM_BIT) {
      val = -val;
    }
    bigint->set (val);

    bool error;
    if (val < -0x7FFFFFFF | val > 0x7FFFFFFF) {
      error = bigint->toInt () != 0x80000000;
    } else {
      error = bigint->toInt () != val;
    }
    if (error) {
      errorExamples->add (val);
    }
    progressionBar_update (error);
  }
  errorExamples->print ();

  delete errorExamples;
  delete bigint;

  ___BTPOP;
}

const struct integerTestsStruct integerTests = {
  testSet,
  testSet2,
  testBsf,
  testBsr,
  testClear,
  testToInt,
  testGetBit,
  testShl,
  testShr,
  testAbsAdd,
  testAbsDec,
  testAbsInc,
  testAbsSub,
  testLshl,
  testRcl
};
