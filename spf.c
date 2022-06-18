/*
 * spc.c
 *
 * common data for kpf
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

uint64_t fa[MAX_PF_NR] = {
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

char *fda[MAX_PF_NR] =
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

uint8_t fa_count;

