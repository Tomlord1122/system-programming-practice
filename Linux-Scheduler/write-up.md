# Linux Scheduler

## Introduction

In this guide, we'll walk through implementing a new scheduler class for the Linux kernel (5.15) that uses the Multilevel Queue (MLQ) algorithm. This implementation will:

1. Create a new scheduling class named `sched_mlq_class`

2. Add a new scheduling policy `SCHED_MLQ` 

3. Position the scheduler between `fair_sched_class` and `rt_sched_class` in priority

4. Support all relevant scheduler-related system calls

## MLQ Scheduler Design

Our MLQ scheduler will have 3 priority queues:

1. **Queue1** (Priority 1): RR policy with 50ms time slice

2. **Queue2** (Priority 2): RR policy with 100ms time slice  

3. **Queue3** (Priority 3): FIFO policy

Higher priority queues preempt lower priority queues. Tasks can only be moved between queues by explicitly changing their priority using system calls.

## Implementation Steps

### 1\. Set Up Your Development Environment

```bash
# Clone Linux kernel source
git clone https://github.com/torvalds/linux.git
cd linux
git checkout v5.15

# Install necessary dependencies (for Ubuntu)
sudo apt-get install build-essential libncurses-dev bison flex libssl-dev libelf-dev
```

### 2\. Create and Modify Necessary Files

#### a. Define MLQ Scheduler Structure in `include/linux/sched.h`

Add the MLQ entity structure to `sched.h`:

```c
struct sched_mlq_entity {
    struct list_head        run_list;
    unsigned long           timeout;
    unsigned long           watchdog_stamp;
    unsigned int            time_slice;
    unsigned short          on_rq;
    unsigned short          on_list;
    struct sched_mlq_entity *back;
};
```

Also add the `mlq_priority` field and entity to the `task_struct`:

```c
struct task_struct {
    // ... existing fields
    unsigned int            mlq_priority;
    struct sched_mlq_entity mlq;
    // ... more fields
};
```

#### b. Create `include/linux/sched/mlq.h`

Create a new header file with helper functions:

```c
#ifndef _LINUX_SCHED_MLQ_H
#define _LINUX_SCHED_MLQ_H

#include <linux/sched.h>

struct task_struct;

static inline int mlq_prio(int prio)
{
    if (unlikely(prio > MAX_RT_PRIO && prio < MAX_RT_PRIO + MAX_MLQ_PRIO))
        return 1;
    return 0;
}

static inline int mlq_task(struct task_struct *p)
{
    return mlq_prio(p->prio);
}

#endif
```

#### c. Update `include/linux/sched/prio.h`

Define MLQ priority constants:

```c
#define MAX_MLQ_PRIO    3

#define MAX_PRIO        (MAX_RT_PRIO + MAX_MLQ_PRIO + NICE_WIDTH)
#define DEFAULT_PRIO    (MAX_RT_PRIO + MAX_MLQ_PRIO + NICE_WIDTH / 2)
```

#### d. Update `include/uapi/linux/sched.h`

Add the new scheduling policy:

```c
#define SCHED_MLQ       7
```

#### e. Update Linker Script in `include/asm-generic/vmlinux.lds.h`

Add the MLQ scheduler to the scheduler classes list:

```c
#define SCHED_DATA                      \
    STRUCT_ALIGN();                     \
    __begin_sched_classes = .;          \
    *(__idle_sched_class)               \
    *(__fair_sched_class)               \
    *(__mlq_sched_class)                \
    *(__rt_sched_class)                 \
    *(__dl_sched_class)                 \
    *(__stop_sched_class)               \
    __end_sched_classes = .;
```

#### f. Update `kernel/sched/sched.h`

Add MLQ-related structures and inline functions:

