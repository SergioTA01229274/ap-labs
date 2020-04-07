Lab  - Advanced Logger
======================

Now it's time to add support for Linux system logging into your logger that you implemented on [logger](https://github.com/CodersSquad/ap-labs/tree/master/labs/logger).

A new `initLogger`function will be added in order to add support for choosing between `stdout` or `syslog` logging.

```
int initLogger(char *logType)
```

You will handle the following log types:
- `stdout` or empty string for `STDOUT` logging
- `syslog` for `SYSLOG` logging

You can use the **The Linux Programming Interface** book as a reference for your implementation. See *37th chapter on 5th section*.


Build and Use instructions
--------------------------
- Run the following commands in order to obtain the executable file that runs the code: `gcc -c logger.c -o logger.o`, `gcc -c testLogger.c -o testLogger.o`, `gcc logger.o testLogger.o -o main`
- Once the files are compiled an executable file "main" will be created
- Run the executable file using `./main` and see the output in stdout. Another output will be logged into syslog. You can check the output in syslog with: `tail /var/log/syslog`



General Instructions
--------------------
- Use the `testLogger.c` file for testing your `logger.c` library implementation.
- You can update the `Makefile` for your compilation and linking.
- Update `README.md` with the proper steps for building and running your code.
- Don't forget to handle errors properly.
- Coding best practices implementation will be also considered.


How to submit your work
=======================
```
GITHUB_USER=<your_github_user>  make submit
```
More details at: [Classify API](../../classify.md)
