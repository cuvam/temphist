#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 40
#define MINTEMP 46
#define MAXTEMP 52

#define DIST_MINTEMP 42
#define DIST_MAXTEMP 56
#define DIST_WIDTH (DIST_MAXTEMP - DIST_MINTEMP + 1)

int main() {
    char *filename = "/path/to/thermal.log";

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open %s\n", filename);
        return 1;
    }

    int buckets[24] = {0};
    int bucketsize[24] = {0};
    int numsamples = 0;

    int otherbuckets[24][DIST_WIDTH] = {0};

    char *line = NULL;
    unsigned long len;
    long num_bytes;
    char *firsttimestamp = NULL;
    char *lasttimestamp = NULL;
    while ((num_bytes = getline(&line, &len, fp)) != EOF) {
        char timestamp[512];
        int hour;
        int minute;
        int second;
        int fan;
        int pkgtemp;
        int core0temp;
        int core1temp;
        int parsed = sscanf(line, "%511s %d:%d:%d Fan:%d Pkg:+%d.0C Core0:+%d.0C Core1:+%d.0C",
                timestamp, &hour, &minute, &second, &fan, &pkgtemp, &core0temp, &core1temp
        );
        if (parsed < 8) {
            printf("failed to parse line (%i) %s\n", parsed, line);
        } else {
            buckets[hour] += pkgtemp;
            bucketsize[hour]++;
            numsamples++;
            int idx = pkgtemp - DIST_MINTEMP;
            if (idx < 0) idx = 0;
            if (idx >= DIST_WIDTH) idx = DIST_WIDTH - 1;
            otherbuckets[hour][idx]++;
            if (firsttimestamp == NULL) {
                firsttimestamp = strdup(timestamp);
            }
            if (lasttimestamp)
                free(lasttimestamp);
            lasttimestamp = strdup(timestamp);
        }
    }
    fclose(fp);

    printf("CPU temperature distribution per hour (%i samples):\n", numsamples);
    printf("%s - %s\n", firsttimestamp, lasttimestamp);

    char brightness[] =  " .:-+#@";
    int brange = strlen(brightness);

    printf("less [");
    for (int i = 0; i < brange; i++) {
        putchar(brightness[i]);
    }
    printf("] more\n\n");

    printf("<%2d ", DIST_MINTEMP);
    for (int j = DIST_MINTEMP; j <= DIST_MAXTEMP; j++)
        printf("%d", j % 10);
    printf(" >%2d\n", DIST_MAXTEMP);

    for (int i = 0; i < 24; i++) {
        int highestBucket = 0;
        for (int j = 0; j < DIST_WIDTH; j++) {
            if (otherbuckets[i][j] > highestBucket)
                highestBucket = otherbuckets[i][j];
        }
        printf("%02i: ", i);
        if (highestBucket > 0) {
            for (int j = 0; j < DIST_WIDTH; j++) {
                putchar(brightness[(otherbuckets[i][j] * (brange - 1)) / highestBucket]);
            }
        }
        printf(" | ");
        float avg;
        if (bucketsize[i] > 0) {
            avg = (float)buckets[i] / (float)bucketsize[i];
            int scaled = ((float)(avg - MINTEMP)/(float)(MAXTEMP-MINTEMP))*WIDTH;
            if (scaled < 0) scaled = 0;
            if (scaled > WIDTH) scaled = WIDTH;
            printf("%.2fC ", avg);
            for (int j = 0; j < scaled; j++)
                putchar('#');
        }
        printf("\n");
    }
}
