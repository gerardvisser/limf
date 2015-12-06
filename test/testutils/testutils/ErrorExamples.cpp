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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limf/errors.h>
#include "ErrorExamples.h"

#define MAX_DATA_PER_EXAMPLE 4

___CLASS (ErrorExamples)

ErrorExamples::ErrorExamples (const char* templ, int dataPerExample, int maxExamples) : wm_template (templ), m_dataPerExample (dataPerExample), m_maxExamples (maxExamples) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (dataPerExample > MAX_DATA_PER_EXAMPLE) {
    errors_printMessageAndExit ("dataPerExample too large.");
  }
#endif

  m_exampleCount = 0;
  m_data = (int64_t*) malloc (sizeof (int64_t) * dataPerExample * maxExamples);

  ___CBTPOP;
}

ErrorExamples::~ErrorExamples (void) {
  ___CBTPUSH;

  free (m_data);

  ___CBTPOP;
}

void ErrorExamples::add (int64_t val, ...) {
  ___CBTPUSH;

  if (m_exampleCount < m_maxExamples) {
    int index = m_dataPerExample * m_exampleCount;
    m_data[index++] = val;
    if (m_dataPerExample > 1) {
      va_list argp;
      va_start (argp, val);
      for (int i = 1; i < m_dataPerExample; ++i) {
        m_data[index++] = va_arg (argp, int64_t);
      }
      va_end (argp);
    }
    ++m_exampleCount;
  }

  ___CBTPOP;
}

void ErrorExamples::print (void) {
  ___CBTPUSH;

  if (m_exampleCount > 0) {
    switch (m_dataPerExample) {
    case 1:
      for (int i = 0; i < m_exampleCount; ++i) {
        printf (wm_template, m_data[i]);
      }
      break;

    case 2:
      for (int i = 0; i < m_exampleCount; ++i) {
        int j = m_dataPerExample * i;
        printf (wm_template, m_data[j], m_data[j + 1]);
      }
      break;

    case 3:
      for (int i = 0; i < m_exampleCount; ++i) {
        int j = m_dataPerExample * i;
        printf (wm_template, m_data[j], m_data[j + 1], m_data[j + 2]);
      }
      break;

    case 4:
      for (int i = 0; i < m_exampleCount; ++i) {
        int j = m_dataPerExample * i;
        printf (wm_template, m_data[j], m_data[j + 1], m_data[j + 2], m_data[j + 3]);
      }
      break;

    default:
      printf ("ErrorExamples::print: cannot print error examples.\n");
      exit (EXIT_FAILURE);
      break;
    }
    printf ("\n");
  }

  ___CBTPOP;
}
