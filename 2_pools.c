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
    for (it = 0; it <= max_it; it++) {
        t2ps[it] = malloc(t2ps_count[it] * sizeof(int));
        int i = 0;
        for(ip = 0; ip < POOLS_COUNT; ip++)
            if (token0[ip] == it) {
                t2ps[it][i++] = ip;
                if (i > 2) break;
            } else if (token1[ip] == it) {
                t2ps[it][i++] = ip;
                if (i > 2) break;
            }
        if (i == 2) {
            fwrite(&t2ps[it][0], sizeof(int), 1, f);
            fwrite(&t2ps[it][1], sizeof(int), 1, f);
        }
        PROGRESS(it, max_it);
    }
    fclose(f);
    PROGRESS(max_it, max_it);

    return 0;
}
