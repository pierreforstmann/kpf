/*
 * mpf.c
 *
 * monitor process flag
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
MODULE_DESCRIPTION("monitor process flag in /proc");

/*
 * module parameters
 */
static char *pf_parameter = "";
module_param(pf_parameter, charp, 0000);
MODULE_PARM_DESC(pf_parameter, "process flag parameter");

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
static uint8_t pf_index;

static void check_process(struct seq_file *s, int i);

static int mpf_proc_show(struct seq_file *s, void *v) 
{

  seq_printf(s, "Monitoring process flags\n");
  check_process(s, pf_index);

  return 0;
}

static int mpf_open(struct inode *inode, struct  file *file) 
{
  return single_open(file, mpf_proc_show, NULL);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,15,0)
static struct file_operations f_ops = {
  .owner = THIS_MODULE,
  .open = mpf_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};
#else
static struct proc_ops  p_ops = {
  .proc_open = mpf_open,
  .proc_read = seq_read,
  .proc_lseek = seq_lseek,
  .proc_release = single_release,
};
#endif


static int check_process_flag(uint64_t u, uint8_t pf_index)
{
	int i;
	int last_match_index ;
	uint64_t match[MAX_PF_NR];
	uint64_t sum;
	int	 found;

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
		for (i = 0; i < fa_count; i++)
		{
			if (pf_index == i && match[i] == 1)
			{
				found = 1;
			}
		}
	}

	return found;
}


static void check_process(struct seq_file *sfile, int index)
{
   struct task_struct *task;

    for_each_process (task) {
	if (check_process_flag(task->flags, index) == 1)
	{
		printk(KERN_INFO "%d %s PF %d", task->pid, task->comm, index);
		seq_printf(sfile,  "%d %s \n", task->pid, task->comm);
	}
    }
}

static int module_init_proc(void) 
{

  struct proc_dir_entry *new_proc;
  int i;
  int found = 0;
  int index;

  for (i=0; i< MAX_PF_NR; i++)
	if (fa[i] == 0)
		break;
  fa_count = i - 1;

   for (i = 0 ; i < fa_count; i++)
	if (strcmp(fda[i], pf_parameter) ==  0)
	{
		found = 1;
		index = i;
		break;
	}
   if (found == 0)
   {
        printk(KERN_INFO "process flag parameter %s not found \n", pf_parameter);
	return 1;
   }
   else	printk(KERN_INFO "process flag parameter %s found \n", pf_parameter);
   
   pf_index = index;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,15,0)
  new_proc = proc_create(pf_parameter, 0644, 0, &f_ops);
#else
  new_proc = proc_create(pf_parameter, 0644, 0, &p_ops);
#endif
  if (new_proc == NULL)
  {
	  printk(KERN_INFO "proc_create failed");
	  return 1;
  }

  return 0;
}

static int __init mpf_start(void)
{
    printk(KERN_INFO "Starting mpf ... \n");

    if (module_init_proc() != 0)
	    return -1;
    
    return 0;  
}

static void __exit mpf_stop(void)
{
  remove_proc_entry(pf_parameter, 0);
  printk(KERN_INFO "... Stopping mpf \n");
}

module_init(mpf_start);
module_exit(mpf_stop);
