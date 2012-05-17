#include        "common.h"
#include        "graph.h"
#include        "list.h"

void
help (char *cmd);

int
da (ugraph_struct *graph);

int
main (int argc, char **argv)
{
  int           opt;
  unsigned int  nodenr = 0, linknr = 0;
  ugraph_struct g;
  char          name[32];

  if (argc <= 1) {
    help (argv[0]);
    exit (-1);
  }

  while ((opt = getopt (argc, argv, "N:n:l:h")) != -1) {
    switch (opt) {
    case 'N':
      strcpy (name, optarg);
      break;
    case 'n':
      nodenr = atoi (optarg);
      break;
    case 'l':
      linknr = atoi (optarg);
      break;
    case 'h':
    default:
      help (argv[0]);
    }
  }

  g.nodenr = nodenr;
  g.linknr = linknr;
  strcpy (g.name, name);

  load_ugraph (&g);

  da (&g);

  delete_ugraph (&g);

  return 0;
}

void
help (char *cmd)
{
  fprintf (stderr, "usage:\n");
  fprintf (stderr, "\t%s -N <name> -n <nodenr> -l <linknr>\n", cmd);

  return;
}

int
da (ugraph_struct *graph)
{
  int           i = -1, j = -1;
  unsigned int  *links = NULL;
  unsigned long du, dv, jiki = 0, jipluski = 0, ji2pluski2 = 0;
  unsigned int  linknr = 0;
  long double   r = 0.0, tmp = 0.0, numerator = 0.0, denominator = 0.0;

  for (i = 0; i < graph->nodenr; ++i) {
    du = graph->nodes[i].lknr;
    links = get_ulinks (graph, i + 1);
    for (j = 0; j < du; ++j) {
      if (links[j] < i + 1) {
        continue;
      }
      ++linknr;
      dv = graph->nodes[links[j] - 1].lknr;
      jiki += (du * dv);
      jipluski += (du + dv);
      ji2pluski2 += (du * du + dv * dv);
    }
  }

  tmp = (long double)jipluski / (2.0 * (long double)linknr);
  tmp *= tmp;
  numerator = ((long double)jiki / (long double)linknr) - tmp;
  denominator = ((long double)ji2pluski2 / (2.0 * (long double)linknr)) - tmp;
  r = numerator / denominator;

  fprintf (stdout, "r = %0.9Lf, linknr = %u\n", r, linknr);

  return 0;
}
