<h1> Testing </h1>

<p> 
In order to effectively generate a gcov coverage report, I did the following: 
1. I Ran 
gcc -fprofile-arcs -ftest-coverage ./giftcardreaderfixed.c -o ./giftcardreaderfixed
this compiled the program and then created a .gcno file 
2. After the executable files were generated, I ran the following: 
./giftcardreaderfixed 1 ./crash1.gft && ./giftcardreaderfixed 1 ./hang.gft
which ran the fixed reader against the crash cases for coverage purposes 
2. Finally, I ran the progam using gcov to see the coverage by running the following: 
gcov -b -c ./giftcardreaderfixed.c 1 ./crash1.gft && ./giftcardreaderfixed.c 1 ./hang.gft

When execution completed, I recived the following stats: 
Lines executed:50.60% of 168
Branches executed:67.74% of 62
Taken at least once:40.32% of 62
Calls executed:33.33% of 45
</p> 


<p>
In order to improve coverage, I closely studied the giftcardreaderfixed.c.gcov file to see which lines never executed. From there, my focus was drawn back to the animate function and I closely examined each of the cases, but didn't notice anything out of ordinary. I then looked futher down and found that the type_of_record set in the write can impact coverage. I set this to type_of_record = 2 and generated a cov2.gft file through running the writer file. Cov1. was generated w/o any changes as its the intial coverage. 

I next ran the following: 

gcov -b -c ./giftcardreaderfixed.c 1 ./fuzz/hang.gft && ./giftcardreaderfixed.c 2 ./fuzz/hang.gft && ./giftcardreaderfixed.c 1 ./fuzz/crash1.gft && ./giftcardreaderfixed.c 2 ./fuzz/crash1.gft && ./giftcardreaderfixed.c 1 ./fuzz/cov1.gft && ./giftcardreaderfixed.c 2 cov1.gft && ./giftcardreaderfixed.c 1 ./fuzz/cov2.gft && ./giftcardreaderfixed.c 2 cov2.gft


When execution completed, I recievd the follwing stats:
File 'giftcardreaderfixed.c'
Lines executed:51.79% of 168
Branches executed:67.74% of 62
Taken at least once:41.94% of 62
Calls executed:37.78% of 45
Creating 'giftcardreaderfixed.c.gcov'

</p>


<p>
The AFL Fuzzer part was likely the most time consuming task out of this whole assignment. It crashed my VM multiple times and after searching for a solution, I realized it was related to VM resource contraints so I had to increase its CPU and Memory to the max in order to get the fuzzer to work properly. 

In order to run this effectivelly, I created two directories (afl_input /. alf_output), the former contained the current crash cases and later would have the additional crashes genereated

First, I ran the following command "AFL_HARDEN=1 afl-gcc giftcardreaderfixed.c -o giftcardReaderFIXED_AFL" and then ran "sudo AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1 afl-fuzz -i afl_input/ -o afl_output/  ./giftcardReaderFIXED_AFL  1 @@" 
I had to add the "AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES" b/c my VM kept freezing up when generating the crash cases, so in order to speed things up and still achieve the objectives of the assignment, I added this flag. 
Source: https://www.cse.psu.edu/~gxt29/teaching/cs447s19/slides/06testingFuzzing.pdf

Out of the generated 11 crash cases, I found that only 3 crash cases worked (resulted in seg error) which I named respectivelly as fuzzer 1 and fuzzer 2. I then studied the 2 selected crash cases and found that 
The first crash, "fuzzer1.gft" appeared to be linked to a logic bug found in line 210 in the reader which treats the record number as a char and not an int; this it can be exploited with large string which can cause the progam to crash. I fixed this by configuring it to an int, so it only stores numbers for its records

The second case "fuzzer2.gft" was linked to amount_added being negative, which can cause a loop in the 0x09, 0x10 locations whch expect a positive variable to be passed. I fixed this by making it unsigned on line 76 as I did in prior fixes, which would prevent this value from ever being negative int which can cause a loop or read into the idenfined location which causes a crash

In order to ensure the two were fixed, I ran them seperately against gift card reader fixed
./giftcardreaderfixed 2 ./fuzz/fuzzer2.gft
./giftcardreaderfixed 1 ./fuzz/fuzzer2.gft

./giftcardreaderfixed 2 ./fuzz/fuzzer1.gft
./giftcardreaderfixed 2 ./fuzz/fuzzer1.gft

Both of them generated output w/o any segmentation errors, which means my fixes worked!

Finally, I reran the coverage check by runnig the following: 
gcov -b -c ./giftcardreaderfixed.c 1 ./fuzz/hang.gft && ./giftcardreaderfixed.c 2 ./fuzz/hang.gft && ./giftcardreaderfixed.c 1 ./fuzz/crash1.gft && ./giftcardreaderfixed.c 2 ./fuzz/crash1.gft && ./giftcardreaderfixed.c 1 ./fuzz/cov1.gft && ./giftcardreaderfixed.c 2 cov1.gft && ./giftcardreaderfixed.c 1 ./fuzz/cov2.gft && ./giftcardreaderfixed.c 2 cov2.gft && ./giftcardreaderfixed.c 2 fuzzer1.gft && ./giftcardreaderfixed.c 2 fuzzer2.gft && ./giftcardreaderfixed.c 1 fuzzer1.gft && ./giftcardreaderfixed.c 2 fuzzer2.gft


Please note that I adjust the record to 2 to see if I can increase the test coverage in order to align with my original test above. In this case, the coverage went up noticably and I received the following output

File 'giftcardreaderfixed.c'
Lines executed:72.62% of 168
Branches executed:90.32% of 62
Taken at least once:62.90% of 62
Calls executed:73.33% of 45
Creating 'giftcardreaderfixed.c.gcov'


In conclusion, the fuzzing part was the most time consuming part of this assignment as it took me a while to allocate system resources to run it effectively, but even then it crashed, so i had to find workaround like setting certain flags to decrease resource use. 
</p>