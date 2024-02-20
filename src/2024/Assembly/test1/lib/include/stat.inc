; https://github.com/torvalds/linux/blob/v6.7/include/uapi/linux/stat.h

%ifndef _STAT_INC_
%define _STAT_INC_

%define S_IRWXU 0q0700
%define S_IRUSR 0q0400
%define S_IWUSR 0q0200
%define S_IXUSR 0q0100

%define S_IRWXG 0q0070
%define S_IRGRP 0q0040
%define S_IWGRP 0q0020
%define S_IXGRP 0q0010

%define S_IRWXO 0q0007
%define S_IROTH 0q0004
%define S_IWOTH 0q0002
%define S_IXOTH 0q0001

%endif
