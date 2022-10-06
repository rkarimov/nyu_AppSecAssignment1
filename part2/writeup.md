<h1> Bugs </h1>

<h3> Crash 1</h3>
<p> I disovered the initial crash case by examining how num_bytes is interpreted by the giftcardreader file. It's there were I noticed the comment that return values aren't checked. From there, I tested a negative value passed to malloc function would result in a buffer overflow which is converted to a unsigned value (large positive value) that cannot be allocated within the given size of malloc (memory allocation), thus resulting in overflow which causes a segmentation fault. 
</p>

<h3> Crash 1 - Fix </h3>
<p> In order to fix Crash 1, I had to inspect the reader file memory allocation functions and found that creating an unsigned int value for alloc and reading it in will fix the crash as unsigned int can never be negative and thus will mitigate the overflow issue </p>

</h3> Crash 2 </h3>
<p> The second crash case did not require a crash file to be passed to the reader. As a matter fact, thats the result of the crash. When you run the giftcardreader without any .gft file or any file at all, their is a segementation fault. When speaking with Kevin and a TA about this, it was concluded that this is indeed an acceptable crash case (does not produce a crash gft itself though)

</h3> Crash 2 -fix  </h3>
<p> The second crash was fairly easy to fix as I added validation for number of arguments passed in the final int main() function in reader file which rejected any null or files with less than 3 arguments</p>

<h3> Hang Bug </h3>

<p> I discovered the hang bug by analyzing the while loop within the animate function. Furthermore, I found that i'm able to trigger a hang as the char *pc variable is supposed to be positive setting a negative value within the loop creates an infinite loop as the while condition for break will never be met b/c the negative value is passed. Specifically, you can pass a negative value to case 0x09 which triggerd an infinite loop at the final check 'if (pc > program+256)' as pc variable will constantly be less than program+256 which triggers a loop<p> 

<h3> Hang Bug - Fix </h3>

<p> In order to fix this issue, I simply set  argument in case 0x09 to an unsigned char, thus it cannot be negative which is what triggers the infinite loop <p> 