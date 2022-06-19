/*
 * monitorpf.c
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
static char *pf = "";
module_param(pf, charp, 0400);
MODULE_PARM_DESC(pf, "symbolic process flag");

static int verbose = 0;
module_param(verbose, int, 0400);
MODULE_PARM_DESC(verbose, "verbose flag (1=true)");

#include "spf.h"

#define MAX_PF_NR	50

extern uint64_t fa[MAX_PF_NR];
extern char *fda[MAX_PF_NR];
extern uint8_t fa_count;

static uint8_t pf_index;

static void check_process(struct seq_file *s, int i);

static int mpf_proc_show(struct seq_file *s, void *v) 
{

  seq_printf(s, "mpf: monitoring process flags\n");
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


static int check_process_flag(uint64_t u, uint8_t index)
{
	int i;
	uint64_t match[MAX_PF_NR];
	int	 found;

	found = 0;
	for (i = 0; i < MAX_PF_NR; i++)
		match[i] = 0;

	for (i = 0; i < fa_count; i++)
	{
		if ((fa[i] & u) == fa[i])
		{
			match[i] = 1;
		}
		if (index == i && match[i] == 1)
		{
				found = 1;
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
		if (verbose == 1)
		  printk(KERN_INFO "mpf: check_process: pid=%d comm=%s PF index=%d", task->pid, task->comm, index);
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

  if (verbose == 1)
  {
	printk(KERN_INFO "mpf: dumping fda and fa arrays ...");
	for (i=0; i < fa_count; i++)
	 printk(KERN_INFO "mpf: entry=%d values=%s %llu", i, fda[i], fa[i]);
	printk(KERN_INFO "mpf: ... done");
  }

   for (i = 0 ; i < fa_count; i++)
	if (strcmp(fda[i], pf) ==  0)
	{
		found = 1;
		index = i;
		break;
	}

   if (found == 0)
   {
        printk(KERN_INFO "mpf: process flag parameter %s not found \n", pf);
	return 1;
   }
   else	printk(KERN_INFO "mpf: process flag parameter %s found \n", pf);
   
   pf_index = index;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,15,0)
  new_proc = proc_create(pf, 0644, 0, &f_ops);
#else
  new_proc = proc_create(pf, 0644, 0, &p_ops);
#endif
  if (new_proc == NULL)
  {
	  printk(KERN_INFO "mpf: proc_create failed");
	  return 1;
  }

  return 0;
}

static int __init mpf_start(void)
{
    printk(KERN_INFO "mpf: starting ...");

    if (module_init_proc() != 0)
	    return -1;
    
    return 0;  
}

static void __exit mpf_stop(void)
{
  remove_proc_entry(pf, 0);
  printk(KERN_INFO "mpf: ... stopping \n");
}

module_init(mpf_start);
module_exit(mpf_stop);
