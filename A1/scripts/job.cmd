#!/bin/bash
##
## optional: energy policy tags
##
# DO NOT USE environment = COPY_ALL
#@ job_type = MPICH
#@ class = large
#@ node = 1
### schedule the job to 2 to 4 islands 
#@ island_count=1,1
#@ total_tasks= 1
## other example
##@ tasks_per_node = 16
#@ wall_clock_limit = 10:00:00
##                    1 h 20 min 30 secs
#@ job_name = mytest
#@ network.MPI = sn_all,not_shared,us
#@ initialdir = $(home)/mydir
#@ output = job$(jobid).out
#@ error = job$(jobid).err
#@ notification=always
#@ notify_user=eric.dong1234@gmail.com
#@ queue
. /etc/profile
. /etc/profile.d/modules.sh
#setup of environment
module unload mpi.ibm
module load mpi.intel
module load papi
mpiexec -n 1 ./myprog.exe
