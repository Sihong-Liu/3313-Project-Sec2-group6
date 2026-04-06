// energy_demo.c – Demo program for the Energy-Aware Scheduler
//
// PURPOSE
// -------
// This program demonstrates the Energy-Aware Scheduler implemented in
// kernel/proc.c.  It forks two CPU-bound worker children and one
// cooperative (sleeping) child so that a contrast in energy_score is
// visible in the `ecops` output.
//
// HOW TO USE DURING A DEMO
// ------------------------
//  1. Boot xv6 and run: energy_demo
//  2. In a second shell: ecops
//     → The CPU-bound workers accumulate a high ENERGY score.
//     → The cooperative worker stays near 0.
//  3. Notice that under low system load (only 3 processes runnable)
//     the CPU-bound workers will be deferred by the energy-aware check
//     and the idle wfi path activates, simulating a low-power mode.
//
// ENERGY-AWARE SCHEDULER RECAP
// ----------------------------
//  • energy_score tracks how many scheduling quanta a process has consumed.
//  • Threshold constants (in proc.c): ENERGY_LIMIT=50, LOAD_THRESHOLD=3,
//    TICK_WEIGHT=1, ENERGY_DECAY=5.
//  • When active runnable count < LOAD_THRESHOLD and a process has
//    energy_score > ENERGY_LIMIT, the scheduler skips it for one pass
//    and subtracts ENERGY_DECAY so it eventually gets scheduled again.
//
// DESIGN NOTES
// ------------
//  • busyloop already exists in the repo and is a perfect single-process
//    test.  This program adds a multi-process scenario with visible contrast.
//  • We keep the loop count small so the demo finishes in a reasonable time.

#include "kernel/types.h"
#include "user/user.h"

// How many spin iterations each "heavy" worker does per round.
// Tuned so each round is noticeable but the whole demo does not take forever.
#define SPIN_ITERS 100000000ULL
#define HEAVY_ROUNDS 5

// A cooperative worker that voluntarily yields CPU time between rounds.
// Its energy_score stays low because it sleeps (not counted as a busy tick).
static void
cooperative_worker(void)
{
  printf("energy_demo: cooperative worker pid=%d starting\n", getpid());
  for(int r = 0; r < HEAVY_ROUNDS; r++){
    // Simulate a bit of useful work, then willingly give up the CPU.
    volatile uint64 x = 0;
    for(uint64 i = 0; i < 1000000ULL; i++) x += i;
    (void)x;
    // pause(1) yields to the scheduler for 1 tick, keeping energy_score low.
    pause(1);
  }
  printf("energy_demo: cooperative worker pid=%d done\n", getpid());
  exit(0);
}

// A CPU-bound worker that spins without sleeping.
// Its energy_score climbs quickly; under low load it will be deferred by
// the energy-aware scheduler once energy_score > ENERGY_LIMIT.
static void
heavy_worker(int id)
{
  printf("energy_demo: heavy worker %d pid=%d starting\n", id, getpid());
  volatile uint64 x = 0;
  for(int r = 0; r < HEAVY_ROUNDS; r++){
    for(uint64 i = 0; i < SPIN_ITERS; i++) x += i;
  }
  // Prevent the compiler from optimising the loop away.
  if(x == 0) printf("energy_demo: unreachable\n");
  printf("energy_demo: heavy worker %d pid=%d done\n", id, getpid());
  exit(0);
}

int
main(void)
{
  printf("\n");
  printf("=== Energy-Aware Scheduler Demo ===\n");
  printf("Spawning 2 CPU-heavy workers + 1 cooperative worker.\n");
  printf("Run `ecops` in another window to watch ENERGY scores grow.\n");
  printf("Under low load, heavy workers will be deferred by the scheduler\n");
  printf("once their energy_score exceeds ENERGY_LIMIT (%d).\n", 50);
  printf("\n");

  // Fork first heavy worker.
  int pid1 = fork();
  if(pid1 == 0)
    heavy_worker(1);
  if(pid1 < 0){
    fprintf(2, "energy_demo: fork failed\n");
    exit(1);
  }

  // Fork second heavy worker.
  int pid2 = fork();
  if(pid2 == 0)
    heavy_worker(2);
  if(pid2 < 0){
    fprintf(2, "energy_demo: fork failed\n");
    exit(1);
  }

  // Fork cooperative (sleeping) worker.
  int pid3 = fork();
  if(pid3 == 0)
    cooperative_worker();
  if(pid3 < 0){
    fprintf(2, "energy_demo: fork failed\n");
    exit(1);
  }

  printf("energy_demo: parent pid=%d waiting for children %d %d %d\n",
         getpid(), pid1, pid2, pid3);

  // Wait for all three children.
  wait(0);
  wait(0);
  wait(0);

  printf("\n");
  printf("energy_demo: all workers finished.\n");
  printf("Run `ecops` now to see final energy scores.\n");
  printf("Tip: heavy workers should show ENERGY near or above %d\n", 50);
  printf("     and the cooperative worker should show ENERGY near 0.\n");
  exit(0);
}
