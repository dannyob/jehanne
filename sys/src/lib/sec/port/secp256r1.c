/* this file has been autogenerated in Jehanne
 * using a port of 9front mpc tool:
 * 
 *      echo '#include "os.h"' > secp256r1.c
 *      echo '#include <mp.h>' >> secp256r1.c
 *      mpc secp256r1.mp >> secp256r1.c
 */
#include "os.h"
#include <mp.h>
void secp256r1(mpint *p, mpint *a, mpint *b, mpint *x, mpint *y, mpint *n, mpint *h){
	strtomp("3FFFFFFFFFFFF0000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000003FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", nil, 16, p);
	uitomp(3UL, a);
	mpsub(p, a, a);
	mpassign(mpzero, b);
	mpassign(mpzero, x);
	mpassign(mpzero, y);
	mpassign(mpzero, n);
	mpassign(mpone, h);
	}
