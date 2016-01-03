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

#ifndef LIMF__INTEGER_OPS_INCLUDED
#define LIMF__INTEGER_OPS_INCLUDED

#include <limf/Integer.h>

class IntegerOps {
private:
  Integer* m_mulResult;
  Integer* m_remainder;
  Integer* m_numerator;
  Integer* m_aux;
  int m_size;

public:
  IntegerOps (int size);
  virtual ~IntegerOps (void);

  bool add (Integer* dst, Integer* src);
  bool add (Integer* dst, int value);
  Integer* createInteger (void);
  bool dec (Integer* dst);
  Integer* div (Integer* dst, Integer* src);
  bool inc (Integer* dst);
  Integer* mul (Integer* srcA, Integer* srcB);
  bool sub (Integer* dst, Integer* src);

private:
  void baseDiv (Integer* result, const Integer* denominator, int denomBsr, int total);
  void baseMul (Integer* srcA, Integer* srcB);
  bool subtractFromRemainder (const Integer* denominator, int denomBsr, int remainderBsr);
};

#endif
