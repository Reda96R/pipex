# ::: Pipex :::

`This project will let you discover in detail a UNIX mechanism that you already know by using it in your program.`

# Background theory:

## What is pipe?

Pipe is primarily used in programming on Unix systems, and it is an [interprocess communication](https://en.wikipedia.org/wiki/Inter-process_communication), which means it passes information from one program process to another, but it only offers one-way communication by passing an output from a process (command) to another.

before knowing more about piping, we should have a look at how linux creates processes.

## How does linux create processes?

In general a new process in Linux is created after an existing process makes an exact copy of itsself, these two pocesses (the child and parent), share the same environment, but they differ when it commes to the ID. 

the new pocess is crerated when one of these functions : **`fork, posix_spawn, _Fork, vfork`** is called, in our case we’ll work with `[fork()](https://man7.org/linux/man-pages/man2/fork.2.html)`.

if you want to dive a little bit deeper in this subject you can check [this article](https://subscription.packtpub.com/book/application-development/9781785883057/1/ch01lvl1sec12/process-creation) about process creation in Linux.

## How does fork() work?

fork() will help us run two processes in one single program, it is responsible of creating the child out of the original process, it returns an int value, that can be :

- 0 : if the child process is created,
- > 0 : if it is the parent process,
- < 0 : if there is an error.

## How does piping work ?

first the parent process calls `pipe()` , which is a function that creates a pipe once for each two processes, that get piped together, then shell forks itself using `fork()` once for each process and the child will run one command after modifying its std(in/out) using `dup()`.

let’s see an example to clarify things,

```bash
ls -la | sort
```

1. first thing bash will create a pipe to pipe `ls` to `sort`,  (`pipe()`)
2. next it will fork itself using `fork()`, two times (one parent and two children),  (`fork()`)
3. then child1 (`ls`) will set it’s stdout fd to the write of pipe1,  (`dup()`)
4. after that, child2 (`sort`) will set its stdin fd to the read of pipe1(output of `ls`).  (`dup()`)
5. each of the children will run their commands.

the kernel will handle the scheduling of processes so they can run in parallel.

# Making the code:

The general idea of this project is to read from file1, then execute cmd1, and send its output to cmd2 which will output to file2.

in more technical way, first we’ll use `dup()` to set the input of the first cmd to file1, then `pipe()` will send the output of cmd1 (`execve()`) as input to cmd2 with the help of `dup()` , and `fork()` will make sure to run the two pocesses in one program.

```bash
//every cmd needs a stdin and strdout
														
							 PIPE
						|---------------------|
			 file1 ---> cmd1 ---> end[1]<--------------->end[0] ---> cmd2 ---> file2  
			 (stdin1)		|---------------------|                   (stdout2)
					    (stdout1)	           (stdin2) 					
```

## Resources:

- [pipes under the hood](https://brandonwamboldt.ca/how-linux-pipes-work-under-the-hood-1518/)
- [this amazing article by Ahmad Mardeni explaining the fork() function](https://www.section.io/engineering-education/fork-in-c-programming-language/#:~:text=What%20is%20a%20Fork()%3F,also%20kills%20the%20child%20process.)
- [pipe system call](https://www.geeksforgeeks.org/pipe-system-call/)
- [how linux creates processes](https://brandonwamboldt.ca/how-linux-creates-processes-1528/)
