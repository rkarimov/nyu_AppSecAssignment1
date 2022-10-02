<h1> Testing <h1>
<p> 

In order to effectively generate a gcov coverage report, I did the following: 
1. Ran gcc -fprofile-arcs -ftest-coverage ./giftcardreaderfixed.c -o ./giftcardreaderfixed which compiled the program and then created a .gcno file 
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
In order to improve coverage, I closely studied the giftcardreaderfixed.c.gcov file to see which lines never executed. From there, my focus was drawn back to the animate function and I closely examined each of the cases, but didn't notice anything out of ordinary. I then looked futher down and found that the type_of_record set in the write can impact coverage. I set this to type_of_record = 2 and generated a cov2.gft file

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


<p>The AFL Fuzzer part was likely the most time consuming out of this whole assignment. 
In order to run this effectivelly, I created two directories (afl_input /. alf_output), the former contained the current crash cases and latter would have the additional crashes genereated </p>