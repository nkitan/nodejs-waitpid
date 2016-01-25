# node-waitpid2

Originally forked from AvianFlu/node-waitpid changing the behaviour to be more like nature intended. 

You should implement a blocking loop in your application to wait for a child process. You can do this asyncronously by passing `WNOHANG` as an option and using a timeout to call `waitpid` until the PID returned is `-1`.
