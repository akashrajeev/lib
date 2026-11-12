#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define IN_FIFO "sentences_fifo"
#define OUT_FIFO "result_fifo"

static int prepare_fifos(void) {
    if (mkfifo(IN_FIFO, 0666) < 0 && errno != EEXIST) { perror("mkfifo input"); return -1; }
    if (mkfifo(OUT_FIFO, 0666) < 0 && errno != EEXIST) { perror("mkfifo output"); return -1; }
    return 0;
}

int main(void) {
    char buf[256];
    long chars=0, words=0, lines=0;
    int in_word=0, has_data=0, last_newline=1;
    if (prepare_fifos() < 0) return 1;
    int fd = open(IN_FIFO, O_RDONLY);
    if (fd < 0) { perror("open input"); return 1; }
    ssize_t n;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        for (ssize_t i=0; i<n; i++) {
            unsigned char c = (unsigned char)buf[i];
            chars++;
            if (isspace(c)) in_word=0;
            else if (!in_word) { words++; in_word=1; }
            if (c == '\n') lines++;
            last_newline = (c == '\n');
            has_data=1;
        }
    }
    close(fd);
    if (n < 0) { perror("read input"); return 1; }
    if (has_data && !last_newline) lines++;
    FILE *fp = fopen("count_report.txt", "w");
    if (!fp) { perror("count_report.txt"); return 1; }
    fprintf(fp,"Characters: %ld\nWords: %ld\nLines: %ld\n", chars, words, lines);
    fclose(fp);
    fp = fopen("count_report.txt", "r");
    if (!fp) { perror("open report"); return 1; }
    fd = open(OUT_FIFO, O_WRONLY);
    if (fd < 0) { perror("open result"); fclose(fp); return 1; }
    size_t bytes;
    while ((bytes=fread(buf,1,sizeof(buf),fp))>0) {
        size_t sent=0;
        while (sent<bytes) {
            ssize_t k=write(fd,buf+sent,bytes-sent);
            if (k<0) { perror("write result"); fclose(fp); close(fd); return 1; }
            sent+=(size_t)k;
        }
    }
    fclose(fp);
    close(fd);
    puts("Report saved to count_report.txt and sent to writer.");
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define IN_FIFO "sentences_fifo"
#define OUT_FIFO "result_fifo"

static int prepare_fifos(void) {
    if (mkfifo(IN_FIFO, 0666) < 0 && errno != EEXIST) { perror("mkfifo input"); return -1; }
    if (mkfifo(OUT_FIFO, 0666) < 0 && errno != EEXIST) { perror("mkfifo output"); return -1; }
    return 0;
}

int main(void) {
    char line[256], buf[256];
    ssize_t n;
    if (prepare_fifos() < 0) return 1;
    printf("Enter sentences. Type @ alone on a line to finish:\n");
    fflush(stdout);
    int fd = open(IN_FIFO, O_WRONLY);
    if (fd < 0) { perror("open input"); return 1; }
    while (fgets(line, sizeof(line), stdin)) {
        if (!strcmp(line, "@\n") || !strcmp(line, "@")) break;
        size_t len = strlen(line), sent = 0;
        while (sent < len) {
            ssize_t k = write(fd, line + sent, len - sent);
            if (k < 0) { perror("write input"); close(fd); return 1; }
            sent += (size_t)k;
        }
    }
    close(fd);
    fd = open(OUT_FIFO, O_RDONLY);
    if (fd < 0) { perror("open result"); return 1; }
    printf("\nReport received from reader:\n");
    while ((n = read(fd, buf, sizeof(buf))) > 0)
        fwrite(buf, 1, (size_t)n, stdout);
    if (n < 0) perror("read result");
    close(fd);
    return n < 0 ? 1 : 0;
}
