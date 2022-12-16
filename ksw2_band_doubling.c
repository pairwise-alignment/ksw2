#include "ksw2.h"

#include <assert.h>
#include <stdio.h>

// EXPONENTIAL SEARCH VARIANTS
// For global alignment these run in O(ns) instead of O(n^2).
// NOTE: I'm not sure how useful these are for variants other than global alignment.
// NOTE: This requires the match score to be 0.
// TODO: Find a reasonable initial value.
// TODO: The bound of score <= q+e*w can be improved by roughly a factor two
// by using separate upper and lower bands, depending on the query and
// target length.
// The `w` argument is unused and only present for consistency with the previous functions.
void ksw_extz_band_doubling(void* km, int qlen, const uint8_t* query, int tlen,
                            const uint8_t* target, int8_t m, const int8_t* mat, int8_t q, int8_t e,
                            int _w, int zdrop, int flag, ksw_extz_t* ez) {
	assert(mat[0] == 0);
	int w = 8;
	while(1) {
		ksw_extz(km, qlen, query, tlen, target, m, mat, q, e, w, zdrop, flag, ez);
		if(-ez->score <= q + e * w) {
			return;
		}
		w *= 2;
	}
}

void ksw_extz2_sse_band_doubling(void* km, int qlen, const uint8_t* query, int tlen,
                                 const uint8_t* target, int8_t m, const int8_t* mat, int8_t q,
                                 int8_t e, int _w, int zdrop, int end_bonus, int flag,
                                 ksw_extz_t* ez) {
	assert(mat[0] == 0);
	int w = 8;
	while(1) {
		ksw_extz2_sse(km, qlen, query, tlen, target, m, mat, q, e, w, zdrop, end_bonus, flag, ez);
		if(-ez->score <= q + w * e) {
			if(-ez->score <= q + e * w) {
				return;
			}
			w *= 2;
		}
	}
}

int ksw_gg_band_doubling(void* km, int qlen, const uint8_t* query, int tlen, const uint8_t* target,
                         int8_t m, const int8_t* mat, int8_t gapo, int8_t gape, int _w,
                         int* m_cigar_, int* n_cigar_, uint32_t** cigar_) {
	assert(mat[0] == 0);
	int w = 8;
	while(1) {
		int score = ksw_gg(km, qlen, query, tlen, target, m, mat, gapo, gape, w, m_cigar_, n_cigar_,
		                   cigar_);
		if(-score <= gapo + w * gape) {
			return score;
		}
		w *= 2;
	}
}

int ksw_gg2_band_doubling(void* km, int qlen, const uint8_t* query, int tlen, const uint8_t* target,
                          int8_t m, const int8_t* mat, int8_t gapo, int8_t gape, int _w,
                          int* m_cigar_, int* n_cigar_, uint32_t** cigar_) {
	assert(mat[0] == 0);
	int w = 8;
	while(1) {
		int score = ksw_gg2(km, qlen, query, tlen, target, m, mat, gapo, gape, w, m_cigar_,
		                    n_cigar_, cigar_);
		if(-score <= gapo + w * gape) {
			return score;
		}
		w *= 2;
	}
}

// FIXME(ragnar): This has bugs.
int ksw_gg2_sse_band_doubling(void* km, int qlen, const uint8_t* query, int tlen,
                              const uint8_t* target, int8_t m, const int8_t* mat, int8_t gapo,
                              int8_t gape, int _w, int* m_cigar_, int* n_cigar_,
                              uint32_t** cigar_) {
	assert(mat[0] == 0);
	int w = 8;
	while(1) {
		int score = ksw_gg2_sse(km, qlen, query, tlen, target, m, mat, gapo, gape, w, m_cigar_,
		                        n_cigar_, cigar_);
		if(-score < gapo + w * gape) {
			return score;
		}
		w *= 2;
	}
}
