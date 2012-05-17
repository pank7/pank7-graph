/*
 * Description:                 Some common functions.
 * File:                        common.h
 * Author:                      Li Yi <liyi@net.pku.edu.cn>
 * Date & Time:                 Tue Apr  1 16:20:48 2008
 */

#include        "common.h"

static int      read_cnt;
static char     *read_ptr;
static char     read_buf[MAXLINE];

static ssize_t
my_read (int fd, char *ptr)
{

  if (read_cnt <= 0) {
  again:
    if ((read_cnt = read (fd, read_buf, sizeof (read_buf))) < 0) {
      if (errno == EINTR)
        goto again;
      return (-1);
    } else if (read_cnt == 0)
      return (0);
    read_ptr = read_buf;
  }

  --read_cnt;
  *ptr = *read_ptr++;
  return (1);
}

static ssize_t
__read (int fd, char *ptr, int *__read_cnt, char **__read_ptr, char *__read_buf,
        size_t len)
{
  if ((*__read_cnt) <= 0) {
  again:
    if (((*__read_cnt) = read (fd, __read_buf, len)) < 0) {
      if (errno == EINTR)
        goto again;
      return (-1);
    } else if (*__read_cnt == 0) {
      return (0);
    }
    *__read_ptr = __read_buf;
  }

  --(*__read_cnt);
  *ptr = *(*__read_ptr)++;
  return (1);
}

ssize_t
read_until (int fd, void *vptr, size_t maxlen, char s)
{
  ssize_t       n, rc;
  char          c, *ptr;
  int           __read_cnt = 0;
  char          *__read_ptr = NULL;
  char          __read_buf[MAXLINE];

  ptr = vptr;
  for (n = 1; n < maxlen; ++n) {
    if ((rc = __read (fd, &c, &__read_cnt, &__read_ptr, __read_buf, MAXLINE))
        == 1) {
      *ptr++ = c;
      if (c == s)
        break;                  /* newline is stored, like fgets() */
    } else if (rc == 0) {
      *ptr = 0;
      return (n - 1);           /* EOF, n - 1 bytes were read */
    } else
      return (-1);		/* error, errno set by read() */
  }

  *ptr = 0;                     /* null terminate like fgets() */
  return (n);
}

ssize_t
readline (int fd, void *vptr, size_t maxlen)
{
  ssize_t       n, rc;
  char          c, *ptr;

  ptr = vptr;
  for (n = 1; n < maxlen; ++n) {
    if ((rc = my_read (fd, &c)) == 1) {
      *ptr++ = c;
      if (c == '\n')
        break;                  /* newline is stored, like fgets() */
    } else if (rc == 0) {
      *ptr = 0;
      return (n - 1);           /* EOF, n - 1 bytes were read */
    } else
      return (-1);		/* error, errno set by read() */
  }

  *ptr = 0;                     /* null terminate like fgets() */
  return (n);
}

ssize_t
readlinebuf (void **vptrptr)
{
  if (read_cnt)
    *vptrptr = read_ptr;
  return (read_cnt);
}
/* end readline */

ssize_t
READLINE (int fd, void *ptr, size_t maxlen)
{
  ssize_t               n;

  if ((n = readline (fd, ptr, maxlen)) < 0)
    ERROR ("readline error");
  return(n);
}
