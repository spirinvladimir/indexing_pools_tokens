#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#define PROGRESS(curr, total) do { \
    int p = (curr * 40) / total; \
    printf("\r[%.*s%*s] %d%%", p, "########################################", 40-p, "", (curr*100)/total); \
    fflush(stdout); \
} while(0)

#define POOLS_COUNT 28

int token0[POOLS_COUNT];
int token1[POOLS_COUNT];

int main() {


    printf("Loading pools and it's tokens\n");
    FILE* file = fopen("p2tt.bin", "rb");
    int it;
    int ip;
    int max_it = 0;
    for (ip = 0; ip < POOLS_COUNT; ip++) {
        int _[2];
        size_t items_read = fread(_, sizeof(int32_t), 2, file);
        token0[ip] = _[0];
        token1[ip] = _[1];
        if (token0[ip] > max_it) max_it = token0[ip];
        if (token1[ip] > max_it) max_it = token1[ip];
        PROGRESS(ip, POOLS_COUNT);
    }
    PROGRESS(POOLS_COUNT, POOLS_COUNT);
    fclose(file);


    printf("\n\nCounting pools for each token\n");
    int t2ps_count[max_it + 1];
    for (it = 0; it <= max_it; it++) {
        t2ps_count[it] = 0;
        for(ip = 0; ip < POOLS_COUNT; ip++)
            if (token0[ip] == it || token1[ip] == it)
                t2ps_count[it]++;
        PROGRESS(it, max_it);
    }
    PROGRESS(max_it, max_it);

    printf("\n\nFind token with only 2 pools\n");
    FILE *f = fopen("2_pools.bin", "wb");
    int** t2ps = malloc((max_it + 1) * sizeof(int*));
    for (int A = 0; A <= max_it; A++) {
        t2ps[A] = malloc(t2ps_count[A] * sizeof(int));
        int i = 0;
        for(ip = 0; ip < POOLS_COUNT; ip++)
            if (token0[ip] == A) {
                t2ps[A][i++] = token1[ip];
                if (i > 2) break;
            } else if (token1[ip] == A) {
                t2ps[A][i++] = token0[ip];
                if (i > 2) break;
            }
        if (i == 2) {
            int B = t2ps[A][0];
            int C = t2ps[A][1];
            for(ip = 0; ip < POOLS_COUNT; ip++)
                if (
                    (token0[ip] == B && token1[ip] == C) ||
                    (token0[ip] == C && token1[ip] == B)
                ) break;
            if (ip == POOLS_COUNT) {
                fwrite(&A, sizeof(int), 1, f);
                fwrite(&B, sizeof(int), 1, f);
                fwrite(&C, sizeof(int), 1, f);
            }
        }
        PROGRESS(A, max_it);
    }
    fclose(f);
    PROGRESS(max_it, max_it);

    return 0;
}
