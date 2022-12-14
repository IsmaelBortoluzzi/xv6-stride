#define TEST_PROCESSES 5
#define TIMES_SCHEDULED_COUNTER 2
#define MINIMUM_TICKETS 5
#define MAXIMUM_TICKETS 500

typedef struct stest_procs {
  int procs_pids_times_scheduled[TEST_PROCESSES][TIMES_SCHEDULED_COUNTER];
} StestProcs;

void stest_save_pid(int pid);
void stest_print();