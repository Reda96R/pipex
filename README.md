# ::: Pipex :::

`This project will let you discover in detail a UNIX mechanism that you already know by using it in your program.`

# Background theory:

## What is pipe?

Pipe is primarily used in programming on Unix systems, and it is an [interprocess communication](https://en.wikipedia.org/wiki/Inter-process_communication), which means it passes information from one program process to another, but it only offers one-way communication by passing an output from a process (command) to another.

before knowing more about piping, we should have a look at how linux creates processes.

## How does linux create processes?

In general a new process in Linux is created after an existing process makes an exact copy of itsself, these two pocesses (the child and parent), share the same environment, but they differ when it commes to the ID. 

the new pocess is crerated when one of these functions : **`fork, posix_spawn, _Fork, vfork`** is called, in our case we’ll work with [fork()](https://man7.org/linux/man-pages/man2/fork.2.html).

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
		

		ft_free_parent(&process_info);//freeing resources after the execution
		return (0);	
	}
	return (ft_msg(ERR_INPUT)); //in the case of invalide number of arguments
}
```

## Resources:

- [pipes under the hood](https://brandonwamboldt.ca/how-linux-pipes-work-under-the-hood-1518/)
- [this amazing article by Ahmad Mardeni explaining the fork() function](https://www.section.io/engineering-education/fork-in-c-programming-language/#:~:text=What%20is%20a%20Fork()%3F,also%20kills%20the%20child%20process.)
- [pipe system call](https://www.geeksforgeeks.org/pipe-system-call/);
- [how linux creates processes](https://brandonwamboldt.ca/how-linux-creates-processes-1528/)
- [how bash redirection work](https://brandonwamboldt.ca/how-bash-redirection-works-under-the-hood-1512/)
- [understanding the waitpid function](https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-waitpid-wait-specific-child-process-end)
- [decorating the makefile : )](https://stackoverflow.com/questions/5947742/how-to-change-the-output-color-of-echo-in-linux)
