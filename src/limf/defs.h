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

#ifndef LIMF__DEFS_INCLUDED
#define LIMF__DEFS_INCLUDED

#include <stdint.h>

#ifdef DEBUG_MODE
# define CAL_B 4
# define CAL_Q 2
#else
# define CAL_B 32
# define CAL_Q 5
#endif

#define CAL_R (CAL_B - 1)

extern const uint64_t CAL_LMASK[CAL_B+1];
extern const uint64_t CAL_RMASK[CAL_B+1];
extern const uint64_t CAL_SMASK[CAL_B+1];

#endif