```c
// Include the MLQ header
#include <linux/sched/mlq.h>

// Define policy check function
static inline int mlq_policy(int policy)
{
    return policy == SCHED_MLQ;
}

// Add to valid_policy check
static inline bool valid_policy(int policy)
{
    return idle_policy(policy) || fair_policy(policy) ||
        mlq_policy(policy) ||
        rt_policy(policy) || dl_policy(policy);
}

// Add task policy check
static inline int task_has_mlq_policy(struct task_struct *p)
{
    return mlq_policy(p->policy);
}

// Define priority array structure
struct mlq_prio_array {
    DECLARE_BITMAP(bitmap, MAX_MLQ_PRIO+1); /* include 1 bit for delimiter */
    struct list_head queue[MAX_MLQ_PRIO];
};

// Define runqueue structure
struct mlq_rq {
    struct mlq_prio_array active;
    unsigned int mlq_nr_running;
    unsigned int rr_nr_running;
    int mlq_queued;
    u64 mlq_time;
    u64 mlq_runtime;
    raw_spinlock_t mlq_runtime_lock;
};

// Add runnable check
static inline bool mlq_rq_is_runnable(struct mlq_rq *mlq_rq)
{
    return mlq_rq->mlq_queued && mlq_rq->mlq_nr_running;
}

// Add runnable task check
static inline bool sched_mlq_runnable(struct rq *rq)
{
    return rq->mlq.mlq_queued > 0;
}

// Add function declarations
extern void init_mlq_rq(struct mlq_rq *mlq_rq);
extern void print_mlq_stats(struct seq_file *m, int cpu);
extern void print_mlq_rq(struct seq_file *m, int cpu, struct rt_rq *rt_rq);
```

#### g. Update `kernel/sched/core.c`

Modify system calls and scheduler functions to support MLQ:

```c
// Update normal priority calculation
static inline int __normal_prio(int policy, int rt_prio, int nice)
{
    // ... existing code
    else if (mlq_policy(policy))
        prio = MAX_RT_PRIO + MAX_MLQ_PRIO - rt_prio;
    // ... existing code
}

// Update scheduler class selection
static void __setscheduler_prio(struct task_struct *p, int prio)
{
    // ... existing code
    else if (mlq_prio(prio))
        p->sched_class = &mlq_sched_class;
    // ... existing code
}

// Update scheduler parameters
static void __setscheduler_params(struct task_struct *p,
                                 const struct sched_attr *attr)
{
    // ... existing code
    p->mlq_priority = attr->sched_priority;
    // ... existing code
}

// Update scheduler validation
static int __sched_setscheduler(struct task_struct *p,
                              const struct sched_attr *attr,
                              bool user, bool pi)
{
    // ... existing code
    if (mlq_policy(policy) && attr->sched_priority > MAX_MLQ_PRIO)
        return -EINVAL;
    if ((dl_policy(policy) && !__checkparam_dl(attr)) ||
        ((rt_policy(policy) || mlq_policy(policy)) != (attr->sched_priority != 0)))
        return -EINVAL;
    // ... existing code
    if (mlq_policy(policy) && attr->sched_priority != p->mlq_priority)
        goto change;
    // ... existing code
}

// Update get_params
static void get_params(struct task_struct *p, struct sched_attr *attr)
{
    // ... existing code
    else if (task_has_mlq_policy(p))
        attr->sched_priority = p->mlq_priority;
    // ... existing code
}

// Update system calls
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, struct sched_param __user *, param)
{
    // ... existing code
    if (task_has_mlq_policy(p))
        lp.sched_priority = p->mlq_priority;
    // ... existing code
}

SYSCALL_DEFINE1(sched_get_priority_max, int, policy)
{
    // ... existing code
    case SCHED_MLQ:
        ret = MAX_MLQ_PRIO;
        break;
    // ... existing code
}

SYSCALL_DEFINE1(sched_get_priority_min, int, policy)
{
    // ... existing code
    case SCHED_MLQ:
        ret = 1;
        break;
    // ... existing code
}

// Update scheduler initialization
void __init sched_init(void)
{
    // ... existing code
    BUG_ON(&idle_sched_class + 1 != &fair_sched_class ||
           &fair_sched_class + 1 != &mlq_sched_class ||
           &mlq_sched_class + 1 != &rt_sched_class ||
           &rt_sched_class + 1   != &dl_sched_class);
    // ... existing code
    init_mlq_rq(&rq->mlq);
    // ... existing code
}
```

