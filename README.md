# ::: Pipex :::

`This project will let you discover in detail a UNIX mechanism that you already know by using it in your program.`

# What is pipe?

Pipe is primarily used in programming on Unix systems, and it is an [interprocess communication](https://en.wikipedia.org/wiki/Inter-process_communication), which means it passes information from one program process to another, but it only offers one-way communication by passing an output from a process (command) to another.

before knowing more about piping, we should have a look at how linux creates processes,

## How does linux create processes?

a new processes in linux can be created using [fork()](https://man7.org/linux/man-pages/man2/fork.2.html) system cal,

## Resources:

- [pipes under the hood](https://brandonwamboldt.ca/how-linux-pipes-work-under-the-hood-1518/)
- [this amazing article by Ahmad Mardeni explaining the fork() function](https://www.section.io/engineering-education/fork-in-c-programming-language/#:~:text=What%20is%20a%20Fork()%3F,also%20kills%20the%20child%20process.)
- [pipe system call](https://www.geeksforgeeks.org/pipe-system-call/)
- [how linux creates processes](https://brandonwamboldt.ca/how-linux-creates-processes-1528/)
