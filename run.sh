
#!/bin/bash

# Author: Akash Maji
# Contact: akashmaji@iisc.ac.in

# Function to run a .out file in a new terminal tab
run_in_new_tab() {
    local file=$1
    gnome-terminal --tab --title="$file" -- bash -c "perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./$file; exec bash"
}

# Check if a parameter is provided
if [ $# -eq 0 ]; then
    # No parameter provided, run all .out files
    for file in *.out; do
        run_in_new_tab "$file"
    done
else
    # Parameter provided, run the specified .out file
    run_in_new_tab "$1"
fi

