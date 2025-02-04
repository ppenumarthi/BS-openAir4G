#!/bin/bash
################################################################################
#   OpenAirInterface
#   Copyright(c) 1999 - 2014 Eurecom
#
#    OpenAirInterface is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#
#    OpenAirInterface is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with OpenAirInterface.The full GNU General Public License is
#    included in this distribution in the file called "COPYING". If not,
#    see <http://www.gnu.org/licenses/>.
#
#  Contact Information
#  OpenAirInterface Admin: openair_admin@eurecom.fr
#  OpenAirInterface Tech : openair_tech@eurecom.fr
#  OpenAirInterface Dev  : openair4g-devel@eurecom.fr
#
#  Address      : Eurecom, Campus SophiaTech, 450 Route des Chappes, CS 50193 - 06904 Biot Sophia Antipolis cedex, FRANCE
#
################################################################################
# file start_enb.bash
# brief
# author Lionel Gauthier
# company Eurecom
# email: lionel.gauthier@eurecom.fr
###########################################
# INPUT OF THIS SCRIPT:
# The eNB config file
#########################################
# This script start  ENB  
# Depending on configuration files, it can be instanciated a virtual switch 
# setting or a VLAN setting for the networking between eNB and MME.
# MME+SP-GW executable have to be launched on the same host by your own (start_epc.bash) before this script is invoked.
# UE executable have to be launched on another host by your own (start_ue.bash) after this script is invoked.
#

###########################################################
# Parameters
###########################################################
#declare EMULATION_DEV_INTERFACE="eth1" -- hacked by Phani
declare EMULATION_DEV_INTERFACE="eth0"
declare EMULATION_MULTICAST_GROUP=1
declare EMULATION_DEV_ADDRESS=`ifconfig $EMULATION_DEV_INTERFACE | grep 'inet addr:'| grep -v '127.0.0.1' | cut -d: -f2 | awk '{ print $1}'`

declare MAKE_LTE_ACCESS_STRATUM_TARGET="oaisim DEBUG=1 ENABLE_ITTI=1 USE_MME=R10 LINK_PDCP_TO_GTPV1U=1 NAS=1 SECU=1 RRC_MSG_PRINT=1"
declare MAKE_LTE_ACCESS_STRATUM_TARGET_RTAI="lte-softmodem DEBUG=1  RTAI=1 HARD_RT=1 EXMIMO=1  ENABLE_ITTI=1 USE_MME=R10 LINK_PDCP_TO_GTPV1U=1 DISABLE_XER_PRINT=1 SECU=1 RRC_MSG_PRINT=1 "
declare MAKE_LTE_ACCESS_STRATUM_TARGET_LINUX_KERNEL="lte-softmodem  DEBUG=0 XFORMS=0 USE_MME=R10 ENABLE_ITTI=1 LINK_PDCP_TO_GTPV1U=1 SECU=1 DISABLE_XER_PRINT=1 EXMIMO=1  Rel8=1  RTAI=0"

###########################################################
THIS_SCRIPT_PATH=$(dirname $(readlink -f $0))
. $THIS_SCRIPT_PATH/utils.bash
. $THIS_SCRIPT_PATH/interfaces.bash
. $THIS_SCRIPT_PATH/networks.bash
###########################################################
check_install_epc_software #-dont know if I hacked this
cd $THIS_SCRIPT_PATH

#######################################################
# FIND CONFIG FILE
#######################################################
DEFAULT_CONFIG_FILE="enb.`hostname -s`.conf"
CONFIG_FILE_ENB=${1:-DEFAULT_CONFIG_FILE}
CONFIG_FILE_ENB=`readlink -m $CONFIG_FILE_ENB`

echo "CONFIG_FILE_ENB=$CONFIG_FILE_ENB"
if [ -f $CONFIG_FILE_ENB ]; then
    echo_warning "ENB config file found is now $CONFIG_FILE_ENB"
