#include "os.h"
#include <mp.h>
#include "dat.h"

// diff = jehanne_abs(b1) - jehanne_abs(b2), i.e., subtract the magnitudes
void
mpmagsub(mpint *b1, mpint *b2, mpint *diff)
{
	int n, m, sign;
	mpint *t;

	// get the sizes right
	if(mpmagcmp(b1, b2) < 0){
		assert(((b1->flags | b2->flags | diff->flags) & MPtimesafe) == 0);
		sign = -1;
		t = b1;
		b1 = b2;
		b2 = t;
	} else {
		diff->flags |= (b1->flags | b2->flags) & MPtimesafe;
		sign = 1;
	}
	n = b1->top;
	m = b2->top;
	if(m == 0){
		mpassign(b1, diff);
		diff->sign = sign;
		return;
	}
	mpbits(diff, n*Dbits);

	mpvecsub(b1->p, n, b2->p, m, diff->p);
	diff->sign = sign;
	diff->top = n;
	mpnorm(diff);
}

// diff = b1 - b2
void
mpsub(mpint *b1, mpint *b2, mpint *diff)
{
	int sign;

	if(b1->sign != b2->sign){
		assert(((b1->flags | b2->flags | diff->flags) & MPtimesafe) == 0);
		sign = b1->sign;
		mpmagadd(b1, b2, diff);
		diff->sign = sign;
		return;
	}

	sign = b1->sign;
	mpmagsub(b1, b2, diff);
	if(diff->top != 0)
		diff->sign *= sign;
}
