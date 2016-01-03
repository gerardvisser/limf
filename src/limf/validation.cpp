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

#ifdef DEBUG_MODE

#include "defs.h"
#include "validation.h"

#define AFTER  "[after]"
#define BEFORE "[before]"

#undef validation_validateIntegerLast0
#undef validation_validateInteger

void validation_validateInteger (const Integer* src, const char* argName, int beforeOrAfter) {
  ___BTPUSH;

  const uint64_t* srcbuf = src->buf ();
  const char* testLocation = beforeOrAfter == VALIDATION_BEFORE ? BEFORE : AFTER;
  if (src->max () > src->size ()) {
    errors_printMessageAndExit ("%s[tested=%s] integer max greater than size (max: %d, size: %d)", testLocation, argName, src->max (), src->size ());
  }
  int i = src->size () - 1;
  while (i > -1 && srcbuf[i] == 0) {
    --i;
  }
  if (i + 1 != src->max ()) {
    errors_printMessageAndExit ("%s[tested=%s] integer max corrupt (was: %d, should've been: %d)", testLocation, argName, src->max (), i + 1);
  }
  while (i > -1 && (srcbuf[i] & CAL_LMASK[0]) == srcbuf[i]) {
    --i;
  }
  if (i > -1) {
    errors_printMessageAndExit ("%s[tested=%s] one or more integer cells corrupt", testLocation, argName);
  }

  ___BTPOP;
}

void validation_validateIntegerLast0 (const Integer* src, const char* argName, int beforeOrAfter) {
  ___BTPUSH;

  const uint64_t* srcbuf = src->buf ();
  validation_validateInteger (src, argName, beforeOrAfter);
  const char* testLocation = beforeOrAfter == VALIDATION_BEFORE ? BEFORE : AFTER;
  if ((srcbuf[src->size () - 1] & CAL_SMASK[CAL_R]) != 0) {
    errors_printMessageAndExit ("%s[tested=%s] integer's most significant bit not zero", testLocation, argName);
  }

  ___BTPOP;
}

#endif
