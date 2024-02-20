; https://github.com/torvalds/linux/blob/v6.7/include/uapi/asm-generic/fcntl.h

%ifndef _FCNTL_INC_
%define _FCNTL_INC_

%define O_RDONLY        0q00000000
%define O_WRONLY        0q00000001
%define O_RDWR          0q00000002
%define O_CREAT         0q00000100
%define O_EXCL          0q00000200
%define O_NOCTTY        0q00000400
%define O_TRUNC         0q00001000
%define O_APPEND        0q00002000
%define O_NONBLOCK      0q00004000
%define O_DSYNC         0q00010000
%define O_DIRECT        0q00040000
%define O_LARGEFILE     0q00100000
%define O_DIRECTORY     0q00200000
%define O_NOFOLLOW      0q00400000
%define O_NOATIME       0q01000000
%define O_CLOEXEC       0q02000000

%endif
