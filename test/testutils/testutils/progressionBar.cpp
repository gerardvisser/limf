/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2009, 2010, 2012, 2015 Gerard Visser.

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

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <limf/errors.h>
#include "progressionBar.h"
#include "Stopwatch.h"

#define BAR_LENGTH 50

static unsigned int errors;
static unsigned int numberOfUpdates;
static unsigned int total;
static Stopwatch* stopwatch;

static void progressionBar_drawBar (double frac);
static void progressionBar_redrawBar (int signum);

static void progressionBar_drawBar (double frac) {
  int done = (int) (BAR_LENGTH * frac + 0.5);
  printf ("\x1B[A");
  if (errors > 0) {
    printf ("\x1B[31m");
  } else {
    printf ("\x1B[32m");
  }
  int i;
  for (i = 0; i < done; ++i) {
    printf ("\342\226\210");
  }
  printf ("\x1B[0m");
  while (i < BAR_LENGTH) {
    printf ("\342\226\221");
    ++i;
  }
  frac *= 100.0;
  printf (" %5.1f %%\n", frac);
}

void progressionBar_init (const char* nameOfTest, unsigned int numberOfSubtests) {
  ___BTPUSH;

  errors = 0;
  numberOfUpdates = 0;
  total = numberOfSubtests;
  stopwatch = new Stopwatch ();
  printf ("Name: %s\n\n", nameOfTest);
  signal (SIGALRM, progressionBar_redrawBar);
  progressionBar_redrawBar (0);
  stopwatch->start ();

  ___BTPOP;
}

static void progressionBar_redrawBar (int signum) {
  if (numberOfUpdates < total) {
    progressionBar_drawBar ((double) numberOfUpdates / total);
    alarm (1);
  }
}

void progressionBar_update (bool subtestFailed) {
  ___BTPUSH;

  ++numberOfUpdates;
  if (subtestFailed) {
    ++errors;
  }
  if (numberOfUpdates == total) {
    char time[64];
    stopwatch->stop ();
    alarm (0);
    stopwatch->print (time);
    progressionBar_drawBar (1.0);
    printf ("Successful: %u (%.2f %%), Errors: %u (%.2f %%)\nTime: %s\n",
            total - errors,
            100.0 * (total - errors) / total,
            errors,
            100.0 * errors / total,
            time);
    if (errors == 0) {
      printf ("\n");
    }
    delete stopwatch;
  }

  ___BTPOP;
}
