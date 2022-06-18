# kpf
Linux kernel module to monitor process flag
## prerequisites

In general you need to install at least following packages (exact list depends on the specific GNU/Linux distribution used):

- kernel-headers
- kernel-devel
- gcc

On CentOS 7.9 I also had to run:
```
sudo ln -s /usr/src/kernels/3.10.0-1160.66.1.el7.x86_64/ /usr/src/kernels/3.10.0-1160.el7.x86_64

```
sudo insmod dpf.ko pf=69238848
sudo dmesg



## to compile
```
make
```
## to run
To decode numeric process flags:
```
sudo insmod dpf.ko pf=69238848
sudo dmesg | tail -n 5
[ 4870.564629] Starting dpf ... 
[ 4870.564631] process flag count=25

[ 4870.564633] 69238848 =  64 (PF_MCE_PROCESS) +  32768 (PF_FROZEN) +  2097152 (PF_KTHREAD) +  67108864 (PF_NO_SETAFFINITY)
sudo rmmod dpf.ko
```

To monitor a specific process flag in `/proc` you can use any valid symbolic process flag as parameter:
```
sudo insmod mpf.ko pf_parameter=PF_KTHREAD
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
14 rcu_sched 
15 migration/0 
16 idle_inject/0 
17 cpuhp/0 
18 kdevtmpfs 
19 inet_frag_wq 
20 kauditd 
21 khungtaskd 
22 oom_reaper 
23 writeback 
24 kcompactd0 
25 ksmd 
26 khugepaged 
72 kintegrityd 
73 kblockd 
74 blkcg_punt_bio 
75 tpm_dev_wq 
76 ata_sff 
77 md 
78 edac-poller 
79 devfreq_wq 
80 watchdogd 
82 kworker/0:1H 
84 kswapd0 
85 ecryptfs-kthrea 
87 kthrotld 
88 irq/24-aerdrv 
89 irq/25-aerdrv 
90 irq/26-aerdrv 
91 irq/27-aerdrv 
92 irq/28-aerdrv 
93 irq/29-aerdrv 
94 irq/30-aerdrv 
95 acpi_thermal_pm 
97 vfio-irqfd-clea 
98 mld 
99 ipv6_addrconf 
109 kstrp 
112 zswap-shrink 
113 kworker/u3:0 
118 charger_manager 
163 cryptd 
177 scsi_eh_0 
180 scsi_tmf_0 
182 scsi_eh_1 
184 scsi_tmf_1 
185 scsi_eh_2 
187 scsi_tmf_2 
190 scsi_eh_3 
192 scsi_tmf_3 
194 scsi_eh_4 
196 scsi_tmf_4 
198 scsi_eh_5 
199 scsi_tmf_5 
213 kdmflush 
251 raid5wq 
311 ext4-rsv-conver 
344 hwrng 
412 kaluad 
413 ipmi-msghandler 
414 kmpath_rdacd 
416 kmpathd 
417 kmpath_handlerd 
508 ext4-rsv-conver 
5481 kworker/0:0 
7969 kworker/u2:1 
9730 kworker/u2:2 
10332 kworker/u2:0 
```
## to stop

sudo rmmod mpf.ko 
```
## compatibility
Currently this module is compatible with Linux kernel 3.10, 4.4, 4.15, 4.18, 5.4 and 5.15.
