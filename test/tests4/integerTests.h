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

#ifndef INTEGER_TESTS_INCLUDED
#define INTEGER_TESTS_INCLUDED

struct integerTestsStruct {
  void (* testSet)(void);
  void (* testSet2)(void);
  /*
  void (* testAbsAdd)(void);
  void (* testAbsDec)(void);
  void (* testAbsInc)(void);
  void (* testAbsSub)(void);
  void (* testBsf)(void);
  void (* testBsr)(void);
  void (* testClear)(void);
  void (* testGetBit)(void);
  void (* testLshl)(void);
  void (* testRcl)(void);
  void (* testShl)(void);
  void (* testShr)(void);
  void (* testSign)(void);
  void (* testToInt)(void);
  */
};

extern const struct integerTestsStruct integerTests;

#endif