#### h. Create `kernel/sched/mlq.c`

The most important file that implements the MLQ scheduling algorithm:

```c
// SPDX-License-Identifier: GPL-2.0
#include "sched.h"

enum {
    MLQ_Zero,
    MLQ_FirstRR,
    MLQ_SecondRR,
    MLQ_FIFO,
};

int sched_rr_1_timeslice = (50 * HZ / 1000);
int sched_rr_2_timeslice = (100 * HZ / 1000);

// Initialize MLQ runqueue
void init_mlq_rq(struct mlq_rq *mlq_rq)
{
    struct mlq_prio_array *array;
    int i;

    array = &mlq_rq->active;
    for (i = 0; i < MAX_MLQ_PRIO; i++) {
        INIT_LIST_HEAD(array->queue + i);
        __clear_bit(i, array->bitmap);
    }
    /* delimiter for bitsearch: */
    __set_bit(MAX_MLQ_PRIO, array->bitmap);

    mlq_rq->mlq_queued = 0;
    mlq_rq->mlq_time = 0;
    mlq_rq->mlq_runtime = 0;
    raw_spin_lock_init(&mlq_rq->mlq_runtime_lock);
}

// Helper functions for entity/task conversions
static inline struct task_struct *mlq_task_of(struct sched_mlq_entity *mlq_se)
{
    return container_of(mlq_se, struct task_struct, mlq);
}

static inline struct rq *rq_of_mlq_rq(struct mlq_rq *mlq_rq)
{
    return container_of(mlq_rq, struct rq, mlq);
}

// More helper functions...

// Core scheduling operations
static void enqueue_task_mlq(struct rq *rq, struct task_struct *p, int flags)
{
    struct sched_mlq_entity *mlq_se = &p->mlq;

    if (flags & ENQUEUE_WAKEUP)
        mlq_se->timeout = 0;

    enqueue_mlq_entity(mlq_se, flags);
}

static void dequeue_task_mlq(struct rq *rq, struct task_struct *p, int flags)
{
    struct sched_mlq_entity *mlq_se = &p->mlq;

    update_curr_mlq(rq);
    dequeue_mlq_entity(mlq_se, flags);
}

// Task scheduling functions
static void check_preempt_curr_mlq(struct rq *rq, struct task_struct *p, int flags)
{
    if (p->mlq_priority < rq->curr->mlq_priority) {
        resched_curr(rq);
        return;
    }
}

static struct task_struct *pick_next_task_mlq(struct rq *rq)
{
    struct task_struct *p = pick_task_mlq(rq);

    if (p)
        set_next_task_mlq(rq, p, true);

    return p;
}

// Timeslice handling
static void task_tick_mlq(struct rq *rq, struct task_struct *task, int queued)
{
    struct sched_mlq_entity *mlq_se = &task->mlq;

    update_curr_mlq(rq);

    // FIFO tasks have no timeslices
    if (task->mlq_priority == MLQ_FIFO)
        return;

    if (--task->mlq.time_slice)
        return;

    // Reset time slice based on priority
    if (task->mlq_priority == MLQ_FirstRR)
        task->mlq.time_slice = sched_rr_1_timeslice;

    if (task->mlq_priority == MLQ_SecondRR)
        task->mlq.time_slice = sched_rr_2_timeslice;

    // Requeue if not the only task
    if (mlq_se->run_list.prev != mlq_se->run_list.next) {
        requeue_task_mlq(rq, task, 0);
        resched_curr(rq);
        return;
    }
}

// Define the scheduler class structure
DEFINE_SCHED_CLASS(mlq) = {
    .enqueue_task      = enqueue_task_mlq,
    .dequeue_task      = dequeue_task_mlq,
    .yield_task        = yield_task_mlq,
    .check_preempt_curr = check_preempt_curr_mlq,
    .pick_next_task    = pick_next_task_mlq,
    .put_prev_task     = put_prev_task_mlq,
    .set_next_task     = set_next_task_mlq,
    .task_tick         = task_tick_mlq,
    .get_rr_interval   = get_rr_interval_mlq,
    .prio_changed      = prio_changed_mlq,
    .switched_to       = switched_to_mlq,
    .update_curr       = update_curr_mlq,
#ifdef CONFIG_SMP
    .balance           = balance_mlq,
    .pick_task         = pick_task_mlq,
    .set_cpus_allowed  = set_cpus_allowed_common,
#endif
};
```

