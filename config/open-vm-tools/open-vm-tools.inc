<?php

function open_vm_tools_deinstall() {
	exec("rm /usr/local/etc/rc.d/vmware*");
}

function open_vm_tools_install() {

	$vmware_guestd = <<<EOF
#!/bin/sh
#
# This file was automatically generated 
# by the pfSense package manager.
# 
# Do not edit this file.  Edit 
# /usr/local/pkg/open-vm-tools.inc instead.
#
# PROVIDE: vmware-guestd
# REQUIRE: DAEMON
# BEFORE: LOGIN

. /etc/rc.subr

# Global
checkvm_cmd="/usr/local/bin/vmware-checkvm > /dev/null"

# VMware guest daemon
name="vmware_guestd"
rcvar=`set_rcvar`
start_precmd="\${checkvm_cmd}"
unset start_cmd
stop_precmd="\${checkvm_cmd}"
unset stop_cmd
command="/usr/local/bin/vmtoolsd"
command_args="-c /usr/local/share/vmware-tools/tools.conf -p /usr/local/lib/open-vm-tools/plugins/vmsvc"
pidfile="/var/run/\${name}.pid"

load_rc_config \$name
vmware_guestd_enable="YES"
vmware_guestd_flags="--background \${pidfile}"
run_rc_command "\$1"

EOF;

	$vmware_kmod = <<<EOF
#!/bin/sh
#
# This file was automatically generated 
# by the pfSense package manager.
# 
# Do not edit this file.  Edit 
# /usr/local/pkg/open-vm-tools.inc instead.
#
# PROVIDE: vmware-kmod
# REQUIRE: FILESYSTEMS
# BEFORE: netif

. /etc/rc.subr

# Global
checkvm_cmd="/usr/local/bin/vmware-checkvm > /dev/null"

# Functions
vmware_guest_vmmemctl_start()
{
	echo 'Loading vmmemctl kernel module.'
	kldload /usr/local/lib/vmware-tools/modules/drivers/vmmemctl.ko >/dev/null 2>&1
}
vmware_guest_vmxnet_start()
{
	echo 'Loading vmxnet kernel module.'
	kldload /usr/local/lib/vmware-tools/modules/drivers/vmxnet.ko >/dev/null 2>&1
}
vmware_guest_vmblock_start()
{
	echo 'Loading vmblock kernel module.'
	kldload /usr/local/lib/vmware-tools/modules/drivers/vmblock.ko >/dev/null 2>&1
}
vmware_guest_vmhgfs_start()
{
	echo 'Loading vmhgfs kernel module.'
	kldload /usr/local/lib/vmware-tools/modules/drivers/vmhgfs.ko >/dev/null 2>&1
}

# VMware kernel module: vmmemctl
name="vmware_guest_vmmemctl"
rcvar=`set_rcvar`
start_precmd="\${checkvm_cmd}"
start_cmd="vmware_guest_vmmemctl_start"
stop_precmd="\${checkvm_cmd}"
stop_cmd=":"

load_rc_config \$name
vmware_guest_vmmemctl_enable="YES"
run_rc_command "\$1"

# VMware kernel module: vmxnet
name="vmware_guest_vmxnet"
rcvar=`set_rcvar`
start_precmd="\${checkvm_cmd}"
start_cmd="vmware_guest_vmxnet_start"
stop_precmd="\${checkvm_cmd}"
stop_cmd=":"

load_rc_config \$name
vmware_guest_vmxnet_enable="YES"
run_rc_command "\$1"

# VMware kernel module: vmblock
name="vmware_guest_vmblock"
rcvar=`set_rcvar`
start_precmd="\${checkvm_cmd}"
start_cmd="vmware_guest_vmblock_start"
stop_precmd="\${checkvm_cmd}"
stop_cmd=":"

load_rc_config \$name
vmware_guest_vmblock_enable="YES"
run_rc_command "\$1"

# VMware kernel module: vmhgfs
name="vmware_guest_vmhgfs"
rcvar=`set_rcvar`
start_precmd="\${checkvm_cmd}"
start_cmd="vmware_guest_vmhgfs_start"
stop_precmd="\${checkvm_cmd}"
stop_cmd=":"

load_rc_config \$name
vmware_guest_vmhgfs_enable="YES"
run_rc_command "\$1"

EOF;

	// Write out conf files.
	$fd = fopen("/usr/local/etc/rc.d/vmware-guestd.sh", "w");
	if(!$fd) 
		die("Could not open /usr/local/etc/rc.d/vmware-guestd.sh for writing");
	fwrite($fd, $vmware_guestd);
	fclose($fd);
	$fd = fopen("/usr/local/etc/rc.d/vmware-kmod.sh", "w");
	if(!$fd) 
		die("Could not open /usr/local/etc/rc.d/vmware-kmod.sh for writing");
	fwrite($fd, $vmware_kmod);
	fclose($fd);

	// Remove non used files
	unlink_if_exists("/usr/local/etc/rc.d/vmware-kmod");
	unlink_if_exists("/usr/local/etc/rc.d/vmware-guestd");

	// Make sure files are executable.
	exec("chmod a+rx /usr/local/etc/rc.d/*.sh");

}

?>