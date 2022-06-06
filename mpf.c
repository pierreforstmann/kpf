/*
 * mpf.c
 *
 * monitor process flag
 *
 * for kernel 5.4
 *
 * Copyright Pierre Forstmann 2022
 */
#include <linux/init.h>      
#include <linux/module.h>   
#include <linux/proc_fs.h>
#include <linux/list.h>
#include <linux/sched/signal.h>
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
	PF_IDLE,
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
	PF_NOFREEZE,
	PF_FROZEN,
	PF_KSWAPD,
	PF_MEMALLOC_NOFS,
	PF_MEMALLOC_NOIO,
	PF_LESS_THROTTLE,
	PF_KTHREAD,
	PF_RANDOMIZE,
	PF_SWAPWRITE,
	PF_MEMSTALL,
	PF_UMH,
	PF_NO_SETAFFINITY,
	PF_MCE_EARLY,
	PF_MEMALLOC_NOCMA,
	PF_FREEZER_SKIP,
	PF_SUSPEND_TASK

};

static char *fda[MAX_PF_NR] =
{
	"PF_IDLE",
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
	"PF_NOFREEZE",
	"PF_FROZEN",
	"PF_KSWAPD",
	"PF_MEMALLOC_NOFS",
	"PF_MEMALLOC_NOIO",
	"PF_LESS_THROTTLE",
	"PF_KTHREAD",
	"PF_RANDOMIZE",
	"PF_SWAPWRITE",
	"PF_MEMSTALL",
	"PF_UMH",
	"PF_NO_SETAFFINITY",
	"PF_MCE_EARLY",
	"PF_MEMALLOC_NOCMA",
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

static struct file_operations ops = {
  .owner = THIS_MODULE,
  .open = mpf_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};


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

  new_proc = proc_create(pf_parameter, 0644, 0, &ops);
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
