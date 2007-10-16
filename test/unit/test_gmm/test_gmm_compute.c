#include <gau_cb.h>
#include <feat.h>
#include <strfuncs.h>

#include <stdio.h>
#include <math.h>

#define TEST_ASSERT(x) if (!(x)) { fprintf(stderr, "FAIL: %s\n", #x); exit(1); }

#define EPSILON 0.001
#define TEST_EQUAL(a,b) TEST_ASSERT((a) == (b))
#define TEST_EQUAL_FLOAT(a,b) TEST_ASSERT(fabs((a) - (b)) < EPSILON)

int
main(int argc, char *argv[])
{
	gau_cb_t *cb;
	mfcc_t ***feats;
	feat_t *fcb;
	int nfr;
	int best, i;
	int32 out_den[4];

	cb = gau_cb_read(NULL, HMMDIR "/means", HMMDIR "/variances", NULL);

	fcb = feat_init("1s_c_d_dd", CMN_CURRENT, FALSE, AGC_NONE, TRUE, 13);
	nfr = feat_s2mfc2feat(fcb, HMMDIR "/pittsburgh.mfc", NULL, NULL,
			      0, -1, NULL, -1);
	feats = feat_array_alloc(fcb, nfr);
	nfr = feat_s2mfc2feat(fcb, HMMDIR "/pittsburgh.mfc", NULL, NULL,
			      0, -1, feats, nfr);

	best = gau_cb_compute_all(cb, 190, 0, feats[30][0], out_den, INT_MIN);
	for (i = 0; i < 4; ++i) {
		printf("%d: %d\n", i, out_den[i]);
	}
	printf("best: %d\n", best);

	TEST_EQUAL(best, 1);
	TEST_EQUAL_FLOAT(out_den[best], -107958);

	gau_cb_free(cb);

	return 0;
}