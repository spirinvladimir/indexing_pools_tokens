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

#define POOLS_COUNT 452063

int token0[POOLS_COUNT];
int token1[POOLS_COUNT];

int get_pool(int A, int B, int* t2ps_count, int** t2ps, int** tt2p) {
    for (int i = 0; i < t2ps_count[A]; i++)
        if (tt2p[A][i] == B)
            return t2ps[A][i];
    return -1;
}

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

    int** t2ps = malloc((max_it + 1) * sizeof(int*));
    int** tt2p = malloc((max_it + 1) * sizeof(int*));

    FILE *ft2pt = fopen("t2pt.bin", "r");
    if (ft2pt) {
        printf("\n\nFile t2pt.bin exists. Loading relations for tokens...\n");
        for (it = 0; it <= max_it; it++) {
            int t2ps_count;
            fread(&t2ps_count, sizeof(int), 1, ft2pt);
            t2ps[it] = malloc(t2ps_count * sizeof(int));
            tt2p[it] = malloc(t2ps_count * sizeof(int));
            for (int i = 0; i < t2ps_count; i++) {
                fread(&t2ps[it][i], sizeof(int), 1, ft2pt);
                fread(&tt2p[it][i], sizeof(int), 1, ft2pt);
            }
            PROGRESS(it, max_it);
        }
        PROGRESS(max_it, max_it);
        fclose(ft2pt);
    } else {
        FILE* ft2pt = fopen("t2pt.bin", "wb");
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


        printf("\n\nMapping token to pools\n");
        for (it = 0; it <= max_it; it++) {
            t2ps[it] = malloc(t2ps_count[it] * sizeof(int));
            tt2p[it] = malloc(t2ps_count[it] * sizeof(int));
            int i = 0;
            fwrite(&t2ps_count[it], sizeof(int), 1, ft2pt);
            for(ip = 0; ip < POOLS_COUNT; ip++)
                if (token0[ip] == it) {
                    t2ps[it][i] = ip;
                    fwrite(&ip, sizeof(int), 1, ft2pt);
                    tt2p[it][i] = token1[ip];
                    fwrite(&token1[ip], sizeof(int), 1, ft2pt);
                    i++;
                } else if (token1[ip] == it) {
                    t2ps[it][i] = ip;
                    fwrite(&ip, sizeof(int), 1, ft2pt);
                    tt2p[it][i] = token0[ip];
                    fwrite(&token0[ip], sizeof(int), 1, ft2pt);
                    i++;
                }
            PROGRESS(it, max_it);
        }
        PROGRESS(max_it, max_it);
    }

    return 0;
}
