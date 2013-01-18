/* This file contains code to calculate Kendall's Tau in O(N log N) time in
 * a manner similar to the following reference:
 *
 * A Computer Method for Calculating Kendall's Tau with Ungrouped Data
 * William R. Knight Journal of the American Statistical Association, Vol. 61,
 * No. 314, Part 1 (Jun., 1966), pp. 436-439
 *
 * Copyright (C) 2010 David Simcha
 *
 * License:
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Comparator function for qsortDoubles.*/
static int qsortDoubleComp(const void* lhs, const void* rhs) {
  double lhsNum, rhsNum;
  lhsNum = *((double*) lhs);
  rhsNum = *((double*) rhs);

  if(lhsNum < rhsNum) {
    return -1;
  } else if(lhsNum > rhsNum) {
    return 1;
  } else {
    return 0;
  }
}

/* Wrapper for C's qsort functionality to simplify use in this library.*/
static void qsortDoubles(double* arr, size_t len) {
  qsort(arr, len, sizeof(double), &qsortDoubleComp);
}

typedef struct {
  double first;
  double second;
} PairOfDoubles;

/* Comparator function for zipSort().*/
static int zipSortComp(const void* lhs, const void* rhs) {
  PairOfDoubles l, r;
  l = *((PairOfDoubles*) lhs);
  r = *((PairOfDoubles*) rhs);

  if(l.first < r.first) {
    return -1;
  } else if(l.first > r.first) {
    return 1;
  } else {
    return 0;
  }
}

/* Uses C's qsort functionality to sort two arrays in lockstep, ordered by
 * the first array.
 */
static void zipSort(double* arr1, double* arr2, size_t len) {
  size_t i;

  PairOfDoubles* pairs = (PairOfDoubles*) malloc(len * sizeof(PairOfDoubles));
  for(i = 0; i < len; i++) {
    pairs[i].first = arr1[i];
    pairs[i].second = arr2[i];
  }

  qsort(pairs, len, sizeof(PairOfDoubles), &zipSortComp);

  // Copy the results back.
  for(i = 0; i < len; i++) {
    arr1[i] = pairs[i].first;
    arr2[i] = pairs[i].second;
  }

  free(pairs);
}

/* Sorts in place, returns the bubble sort distance between the input array
 * and the sorted array.
 */
static uint64_t insertionSort(double* arr, size_t len) {
  size_t maxJ, i;
  uint64_t swapCount = 0;

  if(len < 2) {
    return 0;
  }

  maxJ = len - 1;
  for(i = len - 2; i < len; --i) {
    size_t j = i;
    double val = arr[i];

    for(; j < maxJ && arr[j + 1] < val; ++j) {
      arr[j] = arr[j + 1];
    }

    arr[j] = val;
    swapCount += (j - i);
  }

  return swapCount;
}

static uint64_t merge(double* from, double* to, size_t middle, size_t len) {
  size_t bufIndex, leftLen, rightLen;
  uint64_t swaps;
  double* left;
  double* right;

  bufIndex = 0;
  swaps = 0;

  left = from;
  right = from + middle;
  rightLen = len - middle;
  leftLen = middle;

  while(leftLen && rightLen) {
    if(right[0] < left[0]) {
      to[bufIndex] = right[0];
      swaps += leftLen;
      rightLen--;
      right++;
    } else {
      to[bufIndex] = left[0];
      leftLen--;
      left++;
    }
    bufIndex++;
  }

  if(leftLen) {
    memcpy(to + bufIndex, left, leftLen * sizeof(double));
  } else if(rightLen) {
    memcpy(to + bufIndex, right, rightLen * sizeof(double));
  }

  return swaps;
}

/* Sorts in place, returns the bubble sort distance between the input array
 * and the sorted array.
 */