else
    echo_error "ENB config file not found, exiting (searched for $CONFIG_FILE_ENB)"
    exit 1
fi

#######################################################
# SOURCE CONFIG FILE
#######################################################
rm -f /tmp/source.txt
VARIABLES="GNU_DEBUGGER\|\
           real_time"

VARIABLES=$(echo $VARIABLES | sed -e 's/\\r//g')
VARIABLES=$(echo $VARIABLES | tr -d ' ')
cat $CONFIG_FILE_ENB | grep -w "$VARIABLES"| tr -d " " | tr -d ";" > /tmp/source.txt
source /tmp/source.txt

##################################################
# LAUNCH eNB executable
##################################################
echo "GNU_DEBUGGER:"$GNU_DEBUGGER

if [ x$real_time == "xemulation" ]; then
    echo_warning "emulation mode"
    make --directory=$OPENAIR_TARGETS/SIMU/USER $MAKE_LTE_ACCESS_STRATUM_TARGET -j`grep -c ^processor /proc/cpuinfo ` || exit 1
    #bash_exec "ip route add 239.0.0.160/28 dev $EMULATION_DEV_INTERFACE" #-- why are we adding here??? Since EPC is on same machine, commented it for now
    bash_exec "ip route add 134.0.0.8/24 dev $EMULATION_DEV_INTERFACE" #-- why are we adding here??? Since EPC is on same machine, commented it for now
    #bash_exec "ip route add 130.245.144.229/24 $EMULATION_DEV_INTERFACE"
    if [ "x$GNU_DEBUGGER" == "xyes" ]; then
        #gdb --args $OPENAIR_TARGETS/SIMU/USER/oaisim -a  -l9 -u0 -b1 -M0 -p2  -g$EMULATION_MULTICAST_GROUP -D $EMULATION_DEV_ADDRESS -K /tmp/itti_enb_emul.log --enb-conf $CONFIG_FILE_ENB 2>&1 | tee /tmp/stdout_enb_emul.log
        gdb --args $OPENAIR_TARGETS/SIMU/USER/oaisim -l9 -u0 -b1 -M0 -p2  -g$EMULATION_MULTICAST_GROUP -D $EMULATION_DEV_ADDRESS -K /tmp/itti_enb_emul.log --enb-conf $CONFIG_FILE_ENB 2>&1 | tee /tmp/stdout_enb_emul.log
    else
        $OPENAIR_TARGETS/SIMU/USER/oaisim -a  -l9 -u0 -b1 -M0 -p2  -g$EMULATION_MULTICAST_GROUP -D $EMULATION_DEV_ADDRESS -K /tmp/itti_enb_emul.log --enb-conf $CONFIG_FILE_ENB 2>&1 | tee /tmp/stdout_enb_emul.log
    fi 
