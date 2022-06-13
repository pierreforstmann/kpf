/*
 * dpf.c
 *
 * decode process flag
 *
 * Copyright Pierre Forstmann 2022
 */
#include <linux/init.h>      
#include <linux/module.h>   
#include <linux/proc_fs.h>
#include <linux/list.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE <= KERNEL_VERSION(4,5,0)
#include <linux/signal.h>
#include <linux/sched.h>
#else
#include <linux/sched/signal.h>
#endif
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pierre Forstmann");
MODULE_DESCRIPTION("decode process flag in log buffer");

/*
 * module parameters
 */
static int pf  = 0; 
module_param(pf, int, 0644);
MODULE_PARM_DESC(pf, "process flag ");

#define MAX_PF_NR	50

static uint64_t fa[MAX_PF_NR] = {
#ifdef	PF_IDLE
	PF_IDLE,
#endif
	PF_EXITING,
	PF_VCPU,
	PF_WQ_WORKER,
	PF_FORKNOEXEC,
	PF_MCE_PROCESS,
	PF_SUPERPRIV,
	PF_DUMPCORE,
	PF_SIGNALED,
	PF_MEMALLOC,
	PF_NPROC_EXCEEDED,
	PF_USED_MATH,
#ifdef	PF_USED_ASYNC
	PF_USED_ASYNC,
#endif
	PF_NOFREEZE,
	PF_FROZEN,
	PF_KSWAPD,
#ifdef	PF_MALLOC_NOFS
	PF_MEMALLOC_NOFS,
#endif
	PF_MEMALLOC_NOIO,
#ifdef PF_LESS_THROTTLE 
	PF_LESS_THROTTLE,
#endif
#ifdef PF_LOCAL_THROTTLE 
	PF_LOCAL_THROTTLE,
#endif
	PF_KTHREAD,
	PF_RANDOMIZE,
	PF_SWAPWRITE,
#ifdef PF_MEMSTALL
	PF_MEMSTALL,
#endif
#ifdef	PF_UMH
	PF_UMH,
#endif
	PF_NO_SETAFFINITY,
	PF_MCE_EARLY,
#ifdef	PF_MEMALLOC_PIN
	PF_MEMALLOC_PIN,
#endif
#ifdef	PF_MEMALLOC_NOCMA
	PF_MEMALLOC_NOCMA,
#endif
#ifdef	PF_IO_WORKER
	PF_IO_WORKER,
#endif
#ifdef	PF_MUTEX_TESTER
	PF_MUTEX_TESTER,
#endif
	PF_FREEZER_SKIP,
	PF_SUSPEND_TASK

};

static char *fda[MAX_PF_NR] =
{
#ifdef	PF_IDLE_
	"PF_IDLE",
#endif	
	"PF_EXITING",
	"PF_VCPU",
	"PF_WQ_WORKER",
	"PF_FORKNOEXEC",
	"PF_MCE_PROCESS",
	"PF_SUPERPRIV",
	"PF_DUMPCORE",
	"PF_SIGNALED",
	"PF_MEMALLOC",
	"PF_NPROC_EXCEEDED",
	"PF_USED_MATH",
#ifdef	PF_USED_ASYNC
	"PF_USED_ASYNC",
#endif
	"PF_NOFREEZE",
	"PF_FROZEN",
	"PF_KSWAPD",
#ifdef	PF_MEMALLOC_NOFS
	"PF_MEMALLOC_NOFS",
#endif
	"PF_MEMALLOC_NOIO",
#ifdef	PF_LESS_THROTTLE
	"PF_LESS_THROTTLE",
#endif
#ifdef	PF_LOCAL_THROTTLE
	"PF_LOCAL_THROTTLE",
#endif
	"PF_KTHREAD",
	"PF_RANDOMIZE",
	"PF_SWAPWRITE",
#ifdef	PF_MEMSTALL
	"PF_MEMSTALL",
#endif
#ifdef	PF_UMH
	"PF_UMH",
#endif
	"PF_NO_SETAFFINITY",
	"PF_MCE_EARLY",
#ifdef PF_MEMALLOC_PIN
	"PF_MEMALLOC_PIN",
#endif
#ifdef PF_MEMALLOC_NOCMA
	"PF_MEMALLOC_NOCMA",
#endif
#ifdef	PF_IO_WORKER
	"PF_IO_WORKER",
#endif
#ifdef	PF_MUTEX_TESTER
	"PF_MUTEX_TESTER",
#endif
	"PF_FREEZER_SKIP",
	"PF_SUSPEND_TASK"
};

static uint8_t fa_count;

static int check_process_flag(uint64_t u)
{
	int i;
	int last_match_index ;
	uint64_t match[MAX_PF_NR];
	uint64_t sum;
	int	 found;

	printk(KERN_INFO "\n");
	sum = 0;
	last_match_index = 0;
	for (i = 0; i < MAX_PF_NR; i++)
		match[i] = 0;

	for (i = 0; i < fa_count; i++)
		if ((fa[i] & u) == fa[i])
		{
			match[i] = 1;
			last_match_index = i;
		}
	found = 0;

	for (i = 0; i < fa_count; i++)
		if (match[i] == 1)
			sum += fa[i];
	if (sum == u)
	{
		found = 1;
		printk(KERN_INFO "%llu = ", u);
		for (i = 0; i < fa_count; i++)
		{
			if (match[i] == 1)
			{
				printk(KERN_INFO " %llu (%s)", fa[i], fda[i]);
				if (i != last_match_index)
					printk(KERN_INFO " + ");
			}
		}
		printk(KERN_INFO "\n");
	}

	return found;
}


static int module_run(void) 
{

  int i;

  for (i=0; i< MAX_PF_NR; i++)
	if (fa[i] == 0)
		break;
  fa_count = i - 1;

  check_process_flag(pf); 

  return 0;
}

static int __init dpf_start(void)
{
    printk(KERN_INFO "Starting dpf ... \n");
    
    if (module_run() != 0)
	    return -1;
    
    return 0;  
}

static void __exit dpf_stop(void)
{
  printk(KERN_INFO "... Stopping mpf \n");
}

module_init(dpf_start);
module_exit(dpf_stop);
