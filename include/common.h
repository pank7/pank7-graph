/*
 * Description:                 Some common defines.
 * File:                        common.h
 * Author:                      Li Yi <liyi@net.pku.edu.cn>
 * Date & Time:                 Thu Mar 27 21:45:05 2008
 */

#ifndef COMMON_H
#define COMMON_H

#include        <stdio.h>
#include        <unistd.h>
#include        <stdlib.h>
#include        <sys/types.h>
#include        <sys/stat.h>
#include        <fcntl.h>
#include        <time.h>
#include        <errno.h>
#include        <string.h>
#include        <assert.h>

typedef enum _mem_status {
  UNINIT,
  INMEMO,
  ONDISK
} mem_status;

#define         TXTPOSTFIX      "txt"
#define         BINPOSTFIX      "bin"
#define         MAXLINE         4096

#define OUTPUT(msg)                                                     \
  do {                                                                  \
    time_t      t = time (&t);                                          \
    char        ct[128];                                                \
    ctime_r (&t, ct);                                                   \
    ct[strlen (ct) - 1] = '\0';                                         \
    fprintf (stdout, "[OUTPUT - %s]: %s\n", ct, #msg);                  \
  } while (0)

#define WARNING(msg)                                                    \
  do {                                                                  \
    time_t      t = time (&t);                                          \
    char        ct[128];                                                \
    ctime_r (&t, ct);                                                   \
    ct[strlen (ct) - 1] = '\0';                                         \
    fprintf (stderr, "[WARNING - %s]: %s\n", ct, #msg);                 \
  } while (0)

#define ERROR(msg)                                                      \
  do {                                                                  \
    time_t      t = time (&t);                                          \
    char        ct[128];                                                \
    ctime_r (&t, ct);                                                   \
    ct[strlen (ct) - 1] = '\0';                                         \
    fprintf (stderr, "[ERROR - %s]: %s\n", ct, #msg);                   \
  } while (0)

#define FATALERROR(msg)                                                 \
  do {                                                                  \
    time_t      t = time (&t);                                          \
    char        ct[128];                                                \
    ctime_r (&t, ct);                                                   \
    ct[strlen (ct) - 1] = '\0';                                         \
    fprintf (stderr, "[FATAL ERROR - %s]: %s\nQUIT!\n", ct, #msg);      \
    exit (-1);                                                          \
  } while (0)

ssize_t
readline (int fd, void *vptr, size_t maxlen);

ssize_t
read_until (int fd, void *vptr, size_t maxlen, char s);

ssize_t
READLINE (int fd, void *ptr, size_t maxlen);

#endif  /* COMMON_H */
