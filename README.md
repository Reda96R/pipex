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

- n = 0 : if the child process is created,
- n > 0 : if it is the parent process,
- n < 0 : if there is an error.

//when we call fork(), we’re actually the glibc wrapper, not the fork(2) syscall !!! 

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

in more technical way, first we’ll use `dup()` to set the input of the first cmd to file1, then `pipe()` will send the output of cmd1 (`execve()`) as input to cmd2 with the help of `dup()` , and `fork()` will split the processe in two simultaneous processes that run at the same time.

```bash
PIPE
						|---------------------|
			 file1 ---> cmd1 ---> ends[1]<--------------->ends[0] ---> cmd2 ---> file2  
			 (stdin1)		|---------------------|                   (stdout2)
					    (stdout1)	           (stdin2)
```

The first thing will need to do is to create the pipe using `pipe()`

```c
int main(int ac, char *av[], char *env[])
{
	int ends;
	pipe(ends);		
}
```

we used `env`, that will contain the paths of cmds.

next will need to split our pocess into two parallel processes,

```c
int main(int ac, char *av[], char *env[])
{
	t_info info
	
	pipe(info.ends);
	info.pid = fork();
	if (info.pid < 0)
		return (ft_error(FORK));
	if (!info.pid)
		ft_child(info, av, env);
	else
		ft_parent(info, av, env); 
}
```

we’ll use `t_info info` that contains everything we’ll need to make our code cleaner, it will defined in our header, we also used `[pid_t](https://www.notion.so/Pipex-ed7ba9ca1a4a45ce90010ee4aacbf98a) pid` that will represent the process ID and we’ll check wether **pid** is the parent or the child,

```c
typedef struct s_info
{
	t_pid pid;
	int ends[2];
	int infile;
	int outfile;
	char **cmd_path;
	char **cmd_args;
	char *cmd;
}t_info
```

now let’s make the parent and the child functions, and we’ll start by the child,

```c
void ft_child(t_info info, char *av[], char *env[])
{
	dup2 (info.ends[1], 1); //changing the output of cmd1 to stdout
	close (ends[0]);
	dup2 (info.infile, 0); //setting stdin to the fd of infile
	info.cmd_args = ft_split(av[2], ' '); //getting cmd1 arguments
	info.cmd = ft_getcmd(info.cmd_path, info.cmd_args[0]); //getting the command
	if (!cmd) //incase we have a problem with the command we'll free the process and exit
	{
		ft_free(&info);
		ft_msg(ERR_CMD);
		exit(1);
	}
	execve (info.cmd, info.cmd_args, env); //executing the command
}
```

for the parent process, it’s the same thing, we just need to change the output fom stdout to the outfile, and the input to `ends[0]` and we need to close `ends[1]` , also to make the parent wait for the child to finish it’s process,

```c
void ft_parent(t_info info, char *av[], char *env[])
{
	int status;
	waitpid(-1, &status, 0);
	dup2 (info.ends[0], 0);
	close (ends[1]);
	dup2 (info.outfile, 1);
	info.cmd_args = ft_split(av[3], ' ');
	info.cmd = ft_getcmd(info.cmd_path, info.cmd_args[0]);
	if (!cmd)
	{
		ft_free(&info);
		ft_msg(ERR_CMD);
		exit(1);
	}
	execve (info.cmd, info.cmd_args, env);
}
```

## in the making… =)

## Resources:

- [pipes under the hood](https://brandonwamboldt.ca/how-linux-pipes-work-under-the-hood-1518/)
- [this amazing article by Ahmad Mardeni explaining the fork() function](https://www.section.io/engineering-education/fork-in-c-programming-language/#:~:text=What%20is%20a%20Fork()%3F,also%20kills%20the%20child%20process.)
- [pipe system call](https://www.geeksforgeeks.org/pipe-system-call/)
- [how linux creates processes](https://brandonwamboldt.ca/how-linux-creates-processes-1528/)
- [how bash redirection work](https://brandonwamboldt.ca/how-bash-redirection-works-under-the-hood-1512/)
