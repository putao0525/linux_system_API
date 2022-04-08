/*读*/
int  flags = 0；
flags = fcntl(fd, F_GETFL);
/*改，采用位或运算，添加非阻塞标志位*/
flags = flags | O_NONBLOCK;
/*写*/
fcntl(fd, F_SETFL, flags);

