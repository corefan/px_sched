// Example-4:
// Tasks can launch sub-tasks and wait for them

#define PX_SCHED_IMPLEMENTATION 1
#include "../px_sched.h"

int main(int, char **) {
  px::Scheduler schd;
  schd.init();

  px::Sync s;
  for(size_t i = 0; i < 10; ++i) {
    auto job = [i] {
      printf("Phase 1: Task %zu completed from %s\n",
       i, px::Scheduler::current_thread_name());
    };
    schd.run(job, &s);
  }

  px::Sync last;
  schd.runAfter(s, [&schd]{
    printf("Phase 2\n");
    px::Sync s;
    for(size_t i = 0; i < 10; ++i) {
      auto job = [i] {
        printf("Phase 2: Task %zu completed from %s\n",
         i, px::Scheduler::current_thread_name());
      };
      schd.run(job, &s);
    }
    schd.waitFor(s);
    printf("Phase 2, done\n");
   }, &last);

  printf("Waiting for tasks to finish...\n");
  schd.waitFor(last); // wait for all tasks to finish
  printf("Waiting for tasks to finish...DONE \n");

  return 0;
}