else
    if [ x$real_time == "xrtai" ]; then
        echo_warning "rtai mode"
        PATH=$PATH:/usr/realtime/bin

        #make --directory=$OPENAIR_TARGETS/RT/USER drivers  || exit 1
        # 2 lines below replace the line above
        cd $OPENAIR_TARGETS/ARCH/EXMIMO/DRIVER/eurecom && make clean && make   || exit 1
        cd $OPENAIR_TARGETS/ARCH/EXMIMO/USERSPACE/OAI_FW_INIT && make clean && make   || exit 1
        cd $THIS_SCRIPT_PATH

        make --directory=$OPENAIR_TARGETS/RT/USER $MAKE_LTE_ACCESS_STRATUM_TARGET_RTAI -j`grep -c ^processor /proc/cpuinfo ` || exit 1

        if [ ! -f /tmp/init_rt_done.tmp ]; then
            echo_warning "STARTING REAL TIME (RTAI)"
            insmod /usr/realtime/modules/rtai_hal.ko     > /dev/null 2>&1
            insmod /usr/realtime/modules/rtai_sched.ko   > /dev/null 2>&1
            insmod /usr/realtime/modules/rtai_sem.ko     > /dev/null 2>&1
            insmod /usr/realtime/modules/rtai_fifos.ko   > /dev/null 2>&1
            insmod /usr/realtime/modules/rtai_mbx.ko     > /dev/null 2>&1
            touch /tmp/init_rt_done.tmp
            chmod 666 /tmp/init_rt_done.tmp
        else
            echo_warning "REAL TIME FOUND STARTED (RTAI)"
        fi

        cd $OPENAIR_TARGETS/RT/USER
        bash ./init_exmimo2.sh
        echo_warning "STARTING SOFTMODEM..."
        #cat /dev/rtf62 > $STDOUT_LOG_FILE &
        if [ "x$GNU_DEBUGGER" == "xyes" ]; then
            touch ~/.gdbinit
            echo "file $OPENAIR_TARGETS/RT/USER/lte-softmodem" > ~/.gdbinit
            echo "set args -K /tmp/itti_enb_rtai.log -V  -O $CONFIG_FILE_ENB" >> ~/.gdbinit
            echo "run" >> ~/.gdbinit
            gdb  2>&1 
             #> $STDOUT_LOG_FILE
            #gdb --args ./lte-softmodem -K /tmp/itti_enb_rtai.log -V  -O $CONFIG_FILE_ENB  2>&1
            cd $THIS_SCRIPT_PATH
        else
            $OPENAIR_TARGETS/RT/USER/lte-softmodem -K $/tmp/itti_enb_rtai.log -V  -O $CONFIG_FILE_ENB
        fi
    else
        if [ x$real_time == "xlinux-kernel" ]; then
            echo_warning "LINUX_KERNEL MODE"

            #make --directory=$OPENAIR_TARGETS/RT/USER drivers  || exit 1
            # 2 lines below replace the line above
            cd $OPENAIR_TARGETS/ARCH/EXMIMO/DRIVER/eurecom && make clean && make   || exit 1
            cd $OPENAIR_TARGETS/ARCH/EXMIMO/USERSPACE/OAI_FW_INIT && make clean && make   || exit 1
            cd $THIS_SCRIPT_PATH

            make --directory=$OPENAIR_TARGETS/RT/USER $MAKE_LTE_ACCESS_STRATUM_TARGET_LINUX_KERNEL -j`grep -c ^processor /proc/cpuinfo ` || exit 1

            cd $OPENAIR_TARGETS/RT/USER
            bash ./init_exmimo2.sh
            echo_warning "STARTING SOFTMODEM..."
            if [ "x$GNU_DEBUGGER" == "xyes" ]; then
                touch .gdbinit_enb
                echo "file $OPENAIR_TARGETS/RT/USER/lte-softmodem" > ~/.gdbinit_enb
                echo "set args -K /tmp/itti_enb_low_latency.log -V  -O $CONFIG_FILE_ENB" >> ~/.gdbinit_enb
                echo "run" >> ~/.gdbinit_enb
                gdb -nh -x ~/.gdbinit_enb 2>&1 
                #> /tmp/stdout_enb_low_latency.log 
            else
                $OPENAIR_TARGETS/RT/USER/lte-softmodem  -K /tmp/itti_enb_low_latency.log -V  -O $CONFIG_FILE_ENB
            fi
            cd $THIS_SCRIPT_PATH
        else
            echo_error "UNKNOWN REAL-TIME MODE real_time"
        fi
    fi
fi

#cat $STDOUT_LOG_FILE |  grep -v '[PHY]' | grep -v '[MAC]' | grep -v '[EMU]' | \
#                        grep -v '[OCM]' | grep -v '[OMG]' | \
#                        grep -v 'RLC not configured' | grep -v 'check if serving becomes' | \
#                        grep -v 'mac_rrc_data_req'   | grep -v 'BCCH request =>' > $STDOUT_LOG_FILE.filtered
