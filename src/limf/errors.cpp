/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2010 - 2015 Gerard Visser.

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

#include "errors.h"

#ifdef DEBUG_MODE

# include <pthread.h>
# include <signal.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define BTSTACK_SIZE          (1024)
# define MAX_THREADS           (64)
# define NAMES_MAX_MEAN_LENGTH (256)
# define STRING_BUF_SIZE       (BTSTACK_SIZE * (NAMES_MAX_MEAN_LENGTH + 4))

# define removeCallStack(index) \
  --threadsIndex; \
  memmove (btsp + index, btsp + index + 1, sizeof (int) * (threadsIndex - index)); \
  memmove (threads + index, threads + index + 1, sizeof (pthread_t) * (threadsIndex - index)); \
  memmove (btstack + BTSTACK_SIZE * index, btstack + BTSTACK_SIZE * (index + 1), sizeof (void*) * BTSTACK_SIZE * (threadsIndex - index))

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static int threadsIndex = 0;
static pthread_t threads[MAX_THREADS];

static int btsp[MAX_THREADS];
static const char* btstack[MAX_THREADS * BTSTACK_SIZE];

static char strbuf[STRING_BUF_SIZE];

static int getThreadIndex (void);

static int addStringToBacktrace (const char* str, int strbufoff) {
  int i;

  i = 0;
  while (str[i] != 0) {
    strbuf[strbufoff] = str[i];
    ++strbufoff;
    ++i;
  }
  return strbufoff;
}

static int createBacktrace (int strbufoff) {
  int index = getThreadIndex ();
  const char** cstack = btstack + BTSTACK_SIZE * index;
  while (btsp[index] < BTSTACK_SIZE) {
    strbuf[strbufoff] = ' ';
    ++strbufoff;

    strbufoff = addStringToBacktrace (cstack[btsp[index]], strbufoff);
    ++btsp[index];

    strbuf[strbufoff] = ':';
    ++strbufoff;

    strbuf[strbufoff] = ' ';
    ++strbufoff;

    strbufoff = addStringToBacktrace (cstack[btsp[index]], strbufoff);
    ++btsp[index];

    strbuf[strbufoff] = '\n';
    ++strbufoff;
  }
  strbuf[strbufoff] = 0;
  return strbufoff;
}

/* PRE: mutex is locked. */
static int getThreadIndex (void) {
  int index = 0;
  pthread_t thread = pthread_self ();
  while (index < threadsIndex && !pthread_equal (thread, threads[index])) {
    ++index;
  }
  if (index == threadsIndex) {
    if (index == MAX_THREADS) {
      pthread_mutex_unlock (&mutex);
      errors_printMessageAndExit ("Maximum number of threads reached (%d)", MAX_THREADS);
    }
    btsp[index] = BTSTACK_SIZE;
    threads[index] = thread;
    ++threadsIndex;
  }
  return index;
}

static void handleFatalSignals (int sig) {
  errors_printMessageAndExit (strsignal (sig));
}

static void printBacktraceAndExit (void) {
  if (createBacktrace (0) > 0) {
    printf ("Backtrace of the current stack:\n%s\n", strbuf);
  }
  exit (EXIT_FAILURE);
}

void errors_pop (void) {
  pthread_mutex_lock (&mutex);

  int index = getThreadIndex ();
  switch (btsp[index]) {
  case BTSTACK_SIZE:
    pthread_mutex_unlock (&mutex);
    errors_printMessageAndExit ("Call stack empty");
    break;

  case BTSTACK_SIZE - 2:
    removeCallStack (index);
    break;

  default:
    btsp[index] += 2;
  }

  pthread_mutex_unlock (&mutex);
}

void errors_printBacktraceFatalSignals (void) {
  signal (SIGABRT, handleFatalSignals);
  signal (SIGBUS, handleFatalSignals);
  signal (SIGILL, handleFatalSignals);
  signal (SIGSEGV, handleFatalSignals);
}

void errors_printMessageAndExit (const char* message, ...) {
  va_list argp;

  pthread_mutex_lock (&mutex); /* Unlocking is not needed anymore since the thread owning the mutex here, will call exit. */
  if (!(message==NULL || message[0]==0)) {
    va_start (argp, message);
    vsprintf (strbuf, message, argp);
    va_end (argp); /* ANSI C requirement. */
    printf("\x1B[31;1mERROR:\n%s\x1B[0m\n", strbuf);
  } else {
    printf("\x1B[31;1mERROR\x1B[0m\n");
  }
  printBacktraceAndExit ();
}

void errors_push (const char* fileName, const char* functionName) {
  pthread_mutex_lock (&mutex);
  int index = getThreadIndex ();
  const char** cstack = btstack + BTSTACK_SIZE * index;
  if (btsp[index] > 1) {
    --btsp[index];
    cstack[btsp[index]] = functionName;
    --btsp[index];
    cstack[btsp[index]] = fileName;
    pthread_mutex_unlock (&mutex);
  } else {
    pthread_mutex_unlock (&mutex);
    errors_printMessageAndExit ("Stack overflow (max. number of entries = %d, see 'errors.cpp' to change)", BTSTACK_SIZE / 2);
  }
}

#endif
