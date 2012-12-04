#include        "common.h"
#include        "graph.h"

void
help (char *cmd);

int
rcc (ugraph_struct *graph);

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
      exit (-1);
    }
  }

  g.nodenr = nodenr;
  g.linknr = linknr;
  strcpy (g.name, name);

  load_ugraph (&g);

  rcc (&g);

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
rcc (ugraph_struct *graph)
{
  int           i = -1, j = -1, k = -1;
  unsigned int  lknr = 0, maxlknr = 0;
  unsigned int  *links = NULL;
  FILE          *fout = stdout;
  long double   Phi = 0.0, Ek = 0.0, Nk = 0.0;

  for (i = 0; i < graph->nodenr; ++i) {
    lknr = graph->nodes[i].lknr;
    if (lknr > maxlknr)
      maxlknr = lknr;
  } /* for (i... */
  fprintf (fout, "max degree: %u\n", maxlknr);

  for (k = 1; k < maxlknr; ++k) {
    Ek = 0.0;
    Nk = 0.0;
    for (i = 0; i < graph->nodenr; ++i) {
      lknr = graph->nodes[i].lknr;
      if (lknr > k) {
        Nk += 1.0;
        links = get_ulinks (graph, i + 1);
        for (j = 0; j < lknr; ++j) {
          if (links[j] < i + 1) {
            continue;
          }
          unsigned int  nlknr = graph->nodes[links[j] - 1].lknr;
          if (nlknr > k) {
            Ek += 1.0;
          }
        } /* for (j... */
      } /* if (lknr > j) */
    } /* for (i... */
    Phi = (2.0 * Ek) / (Nk * (Nk - 1.0));
    fprintf (fout, "%d\t%.9Lf\t%.9Lf\t%.9Lf\n", k, Phi, Ek, Nk);
    fflush (fout);
  } /* for (k... */

  return 0;
}
