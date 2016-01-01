/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2015, 2016 Gerard Visser.

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

#ifndef LIMF__INTEGER_INCLUDED
#define LIMF__INTEGER_INCLUDED

#include <stdint.h>

class Integer {
private:
  uint64_t* m_buf;
  const int m_size;
  int m_max;
  bool m_sign;

public:
  ~Integer (void);

  int bsf (void);
  int bsr (void);
  void clear (void);
  bool getBit (int bitNo);
  void set (Integer* other);
  void set (int64_t val);
  void shl (int x);
  void shr (int x);
  bool sign (void);
  int toInt (void);

#ifdef DEBUG_MODE
  const uint64_t* buf (void);
  int max (void);
  int size (void);
#else
private:
#endif
  Integer (int size);

  bool absAdd (Integer* other);
  bool absDec (void);
  bool absInc (void);
  bool absSub (Integer* other);
  void lshl (Integer* incomingBits, int x);
  void rcl (bool carry);
  void setMax (int fromIndex);

  friend class IntegerOps;
};

#endif
