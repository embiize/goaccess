/**
 * gstorage.c -- common storage handling
 *    ______      ___
 *   / ____/___  /   | _____________  __________
 *  / / __/ __ \/ /| |/ ___/ ___/ _ \/ ___/ ___/
 * / /_/ / /_/ / ___ / /__/ /__/  __(__  |__  )
 * \____/\____/_/  |_\___/\___/\___/____/____/
 *
 * The MIT License (MIT)
 * Copyright (c) 2009-2016 Gerardo Orellana <hello @ goaccess.io>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#if !defined __SUNPRO_C
#include <stdint.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "gstorage.h"

#include "error.h"
#include "xmalloc.h"

/* Allocate memory for a new GMetrics instance.
 *
 * On success, the newly allocated GMetrics is returned . */
GMetrics *
new_gmetrics (void)
{
  GMetrics *metrics = xcalloc (1, sizeof (GMetrics));

  return metrics;
}

/* Allocate space off the heap to store an int.
 *
 * On success, the newly allocated pointer is returned . */
int *
int2ptr (int val)
{
  int *ptr = xmalloc (sizeof (int));
  *ptr = val;

  return ptr;
}

/* Allocate space off the heap to store a uint64_t.
 *
 * On success, the newly allocated pointer is returned . */
uint64_t *
uint642ptr (uint64_t val)
{
  uint64_t *ptr = xmalloc (sizeof (uint64_t));
  *ptr = val;

  return ptr;
}

/* Wrapper to set metric data into a new GMetrics destination buffer. */
void
set_data_metrics (GMetrics * ometrics, GMetrics ** nmetrics, int valid)
{
  GMetrics *metrics;
  float percent = get_percentage (valid, ometrics->hits);

  metrics = new_gmetrics ();
  metrics->bw.nbw = ometrics->bw.nbw;
  metrics->id = ometrics->id;
  metrics->data = ometrics->data;
  metrics->hits = ometrics->hits;
  metrics->percent = percent < 0 ? 0 : percent;
  metrics->visitors = ometrics->visitors;

  if (conf.serve_usecs && ometrics->hits > 0) {
    metrics->avgts.nts = ometrics->avgts.nts;
    metrics->cumts.nts = ometrics->cumts.nts;
    metrics->maxts.nts = ometrics->maxts.nts;
  }

  if (conf.append_method && ometrics->method)
    metrics->method = ometrics->method;

  if (conf.append_protocol && ometrics->protocol)
    metrics->protocol = ometrics->protocol;

  *nmetrics = metrics;
}
