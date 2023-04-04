# ::: pipex :::
`This project will let you discover in detail a UNIX mechanism that you already know by using it in your program.`

# Background theory:

## What is a process?

In simple words, **a program in the state of execution is what we call a process**, you didn’t get it? okay let’s start from the beginning,

As we know a pogram is a set of instructions that we give to the computer in order to be executed, and this program with all of its files, is saved on the hard drive, but once we execute it, the system loads its instruction into the RAM and executes them, in this case it is no more a program, it is now a ***process** (with fancy italian accent),* and each process has it’s own stack, heap, etc.., this will make sure that each process is a distinct entity, and therefore a process cannot easily access another’s memory or instructions.

still no idea what I’m talking about? check [this article](https://www.tutorialspoint.com/operating_system/os_processes.htm).

## Life Cycle of a process:

> ***“SPOILER ALERT: We all die in the end.”***
> 
> 
> Stewart Stafford
> 

the life of our process (let’s call it Antonio because why not : ) ) starts when we execute the program, next Antonio will be waiting fo a processor to be allocated to him by the OS scheduler,

after being assigned to a processor, Antonio’s state is set to running and the processor starts executing his instructions, Antonio can move to the waiting state  if he needs to wait for a resource, if not he keeps running until he finishes the execution, or he’s killed by the murderous OS, that’s when our poor friend moves to his last state, the terminated state where he waits to be removed from main memory.

<aside>
💡 to see all active processes on a UNIX system, you can use the following command:

</aside>

```bash
$ps -e
```

## How does Linux create processes?

In general a new process in Linux is created after an existing process makes an exact copy of itself, these two processes (the child and parent), share the same environment, but they differ when it comes to the ID.

