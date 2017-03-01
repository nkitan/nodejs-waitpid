# node-waitpid2

Originally forked from AvianFlu/node-waitpid changing the behaviour to be more like nature intended.

The `waitpid` syscall is used to [wait for a process to change state](https://linux.die.net/man/2/waitpid).

For terminated child processes, waiting for them releases the resources associated with the child in the process table. If you don't do this, they could remain zombies for ever!

You should implement a blocking loop in your application in order to wait for a child process. You should do this asyncronously by passing `WNOHANG` as an option and using a timeout to call `waitpid` until the PID returned is `-1`. Don't let the `waitpid` call block the whole thread.

Version 2.0.0 moves to the latest node addon/v8 API.
