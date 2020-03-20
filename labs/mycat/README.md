Lab - mycat
===========

Rewrite the `cat2.c` program by using `read`, `write`, `open` and `close` instead of their library equivalents.

Then, generate a report with performance metrics between the existing `cat` command  and your `mycat`.
The format of the report is free, you can add charts or whatever helps to understand the speed of both approaches.

General instructions
--------------------
1. Don't forget to sync first with the base [master](https://github.com/CodersSquad/ap-labs) branch.
2. Push your code to your personal fork.
3. Use the [`cat2.c`](./cat2.c) file for your code.
4. Have fun and don't forget the [next section](#how-to-submit-your-work).


Use instructions
----------------
*There is .sh file called 'cat_stats.sh' in the folder. That file is in charge of running the LINUX cat command and the ./cat2 c file
in order to compare their performance.

*As a build requirement, the .sh file will use ap-get to install bc.

*In order to compare the performance of both the LINUX cat command and the cat2.c file, 
just run ./cat_stats.sh [file name to use for the test]


How to submit your work
=======================
```
GITHUB_USER=<your_github_user>  make submit
```
More details at: [Classify API](../../classify.md)
