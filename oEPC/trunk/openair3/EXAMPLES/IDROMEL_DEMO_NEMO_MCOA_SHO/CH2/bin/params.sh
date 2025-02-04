#!/bin/bash
DEMO_DIR=$OPENAIR3_DIR"/EXAMPLES/IDROMEL_DEMO_NEMO_MCOA_SHO/CH2"
RADVD_CONF_FILE=$DEMO_DIR/etc/radvd.conf
#----------------------------------------------
HOME_AGENT_NW=2001:4::/32
HOME_AGENT_ADDR_HL=2001:4::1/64
HOME_AGENT_ADDR_CN=2001:1::1/64
#----------------------------------------------
CH1_EGRESS_ETHERNET_INTERFACE=eth0
CH1_INGRESS_ETHERNET_INTERFACE=eth3
CH1_IN6_ADDR=2001:2f0:110:6000::1/64
CH1_IN6_NW=2001:2f0:110:6000::/64
CH1_IN6_ADDR_CN=2001:1::6/64
CH1_IN_ADDR=10.0.1.1/24
CH1_IN_BROADCAST_ADDR=10.0.1.255
#----------------------------------------------
CH2_EGRESS_ETHERNET_INTERFACE=eth0
CH2_INGRESS_ETHERNET_INTERFACE=eth1
CH2_IN6_ADDR=2001:2f0:110:7000::1/64
CH2_IN6_NW=2001:2f0:110:7000::/64
CH2_IN6_ADDR_CN=2001:1::7/64
CH2_IN_ADDR=10.0.2.2/24
CH2_IN_BROADCAST_ADDR=10.0.2.255
#----------------------------------------------
MR_IN_ADDR=10.0.1.3/24
MR_IN_ADDR2=10.0.2.3/24
MR_IN6_ADDR1=2001:2f0:110:6000:3d6:5200:300:1/64
MR_IN6_LLADDR1=fe80:0:0:0:3d6:5200:300:1/64
MR_IN6_ADDR2=2001:2f0:110:7000:3d6:5200:300:2/64
MR_IN6_LLADDR2=fe80:0:0:0:3d6:5200:300:2/64
#----------------------------------------------


