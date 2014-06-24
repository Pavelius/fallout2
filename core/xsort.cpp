//Copyright 2014 by Pavel Chistyakov
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

// Always compile this module for speed, not size
#pragma optimize("t", on)

#define CUTOFF 8
#define STKSIZ (8*sizeof(void*) - 2)

static void swap(char *a, char *b, unsigned width)
{
	char tmp;

	if(a != b)
		/* Do the swap one character at a time to avoid potential alignment
		problems. */
	while(width--)
	{
		tmp = *a;
		*a++ = *b;
		*b++ = tmp;
	}
}

static void shortsort(char *lo, char *hi, unsigned width, int(*comp)(const void *, const void *, void*), void* param)
{
	char *p, *max;

	/* Note: in assertions below, i and j are alway inside original bound of
	array to sort. */

	while(hi > lo)
	{
		/* A[i] <= A[j] for i <= j, j > hi */
		max = lo;
		for(p = lo + width; p <= hi; p += width)
		{
			/* A[i] <= A[max] for lo <= i < p */
			if(comp(p, max, param) > 0)
				max = p;
			/* A[i] <= A[max] for lo <= i <= p */
		}

		/* A[i] <= A[max] for lo <= i <= hi */

		swap(max, hi, width);

		/* A[i] <= A[hi] for i <= hi, so A[i] <= A[j] for i <= j, j >= hi */

		hi -= width;

		/* A[i] <= A[j] for i <= j, j > hi, loop top condition established */
	}
	/* A[i] <= A[j] for i <= j, j > lo, which implies A[i] <= A[j] for i < j,
	so array is sorted */
}

