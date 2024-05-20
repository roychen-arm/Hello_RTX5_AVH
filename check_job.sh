#!/usr/bin/env bash

#install vcpkg
#. <(curl https://aka.ms/vcpkg-init.sh -L)
. ~/.vcpkg/vcpkg-init

#activate enviroment
vcpkg activate

#main branch
cbuild Hello_rtos.csolution.yml --context-set --update-rte --packs --context Hello_rtos+AVH

#hello_vsi_new branch
#cbuild ./hello_vsi/hello_vsi.csolution.yml --context-set --update-rte --packs --context hello_vsi.event+Corstone_300

#run FVP with binary file
#FVP_MPS2_Cortex-M7 out/Hello_rtos/AVH/Hello_rtos.axf -C fvp_mps2.mps2_visualisation.disable-visualisation=1
FVP_MPS2_Cortex-M7 out/Hello_rtos/AVH/Hello_rtos.axf --stat --simlimit 24 $*