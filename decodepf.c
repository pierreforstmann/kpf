/*
 * decodepf.c
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

#include "spf.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pierre Forstmann");
MODULE_DESCRIPTION("decode process flag in log buffer");

/*
 * module parameters
 */
static int pf  = 0; 
module_param(pf, int, 0644);
MODULE_PARM_DESC(pf, "process flag ");

static int verbose = 0;
module_param(verbose, int, 0400);
MODULE_PARM_DESC(verbose, "verbose flag (1=true)");

extern uint64_t fa[MAX_PF_NR];
extern char *fda[MAX_PF_NR];
extern uint8_t fa_count;
extern void dump_arrays(char *module);

static int check_process_flag(uint64_t u)
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
		found = 1;
		printk(KERN_INFO "dpf: ");
		printk(KERN_CONT "%llu = ", u);
		for (i = 0; i < fa_count; i++)
		{
		  if (match[i] == 1)
		  {	
		     if (i != last_match_index)
		     {
		       printk(KERN_CONT " %llu (%s)", fa[i], fda[i]);
		       printk(KERN_CONT " + ");
		     }
		     else
		       printk(KERN_CONT " %llu (%s) \n", fa[i], fda[i]);
		 }
	       }
	}
	else 	printk(KERN_INFO "dpf: %llu is not a valid process flag \n", u);

	return found;
}


static int module_run(void) 
{

  int i;

  for (i=0; i< MAX_PF_NR; i++)
	if (fa[i] == 0)
		break;
  fa_count = i - 1;

  if (verbose == 1)
	  dump_arrays("dpf");

  check_process_flag(pf); 

  return 0;
}

static int __init dpf_start(void)
{
    printk(KERN_INFO "dpf: starting ... \n");
    
    if (module_run() != 0)
	    return -1;
    
    return 0;  
}

static void __exit dpf_stop(void)
{
  printk(KERN_INFO "dpf: ... stopping \n");
}

module_init(dpf_start);
module_exit(dpf_stop);
