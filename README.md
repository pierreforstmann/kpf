# kpf
Linux kernel module to decode and monitor process flag
## Prerequisites

In general you need to install at least following packages (exact list depends on the specific GNU/Linux distribution used):

- kernel-headers
- kernel-devel
- gcc

On CentOS 7.9 I also had to run:
```
sudo ln -s /usr/src/kernels/3.10.0-1160.66.1.el7.x86_64/ /usr/src/kernels/3.10.0-1160.el7.x86_64

```

## To compile
```
make
```
## To decode a specific numeric process flag in log buffer
```

$ sudo insmod dpf.ko pf=7
$ sudo dmesg | tail -n 2
[12236.102782] dpf: starting ... 
[12236.102785] dpf: 7 =  2 (PF_EXITING) +  4 (PF_VCPU) +  1 (PF_WQ_WORKER) 
$ sudo rmmod dpf.ko
```

## To monitor a specific process flag in `/proc` 

Use any valid symbolic process flag as parameter:
```
sudo insmod mpf.ko pf=PF_KTHREAD
```
Processes with PF_KTHREAD flag set are written to `/proc/PF_KTHREAD`:

```Monitoring process flags
2 kthreadd 
3 rcu_gp 
4 rcu_par_gp 
5 netns 
7 kworker/0:0H 
8 kworker/0:1 
10 mm_percpu_wq 
11 rcu_tasks_rude_ 
12 rcu_tasks_trace 
13 ksoftirqd/0 
[...]
508 ext4-rsv-conver 
5481 kworker/0:0 
7969 kworker/u2:1 
9730 kworker/u2:2 
10332 kworker/u2:0 
```
## To stop monitoring
```
sudo rmmod mpf.ko 
```
## Compatibility
Currently this module is compatible with Linux kernel 3.10, 4.4, 4.15, 4.18, 5.4 and 5.15.
