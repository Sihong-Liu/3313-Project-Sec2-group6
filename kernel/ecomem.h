// Eco Memory Statistics
// Tracks per-process and system-wide memory usage to identify
// waste and support energy-efficient memory management.
// Unused memory pages can be "powered down" to save energy.

struct ecomem_proc {
  int pid;
  char name[16];
  uint64 pages_allocated;    // total physical pages allocated
  uint64 pages_mapped;       // pages actually mapped in page table
  uint64 pages_unmapped;     // pages in address space but not yet mapped (lazy)
  uint64 mem_bytes;          // process sz (virtual address space size)
  int waste_percent;         // percentage of allocated but unmapped pages
};

struct ecomem_sys {
  uint64 total_pages;        // total physical pages in system
  uint64 free_pages;         // currently free pages
  uint64 used_pages;         // currently used pages
  int utilization_percent;   // memory utilization percentage
  int efficiency_score;      // 0-100, higher = more sustainable
  uint64 pages_saveable;     // pages that could be powered down
};
