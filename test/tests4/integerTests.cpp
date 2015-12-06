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

static void testAbsAdd (void) {
  ___BTPUSH;
  ___BTPOP;
}

static void testAbsDec (void) {
  ___BTPUSH;
  ___BTPOP;
}

static void testAbsInc (void) {
  ___BTPUSH;
  ___BTPOP;
}

static void testAbsSub (void) {
  ___BTPUSH;
  ___BTPOP;
}

static void testBsf (void) {
  ___BTPUSH;
  ___BTPOP;
}

static void testBsr (void) {
  ___BTPUSH;
  ___BTPOP;
}

static void testClear (void) {
  ___BTPUSH;
  ___BTPOP;
}

static void testGetBit (void) {
  ___BTPUSH;
  ___BTPOP;
}

static void testLshl (void) {
  ___BTPUSH;
  ___BTPOP;
}

static void testRcl (void) {
  ___BTPUSH;
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

static void testShl (void) {
  ___BTPUSH;
  ___BTPOP;
}

static void testShr (void) {
  ___BTPUSH;
  ___BTPOP;
}

static void testSign (void) {
  ___BTPUSH;
  ___BTPOP;
}

static void testToInt (void) {
  ___BTPUSH;
  ___BTPOP;
}

const struct integerTestsStruct integerTests = {
  testSet,
  testSet2
};
