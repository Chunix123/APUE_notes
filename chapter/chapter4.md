# 文件和目录

## 4.1 引言

本章将描述文件系统的其他特征和文件的性质。

## 4.2 函数stat、fstat、fstatat和lstat

4个stat函数以及他们的返回信息。

```c
#include <sys/stat.h>

int stat(const char *restrict pathname, struct stat *restrict buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *restrict pathname, struct stat *restrict buf);
int fstatat(int fd, const char *restrict pathname, struct stat *restrict buf, int flag);
											所有4个函数的返回值：若成功，返回0；若出错，返回-1
```

* stat函数给出pathname，将返回与此命名有关的信息结构。

* fstat函数获得已在描述符fd上打开文件的有关信息。

* lstat函数类似于stat，但是当命名的文件是一个符号链接时，返回该符号链接的有关信息，而不是由该符号链接引用的文件的信息。

* fstatat函数为一个相对于当前打开目录（由fd参数指向）的路径名返回文件统计信息。

 		flag参数控制着是否跟随着一个符号链接:

>>1.当AT_SYMLINK_NOFOLLOW标志被设置时，fstatat不会跟随符号链接，而是返回符号链接本身的信息;
>>2.在默认情况下，返回的是符号链接所指向的实际文件的信息;

 		fd参数的值

>>1.是AT_FDCWD，并且pathname参数是一个相对路径名， fstatat会计算相对于当前目录的pathname参数;
>>2.如果pathname是一个绝对路径，fd参数就会被忽略;

​		这两种情况下，根据flag的取值，fstatat的作用就跟stat或lstat一样

## 4.3 文件类型

文件类型包括如下几种:

1. 普通文件（regular file）。这是最常用的文件类型，这种文件包含了某种形式的数据。至于这种数据是文本还是二进制数据，对于UNIX内核而言并无区别。对普通文件内容的解释由处理该文件的应用程序进行。
2. 目录文件（directory file）。这种文件包含了其他文件的名字以及指向与这些文件有关信息的指针。对一个目录文件具有读权限的任一进程都可以读该目录的内容，但只有内核可以直接写目录文件。进程必须使用本章介绍的函数才能更改目录。
3. 块特殊文件（block special file）。这种类型的文件提供对设备（如磁盘）带缓冲的访问，每次访问以固定长度为单位进行。
4. 字符特殊文件（character special file）。这种类型的文件提供对设备不带缓冲的访问，每次访问长度可变。系统中的所有设备要么是字符特殊文件，要么是块特殊文件。
5. FIFO。这种类型的文件用于进程间通信，有时也称为命名管道（named pipe）。15.5节将对其进行说明。
6. 套接字（socket）。这种类型的文件用于进程间的网络通信。套接字也可用于在一台宿主机上进程之间的非网络通信。第16章将用套接字进行进程间的通信。
7. 符号链接（symbolic link）。这种类型的文件指向另一个文件。4.17节将更多地描述符号链接。

## 4.4 设置用户ID和设置组ID



## 4.5 文件访问权限

## 4.6 新文件和目录的所有权

## 4.7 函数access和faccessat

## 4.8 函数umask

## 4.9 函数chmod、fchmod和fchmodat

## 4.10 粘着位

## 4.11 函数chown、fchowm、fchownat和lchown

## 4.12 文件长度

## 4.13 文件截断

## 4.14 文件系统