#### i. Update `kernel/sched/Makefile`

Add mlq.o to the list of object files:

```makefile
obj-y += idle.o fair.o rt.o mlq.o deadline.o
```

### 3\. Compile the Kernel

```bash
# Configure the kernel
make defconfig
make menuconfig  # Optional: configure additional options

# Build the kernel
make -j$(nproc)

# Install modules and kernel
sudo make modules_install
sudo make install
```

### 4\. Testing the MLQ Scheduler

Create a test program to set and use the MLQ scheduler:

```c
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>

// Define SCHED_MLQ since it might not be in user headers yet
#ifndef SCHED_MLQ
#define SCHED_MLQ 7
#endif

int main(int argc, char *argv[]) {
    struct sched_param param;
    int priority = 1;
    
    if (argc > 1) {
        priority = atoi(argv[1]);
        if (priority < 1 || priority > 3) {
            fprintf(stderr, "Priority must be between 1 and 3\n");
            return 1;
        }
    }
    
    printf("Setting scheduler policy to MLQ with priority %d\n", priority);
    
    param.sched_priority = priority;
    if (sched_setscheduler(0, SCHED_MLQ, &param) == -1) {
        perror("sched_setscheduler failed");
        return 1;
    }
    
    printf("Success! Running with MLQ scheduling.\n");
    
    // Do some work
    for (int i = 0; i < 5; i++) {
        printf("Working... %d\n", i);
        sleep(1);
    }
    
    return 0;
}
```

Compile and run:

```bash
gcc -o test_mlq test_mlq.c
sudo ./test_mlq 1  # Try with different priorities
```

## Understanding Implementation Details

### MLQ Priority Queue Structure

The MLQ scheduler uses a bitmap to efficiently find the highest priority non-empty queue:

```c
struct mlq_prio_array {
    DECLARE_BITMAP(bitmap, MAX_MLQ_PRIO+1);
    struct list_head queue[MAX_MLQ_PRIO];
};
```

### Time Slice Management

- Queue 1 (priority 1): 50ms time slice (`sched_rr_1_timeslice`)

- Queue 2 (priority 2): 100ms time slice (`sched_rr_2_timeslice`)

- Queue 3 (priority 3): FIFO policy (no time slices)

The `task_tick_mlq()` function manages time slices, resets them when they expire, and requeues tasks if needed.

### System Call Integration

For user applications to interact with the MLQ scheduler, we've added support in:

- `sched_setscheduler()`: Set task scheduler to MLQ with priority 1-3

- `sched_getscheduler()`: Return SCHED_MLQ for MLQ tasks

- `sched_setparam()`: Change MLQ priority

- `sched_getparam()`: Get current MLQ priority

- `sched_get_priority_max()`: Return 3 for SCHED_MLQ

- `sched_get_priority_min()`: Return 1 for SCHED_MLQ

## Conclusion

You've now successfully implemented a Multilevel Queue scheduler for the Linux kernel! This scheduler provides three priority queues with different scheduling policies (RR and FIFO) and supports all the standard scheduler-related system calls.