void xsort(void *base, int num, int width, int(*comp)(const void *, const void *, void*), void* param)
{
	/* Note: the number of stack entries required is no more than
	1 + log2(num), so 30 is sufficient for any array */
	char *lo, *hi;              /* ends of sub-array currently sorting */
	char *mid;                  /* points to middle of subarray */
	char *loguy, *higuy;        /* traveling pointers for partition step */
	int size;                /* size of the sub-array */
	char *lostk[STKSIZ], *histk[STKSIZ];
	int stkptr;                 /* stack for saving sub-array to be processed */

	if(num < 2)
		return;                 /* nothing to do */

	stkptr = 0;                 /* initialize stack */

	lo = (char*)base;
	hi = (char*)base + width*(num - 1);        /* initialize limits */

	/* this entry point is for pseudo-recursion calling: setting
	lo and hi and jumping to here is like recursion, but stkptr is
	preserved, locals aren't, so we preserve stuff on the stack */

recurse:

	size = (hi - lo) / width + 1;        /* number of el's to sort */

	/* below a certain size, it is faster to use a O(n^2) sorting method */
	if(size <= CUTOFF)
		shortsort(lo, hi, width, comp, param);
	else
	{
		/* First we pick a partitioning element.  The efficiency of the
		algorithm demands that we find one that is approximately the median
		of the values, but also that we select one fast.  We choose the
		median of the first, middle, and last elements, to avoid bad
		performance in the face of already sorted data, or data that is made
		up of multiple sorted runs appended together.  Testing shows that a
		median-of-three algorithm provides better performance than simply
		picking the middle element for the latter case. */

		mid = lo + (size / 2)*width;      /* find middle element */

		/* Sort the first, middle, last elements into order */
		if(comp(lo, mid, param) > 0)
			swap(lo, mid, width);
		if(comp(lo, hi, param) > 0)
			swap(lo, hi, width);
		if(comp(mid, hi, param) > 0)
			swap(mid, hi, width);

		/* We now wish to partition the array into three pieces, one consisting
		of elements <= partition element, one of elements equal to the
		partition element, and one of elements > than it.  This is done
		below; comments indicate conditions established at every step. */

		loguy = lo;
		higuy = hi;

		/* Note that higuy decreases and loguy increases on every iteration,
		so loop must terminate. */
		for(;;)
		{
			/* lo <= loguy < hi, lo < higuy <= hi,
			A[i] <= A[mid] for lo <= i <= loguy,
			A[i] > A[mid] for higuy <= i < hi,
			A[hi] >= A[mid] */

			/* The doubled loop is to avoid calling comp(mid,mid), since some
			existing comparison funcs don't work when passed the same
			value for both pointers. */

			if(mid > loguy)
			{
				do
				{
					loguy += width;
				} while(loguy < mid && comp(loguy, mid, param) <= 0);
			}
			if(mid <= loguy)
			{
				do
				{
					loguy += width;
				} while(loguy <= hi && comp(loguy, mid, param) <= 0);
			}

			/* lo < loguy <= hi+1, A[i] <= A[mid] for lo <= i < loguy,
			either loguy > hi or A[loguy] > A[mid] */

			do
			{
				higuy -= width;
			} while(higuy > mid && comp(higuy, mid, param) > 0);

			/* lo <= higuy < hi, A[i] > A[mid] for higuy < i < hi,
			either higuy == lo or A[higuy] <= A[mid] */

			if(higuy < loguy)
				break;

			/* if loguy > hi or higuy == lo, then we would have exited, so
			A[loguy] > A[mid], A[higuy] <= A[mid],
			loguy <= hi, higuy > lo */

			swap(loguy, higuy, width);

			/* If the partition element was moved, follow it.  Only need
			to check for mid == higuy, since before the swap,
			A[loguy] > A[mid] implies loguy != mid. */

			if(mid == higuy)
				mid = loguy;

			/* A[loguy] <= A[mid], A[higuy] > A[mid]; so condition at top
			of loop is re-established */
		}

		/*     A[i] <= A[mid] for lo <= i < loguy,
		A[i] > A[mid] for higuy < i < hi,
		A[hi] >= A[mid]
		higuy < loguy
		implying:
		higuy == loguy-1
		or higuy == hi - 1, loguy == hi + 1, A[hi] == A[mid] */

		/* Find adjacent elements equal to the partition element.  The
		doubled loop is to avoid calling comp(mid,mid), since some
		existing comparison funcs don't work when passed the same value
		for both pointers. */

		higuy += width;
		if(mid < higuy)
		{
			do
			{
				higuy -= width;
			} while(higuy > mid && comp(higuy, mid, param) == 0);
		}
		if(mid >= higuy)
		{
			do
			{
				higuy -= width;
			} while(higuy > lo && comp(higuy, mid, param) == 0);
		}

		/* OK, now we have the following:
		higuy < loguy
		lo <= higuy <= hi
		A[i]  <= A[mid] for lo <= i <= higuy
		A[i]  == A[mid] for higuy < i < loguy
		A[i]  >  A[mid] for loguy <= i < hi
		A[hi] >= A[mid] */

		/* We've finished the partition, now we want to sort the subarrays
		[lo, higuy] and [loguy, hi].
		We do the smaller one first to minimize stack usage.
		We only sort arrays of length 2 or more.*/

		if(higuy - lo >= hi - loguy)
		{
			if(lo < higuy)
			{
				lostk[stkptr] = lo;
				histk[stkptr] = higuy;
				++stkptr;
			}                           /* save big recursion for later */

			if(loguy < hi)
			{
				lo = loguy;
				goto recurse;           /* do small recursion */
			}
		}
		else
		{
			if(loguy < hi)
			{
				lostk[stkptr] = loguy;
				histk[stkptr] = hi;
				++stkptr;               /* save big recursion for later */
			}

			if(lo < higuy)
			{
				hi = higuy;
				goto recurse;           /* do small recursion */
			}
		}
	}

	/* We have sorted the array, except for any pending sorts on the stack.
	Check if there are any, and do them. */

	--stkptr;
	if(stkptr >= 0)
	{
		lo = lostk[stkptr];
		hi = histk[stkptr];
		goto recurse;           /* pop subarray from stack */
	}
	else
		return;                 /* all subarrays done */
}

void* xfind(const void *key, const void *base0, int nmemb, int size, int(*compar)(const void*, const void*, void*), void* param)
{
	const char* base = (const char*)base0;
	unsigned lim;
	int cmp;
	const void *p;
	for(lim = nmemb; lim != 0; lim >>= 1)
	{
		p = base + (lim >> 1) * size;
		cmp = (*compar)(key, p, param);
		if(cmp == 0)
			return ((void*)p);
		if(cmp > 0)
		{	/* key > p: move right */
			base = (char*)p + size;
			lim--;
		}		/* else move left */
	}
	return 0;
}