static uint64_t mergeSort(double* x, double* buf, size_t len) {
  uint64_t swaps;
  size_t half;

  if(len < 10) {
    return insertionSort(x, len);
  }

  swaps = 0;

  if(len < 2) {
    return 0;
  }

  half = len / 2;
  swaps += mergeSort(x, buf, half);
  swaps += mergeSort(x + half, buf + half, len - half);
  swaps += merge(x, buf, half, len);

  memcpy(x, buf, len * sizeof(double));
  return swaps;
}

static uint64_t getMs(double* data, size_t len) {  /* Assumes data is sorted.*/
  uint64_t Ms = 0, tieCount = 0;
  size_t i;

  for(i = 1; i < len; i++) {
    if(data[i] == data[i-1]) {
      tieCount++;
    } else if(tieCount) {
      Ms += (tieCount * (tieCount + 1)) / 2;
      tieCount++;
      tieCount = 0;
    }
  }
  if(tieCount) {
    Ms += (tieCount * (tieCount + 1)) / 2;
    tieCount++;
  }
  return Ms;
}

/* This function calculates the Kendall Tau B on a pair of C-style "arrays".
 * Note that it will completely overwrite arr1 and sort arr2, so these need
 * to be duplicated before passing them in.
 */
double kendallNlogN(double* arr1, double* arr2, size_t len) {
  uint64_t m1 = 0, m2 = 0, tieCount, swapCount, nPair;
  int64_t s;
  size_t i;

  zipSort(arr1, arr2, len);
  nPair = (uint64_t) len * ((uint64_t) len - 1) / 2;
  s = nPair;

  tieCount = 0;
  for(i = 1; i < len; i++) {
    if(arr1[i - 1] == arr1[i]) {
      tieCount++;
    } else if(tieCount > 0) {
      qsortDoubles(arr2 + i - tieCount - 1, tieCount + 1);
      m1 += tieCount * (tieCount + 1) / 2;
      s += getMs(arr2 + i - tieCount - 1, tieCount + 1);
      tieCount++;
      tieCount = 0;
    }
  }
  if(tieCount > 0) {
    qsortDoubles(arr2 + i - tieCount - 1, tieCount + 1);
    m1 += tieCount * (tieCount + 1) / 2;
    s += getMs(arr2 + i - tieCount - 1, tieCount + 1);
    tieCount++;
  }

  // Using arr1 as the buffer because we're done with it.
  swapCount = mergeSort(arr2, arr1, len);

  m2 = getMs(arr2, len);
  s -= (m1 + m2) + 2 * swapCount;

  double denominator1 = nPair - m1;
  double denominator2 = nPair - m2;
  double cor = s / sqrt(denominator1) / sqrt(denominator2);
  return cor;
}

/* This function uses a simple O(N^2) implementation.  It probably has a smaller
 * constant and therefore is useful in the small N case, and is also useful
 * for testing the relatively complex O(N log N) implementation.
 */
double kendallSmallN(double* arr1, double* arr2, size_t len) {
  /* Not using 64-bit ints here because this function is meant only for
     small N and for testing.
  */
  int m1 = 0, m2 = 0, s = 0, nPair;
  size_t i, j;
  double denominator1, denominator2;

  for(i = 0; i < len; i++) {
    for(j = i + 1; j < len; j++) {
      if(arr2[i] > arr2[j]) {
        if (arr1[i] > arr1[j]) {
          s++;
        } else if(arr1[i] < arr1[j]) {
          s--;
        } else {
          m1++;
        }
      } else if(arr2[i] < arr2[j]) {
        if (arr1[i] > arr1[j]) {
          s--;
        } else if(arr1[i] < arr1[j]) {
          s++;
        } else {
          m1++;
        }
      } else {
        m2++;

        if(arr1[i] == arr1[j]) {
          m1++;
        }
      }
    }
  }

  nPair = len * (len - 1) / 2;
  denominator1 = nPair - m1;
  denominator2 = nPair - m2;
  return s / sqrt(denominator1) / sqrt(denominator2);
}
