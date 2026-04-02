#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/ecomem.h"
#include "user/user.h"

// Print an integer right-aligned in a field of width w.
static void
printint(int val, int width)
{
  char buf[16];
  int i = 0;
  int neg = 0;
  uint v;

  if(val < 0){
    neg = 1;
    v = -val;
  } else {
    v = val;
  }
  do {
    buf[i++] = '0' + (v % 10);
    v /= 10;
  } while(v > 0);
  if(neg) buf[i++] = '-';

  int pad = width - i;
  while(pad-- > 0) printf(" ");
  while(--i >= 0) printf("%c", buf[i]);
}

static void
printstr(const char *s, int width)
{
  int len = strlen(s);
  printf("%s", s);
  int pad = width - len;
  while(pad-- > 0) printf(" ");
}

int
main(int argc, char *argv[])
{
  struct ecomem_proc *procs;
  struct ecomem_sys sys;
  int max = 16;

  procs = (struct ecomem_proc *)malloc(max * sizeof(struct ecomem_proc));
  if(procs == 0){
    printf("ecomem: malloc failed\n");
    exit(1);
  }

  int n = ecomemstat(procs, &sys, max);
  if(n < 0){
    printf("ecomem: ecomemstat failed\n");
    free(procs);
    exit(1);
  }

  // System-wide report
  printf("\n");
  printf("=== ECO MEMORY SUSTAINABILITY REPORT ===\n");
  printf("\n");
  printf("System Memory:\n");
  printf("  Total pages:       ");
  printint(sys.total_pages, 6);
  printf("  (");
  printint((int)(sys.total_pages * 4), 1);
  printf(" KB)\n");
  printf("  Used pages:        ");
  printint(sys.used_pages, 6);
  printf("  (");
  printint((int)(sys.used_pages * 4), 1);
  printf(" KB)\n");
  printf("  Free pages:        ");
  printint(sys.free_pages, 6);
  printf("  (");
  printint((int)(sys.free_pages * 4), 1);
  printf(" KB)\n");
  printf("  Utilization:       ");
  printint(sys.utilization_percent, 3);
  printf("%%\n");
  printf("  Pages saveable:    ");
  printint(sys.pages_saveable, 6);
  printf("  (could power down ");
  printint((int)(sys.pages_saveable * 4), 1);
  printf(" KB)\n");
  printf("  Efficiency score:  ");
  printint(sys.efficiency_score, 3);
  printf("/100\n");
  printf("\n");

  // Per-process report
  printf("Per-Process Memory Usage:\n");
  printstr("PID", 6);
  printstr("NAME", 16);
  printstr("MAPPED", 8);
  printstr("UNMAPPED", 10);
  printstr("TOTAL", 8);
  printstr("WASTE%%", 8);
  printf("\n");

  // Separator
  printf("------------------------------------------------------\n");

  for(int i = 0; i < n; i++){
    printint(procs[i].pid, 4);
    printf("  ");
    printstr(procs[i].name, 16);
    printint(procs[i].pages_mapped, 6);
    printf("  ");
    printint(procs[i].pages_unmapped, 8);
    printf("  ");
    printint(procs[i].pages_allocated, 6);
    printf("  ");
    printint(procs[i].waste_percent, 5);
    printf("%%");
    printf("\n");
  }

  printf("\n");
  if(sys.efficiency_score >= 80)
    printf("RATING: EXCELLENT - System is energy efficient.\n");
  else if(sys.efficiency_score >= 50)
    printf("RATING: MODERATE - Some memory could be powered down.\n");
  else
    printf("RATING: POOR - High memory waste, consider consolidation.\n");
  printf("\n");

  free(procs);
  exit(0);
}
