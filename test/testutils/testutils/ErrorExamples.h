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

#ifndef TESTUTILS__ERROR_EXAMPLES_INCLUDED
#define TESTUTILS__ERROR_EXAMPLES_INCLUDED

#include <stdint.h>

class ErrorExamples {
private:
  const char* const wm_template;
  int64_t* m_data;
  const int m_dataPerExample;
  const int m_maxExamples;
  int m_exampleCount;

public:
  ErrorExamples (const char* templ, int dataPerExample, int maxExamples = 10);
  virtual ~ErrorExamples (void);

  void add (int64_t val, ...);
  void print (void);
};

#endif