the new process is created when one of these functions : **`fork, posix_spawn, _Fork, vfork`** is called, in our case we’ll work with [fork()](https://man7.org/linux/man-pages/man2/fork.2.html).

if you want to dig a little bit deeper in this subject you can check [this article](https://subscription.packtpub.com/book/application-development/9781785883057/1/ch01lvl1sec12/process-creation) about process creation in Linux.

## How does fork() work?

first let’s have a look at fork()’s prototype from `<unistd.h>`:

```c
pid_t fork(void);
```

fork() will help us run two processes in one single program, it is responsible of creating the child out of the original process(the parent), it returns an int value of type pid_t, that can be :

- n = 0 : if the child process is created, (child receives 0 as PID)
- n > 0 : if it is the parent process, (the parent receives the child’s PID)
- n < 0 : if there is an error. (-1)

an important aspect is that the child inherits the parent’s instructions pointer, wait what’s that?

an instruction pointer aka Program Counter, is a processor register that indicates the current address of the instruction being executed and automatically incremented to the next one, this means that when the child is made, it will be at the same place in the code that the parent is, but it doesn’t start from the beginning.

## Wait!!! :

in general the parent process doesn’t wait for its children, so in some cases when the child takes some time in executing, the parent will finish executing and give us the output before the child is even terminated, in this case we call it an **Orphan process**, and it is adopted by [init](https://en.wikipedia.org/wiki/Init) (it’s PID becomes 1) and it is quickly eliminated from the system.

### Watch out from Zombies!!!

yes you’ve heard it right, we have **zombie processes**, they’re processes that has completed execution but stay in the system , waiting for the parent to acknowledge it, in more technical way, the parent has not yet called the `wait()` or `waitpid()` systemcalls to read the child’s exit status and remove its entry from the process table.

in general a zombie process is peaceful, because the system frees its resources (data, execution stack…) but its process control block will be saved, and therefore it consumes no energy or memory space, and it is automatically eliminated by `init` if they become orphan, but if the parent is never intended to end (server, background process, etc…), and it creates children regularly without waiting for them, in this case the process table might get saturated, and that would block the system, preventing the execution of new processes.

if you’re a fan of The Walking Dead or want to know more about zombie processes, check [this article](https://www.it-connect.fr/les-processus-zombies/) (it is in French) about zombie processes, and how to create them.

if we want the parent process to take good care of his children we can use the `wait` and `waitpid` system calls.

## wait() and waitpid() :

we can use the wait or the waitpid system calls, to make the parent wait for his children to finish their execution,

```c
#include <sys/wait.h>
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int option);
```

the `wait()` system call make the parent wait until  one of the children is done, while `waitpid` suspends execution of the parent process until the child specified by pid changes its state, by default it waits only for the specified child, but that can be modified thanks to the `option` argument.

**to make sure we don’t have confusion, we’ll rename pid to n,**

- **status:** a pointer to int that is used by wait and waitpid to store the child’s exit status, this status can help us know if the child finished normally or was terminated.
- **n:** the PID of the child we’re going to wait for, it can be:
    - n = -1: in this case we wait for any child process,
    - n = 0: wait for any child process whose pgid is equal to that of the parent,
    - n < -1: it’s the same as `n = 0`, but the difference here is that we specify the pgid, meaning we wait for any child whose pgid is equal to the absolute value of n,
    - n > 0: wait for the child whose PID is equal to the value of n.
- **option:** The child state changes to wait for are specified by ORing one or more of the flags specified in [waitpid(2)](https://linux.die.net/man/2/waitpid).

<aside>
💡 **pgid**: a process group id is a unique identifier assigned to one or more processes, it is created when a process creates a new child, and they both belong to the same group, it is used by the operating system to identify and manage all processes in a single group as a unit.

</aside>

both `wait()` and `waitpid()` return an int that contains information about the way which a child finished its execution, we can have a lot of information based on this status int thanks to several macros,

- **`WIFEXITED**(status)`: returns true if the child terminated normally, for example by calling exit or finishing through the main function.
- **`WEXITSTATUS**(status)`: to be used only if `WIFEXITED` returned true. Returns the child’s exit code, meaning the number the child specified to the exit function or in the main function’s return.
- **`WIFSIGNALED**(status)`: returns true if the child was forcefully terminated by a [signal](http://www.cs.kent.edu/~ruttan/sysprog/lectures/signals.html).
- **`WTERMSIG**(status)`: to be used only if `WIFSIGNALED` returned true. Returns the signal number that provoked the child’s termination.

## What is pipe?

Pipe is primarily used in programming on Unix systems, and it is an [inter-process communication](https://en.wikipedia.org/wiki/Inter-process_communication), which means it passes information from one program process to another, but it only offers one-way communication by passing an output from a process (command) to another, where a process writes to the write end of the pipe, the data is then stored in a memory buffer until it is read by another process from the pipe’s read end

a pipe is a sort of a file stored outside of the file system, that has no name nor a particular attribute, but thanks to its two fd’s we can deal with it as a file, because when we create a pipe, we get two fd’s, the first one is opened in read only mode, the other in write only mode.

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

in more technical way, first we’ll use `dup2()` to set the input of the first cmd to file1, then `pipe()` will send the output of cmd1 (`execve()`) as input to cmd2 with the help of `dup2()` , and `fork()` will split the processes in two simultaneous processes that run at the same time.

```
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
	t_process_info process_info
	
	pipe(process_info.ends);
	process_info.pid = fork();
	if (process_info.pid < 0)
		return (ft_error(FORK));
	if (!process_info.pid)
		ft_child(process_info, av, env);
	else
		ft_parent(process_info, av, env);
}
```

in order to make the code cleaner and easy to understand, we’ll use `t_process_info process_info` that contains all the informations we’ll need, it will defined in our header, we also used [pid_t](https://www.notion.so/Pipex-ed7ba9ca1a4a45ce90010ee4aacbf98a) pid that will represent the process ID and we’ll check wether **pid** is the parent or the child,

```c
typedef struct s_process_info
{
	t_pid pid;
	int ends[2];
	int infile;
	int outfile;
	char **cmd_path;
	char **cmd_args;
	char *cmd;
}t_process_info
```

now let’s make the parent and the child functions, and we’ll start by the child,

```c
void ft_child(t_process_info process_info, char *av[], char *env[])
{
	dup2 (process_info.ends[1], 1); //changing the output of cmd1 to stdout
	close (process_info.ends[0]);
	dup2 (process_info.infile, 0); //setting stdin to the fd of infile
	process_info.cmd_args = ft_split(av[2], ' '); //getting cmd1 arguments
	process_info.cmd = ft_getcmd(process_info.cmd_path, process_info.cmd_args[0]); //getting the command
	if (!process_info.cmd) //incase we have a problem with the command we'll free the process and exit
	{
		ft_free(&process_info);
		ft_msg(ERR_CMD);
		exit(1);
	}
	execve (process_info.cmd, process_info.cmd_args, env); //executing the command
}
```

for the parent process, it’s the same thing, we just need to change the output fom stdout to the outfile, and the input to `ends[0]` and we need to close `ends[1]` , also to make the parent wait for the child to finish it’s process,

```c
void ft_parent(t_process_info process_info, char *av[], char *env[])
{
	dup2 (process_info.ends[0], 0);
	close (process_info.ends[1]);
	dup2 (process_info.outfile, 1);
	process_info.cmd_args = ft_split(av[3], ' ');
	process_info.cmd = ft_getcmd(process_info.cmd_path, process_info.cmd_args[0]);
		if (!process_info.cmd)
	{
		ft_free(&process_info);
		ft_msg(ERR_CMD);
		exit(1);
	}
	execve (process_info.cmd, process_info.cmd_args, env);
}
```

in oder to prevent leaks, we need to free all the resources used by the child and the parent after the execution of the commands or in the case of an error,

we’ll start with the parent,

```c
void ft_free_parent(t_process_info *process_info)
{
	int i;

	i = 0;
	close (process_info->infile);
	close (process_info->outfile);
	while (process_info->cmd_path)
	{
		free(process_info->cmd_path[i]);
		i++;
	}
	free (process_info->cmd_path);
}
```

we first close the file descriptor for infile and the outfile using the `close()` function. Then we iterates through the `cmd_path` array in the `t_process_info` struct and we free the memory allocated for each element of the array using the `free()` function. Finally, we free the memory allocated for the `cmd_path` array itself.

now let’s deal with the child,

```c
void ft_free_child(t_process_info *process_info)
{
	int i;

	i = 0;
	while (process_info->cmd_args[i])
	{
		free(process_info->cmd_args[i]);
		i++;
	}
	free (process_info->cmd_args);
	free (process_info->cmd);
}
```

 we first iterate through the `cmd_args` array in the `t_process_info` struct and free the memory allocated for each element of the array using the `free()` function. Then we free the memory allocated for the `cmd_args` array itself. Lastly, we free the memory allocated for the `cmd` field in the `t_process_info` struct.

<aside>
💡 A pointer is used as an argument here (`*process_info`) because it allows the function to modify the struct and its members directly, rather than working with a copy of the struct, because passing a struct by value creates a copy of the struct on the stack and any modifications made to the struct within the function will not be reflected in the original struct, and by passing a pointer to the struct, the function can access and modify the original struct, and any changes made to the struct within the function will be reflected in the original struct after the function is done executing.
Additionally, using a pointer also save memory, as it does not need to copy the entire struct on the stack, just the memory address is passed, which is much smaller.

</aside>

now let’s structure our code together,

```c
#include "pipex.h"
char *ft_path_finder(char *env[]) // this function will find the paths to cmds
{
	while(ft_strncmp("PATH", *env, 4))
		env++;
	return (*env + 5);
}
```

This function will take the environment variable (`char *env[]`) and iterates through it until it finds the "PATH" variable. Once it finds "PATH", it returns a pointer to the value of the PATH variable with the first five characters (which appear to be "PATH=") removed. So this function is used to find the PATH variable in the environment and return the path string after "PATH=".

to make things clearer, if we execute echo `$PATH` which will print the value of the PATH variable, we’ll get something like this :

```c
/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki
```

<aside>
💡 The PATH environment variable is a system variable that contains a list of directories, separated by a semicolon (on Windows) or a colon (on Linux and macOS), where the operating system should look for executable files, so when you type a command into the command prompt or terminal, the operating system looks for that command in the directories listed in the PATH variable in the order they are specified. If the command is found in one of the directories, it is executed. If the command is not found in any of the directories listed in the PATH variable, the operating system will return an error message indicating that the command could not be found.

</aside>

```c
#include "pipex.h"
char *ft_path_finder(char *env[]) // this function will find the paths to cmds
{
	while(ft_strncmp("PATH", *env, 4))
		env++;
	return (*env + 5);
}

void ft_close_pinpe(t_process_info *process_info)
{
	close(process_info->ends[0]);
	close(process_info->ends[1]);
}

int main(int ac, char *av[], char *env[])
{
	t_process_info process_info;

	if (ac == 5)
	{
		/*:::first thing is to open the in & out files:::*/
		process_info.infile = open(av[1], O_RDONLY);
		if (pocess_info.infile < 0)
			ft_errormsg(ERR_INFILE);
		process_info.outfile = open(av[ac - 1], O_TRUNC | O_CREAT | O_RDWR, 0777);
		if (process_info.outfile < 0)
			ft_errormsg(ERR_OUTFILE);
		/*:::::::::::::::::::::::::::::::::::::::::::::::*/

		/*:::now we get the paths for the commands:::*/
		process_path = ft_path_finder(env);
		process_info.cmd_path = ft_split(process_info.cmd_path, ':');
		/*:::::::::::::::::::::::::::::::::::::::::::::::*/
		
		process_info.pid = fork();
		if (process_info.pid < 0)
			ft_errormsg(ERR_FORK);
	
		
		/*:::checking the output of fork and executing each command:::*/
		if (!process_info.pid)
			ft_child(process_info, ac, env);
		else
			ft_parent(process_info, ac, env);
		/*:::::::::::::::::::::::::::::::::::::::::::::::*/
		
		ft_close_pipe(&process_info);
		ft_free_parent(&process_info);//freeing resources after the execution
		return (0);	
	}
return (ft_msg(ERR_INPUT)); //in the case of invalide number of arguments
}
```

<aside>
💡 The O_TRUNC flag is used when opening a file to truncate its length to 0. This means that any existing data in the file will be deleted and the file will be left empty. This can be useful in situations where the file is being used to store temporary data or when the file is being used to store data that is being continually overwritten, such as a log file. It also ensures that the file is of the expected size before using it, avoiding issues with reading stale data or data from previous runs.

</aside>

the problem with this implementation is that the two processes do not run in simultaneously, so the solution is to have two children processes instead of one, they will run at the same time while the parent will supervise them until they’re done,

```c
typedef struct s_process_info
{
	t_pid fchild;
	t_pid schild
	int status;
	int ends[2];
	int infile;
	int outfile;
	char **cmd_path;
	char **cmd_args;
	char *cmd;
}t_process_info
```

```c
int main(int ac, char *av[], char *env[])
{
	t_process_info process_info;
	
	if (ac == 5)
	{
		process_info.infile = open(av[1], O_RDONLY);
		if (pocess_info.infile < 0)
			ft_errormsg(ERR_INFILE);
		process_info.outfile = open(av[ac - 1], O_TRUNC | O_CREAT | O_RDWR, 0777);
		if (process_info.outfile < 0)
			ft_errormsg(ERR_OUTFILE);
		process_path = ft_path_finder(env);
		process_info.cmd_path = ft_split(process_info.cmd_path, ':');
		
		/*:::we'll fork two times (two children):::*/
		process_info.fchild = fork(); // First child
		if (!process_info.fchild)
				ft_fchild(process_info, av, env);
		process_info.schild = fork();// Second child
		if (!process_info.schild)
			ft_schild(process_info, av, anv);
		/*:::::::::::::::::::::::::::::::::::::::::::::::*/
		
		ft_close_pipe(&process_info);
		waitpid(process_info.fchild, &process_info.status, 0);
		waitpid(process_info.schild, &process_info.status, 0);
		ft_free_parent(&process_info);
		return (0);
	}
	return (ft_msg(ERR_INPUT));
}
```

## Resources:

- [pipes under the hood](https://brandonwamboldt.ca/how-linux-pipes-work-under-the-hood-1518/)
- [this article by Ahmad Mardeni explaining the fork() function](https://www.section.io/engineering-education/fork-in-c-programming-language/#:~:text=What%20is%20a%20Fork()%3F,also%20kills%20the%20child%20process.)
- [pipe system call](https://www.geeksforgeeks.org/pipe-system-call/)
- [how linux creates processes](https://brandonwamboldt.ca/how-linux-creates-processes-1528/)
- [how bash redirection work](https://brandonwamboldt.ca/how-bash-redirection-works-under-the-hood-1512/)
- [understanding the waitpid function](https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-waitpid-wait-specific-child-process-end)
- [decorating the makefile : )](https://stackoverflow.com/questions/5947742/how-to-change-the-output-color-of-echo-in-linux)
- [wait and waitpid](https://www.notion.so/Pipex-ed7ba9ca1a4a45ce90010ee4aacbf98a)
