#include        "common.h"
#include        "graph.h"

void
help (char *cmd);

int
cc (ugraph_struct *graph);

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

  cc (&g);

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

static int
in (unsigned int *links, unsigned int lknr, unsigned int node)
{
  int   left = 0, right = lknr - 1;
  int   mid = (left + right) / 2;

  do {
    if (links[mid] == node) return 1;
    else if (links[mid] < node) left = mid + 1;
    else if (links[mid] > node) right = mid;
    mid = (left + right) / 2;
  } while (left < right);

  return 0;
}

int
cc (ugraph_struct *graph)
{
  int           i = -1, j = -1, k = -1;
  unsigned int  *links = NULL, *linksj = NULL;
  unsigned int  lknr = 0, lknrj = 0, Ei = 0;
  long double   cc;
  FILE          *fout = stdout;

  for (i = 0; i < graph->nodenr; ++i) {
    cc = 0.0;
    Ei = 0;
    lknr = graph->nodes[i].lknr;
    if (lknr <= 1) {
      fprintf (fout, "%u\t%.9Lf\t0\t%u\n", graph->nodes[i].node, cc, lknr);
      continue;
    }
    links = get_ulinks (graph, i + 1);
    for (j = 0; j < lknr; ++j) {
      lknrj = graph->nodes[links[j] - 1].lknr;
      linksj = get_ulinks (graph, links[j]);
      for (k = 0; k < lknrj; ++k) {
        if (linksj[k] < links[j]) {
          continue;
        }
        if (linksj[k] > links[lknr - 1]) {
          break;
        }
        if (in (links + j, lknr - j, linksj[k])) {
          ++Ei;
        }
      } /* for (k... */
    } /* for (j... */
    cc = ((long double)Ei * 2.0) / (long double)(lknr * (lknr - 1));
    fprintf (fout, "%u\t%.9Lf\t%u\t%u\n", graph->nodes[i].node, cc, Ei, lknr);
  } /* for (i... */

  return 0;
}
