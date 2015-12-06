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
#include <time.h>
#include <limf/errors.h>
#include <testutils/testRunner.h>
#include "integerTests.h"

int main (int argc, char** args, char** env) {
  ___BTPUSH;
  errors_printBacktraceFatalSignals ();
  srand (time (NULL));

  run (integerTests);

  ___BTPOP;
  return 0;
}
