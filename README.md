# mpf
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

## to compile
```
make
```
## to run
You can use any valid process flag as parameter:
```
sudo insmod mpf.ko pf_parameter=PF_RANDOMIZE
```
Processes with PF_RANDOMIZE flag set are written to /proc/PF_RANDOMIZE:

```
$ cat /proc/PF_RANDOMIZE
Monitoring process flags
1 systemd 
385 systemd-journal 
413 systemd-udevd 
575 multipathd 
651 systemd-timesyn 
660 systemd-network 
662 systemd-resolve 
673 accounts-daemon 
674 avahi-daemon 
675 dbus-daemon 
676 NetworkManager 
680 irqbalance 
681 networkd-dispat 
682 polkitd 
684 rsyslogd 
685 snapd 
686 switcheroo-cont 
688 systemd-logind 
691 udisksd 
701 wpa_supplicant 
708 avahi-daemon 
751 ModemManager 
768 cron 
774 atd 
789 sshd 
791 unattended-upgr 
796 gdm3 
804 gdm-session-wor 
812 systemd 
813 (sd-pam) 
822 pulseaudio 
824 rtkit-daemon 
828 gdm-wayland-ses 
830 dbus-daemon 
832 dbus-run-sessio 
833 dbus-daemon 
834 gnome-session-b 
859 gnome-shell 
910 at-spi-bus-laun 
915 dbus-daemon 
924 Xwayland 
1010 upowerd 
```

## to stop
```
sudo rmmod mpf.ko 
```
## compatibility
Currently this module is compatible with Linux kernel 3.10, 4.4, 4.8, 4.18, 5.4 and 5.15.
