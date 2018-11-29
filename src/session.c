#include "main.h"
#include "json.h"
#include "json_tokener.h"

int do_use_fd (int efd, struct epoll_event ev) {
    int         cfd                 = ev.data.fd;
    int         sendflags           = 0; 
    int         sizen               = 0; 
    int         readn               = 0; 


    char        buf_in[1024]        = { '\0' }; 
    char        readbuf[1024]       = { '\0' }; 

    while(1) {
        DEBUG("");
        readn = read (cfd, readbuf, 255); 
        if (readn <0 ) { 
            if (EAGAIN == errno ) { 
                break; 
            } 
            do_del_fd(efd, cfd); 
            close (cfd); 
            printf ("Close fd %d by %d\n", cfd,readn); 
            perror("Closed"); 
            return -1; 
        } 
        sizen += readn; 
        if (sizen >= 1024) { 
            do_del_fd(efd, cfd); 
            close (cfd); 
            printf ("Close buffer full fd %d by %d\n", cfd,readn); 
            perror("Closed"); 
            return -1; 
        }
        DEBUG("");
        memcpy(buf_in,readbuf,readn);
        break;
    } 

#ifdef USE_ETONESHOT 
    // re-set fd to epoll 
    do_modify_fd(efd, cfd); 
#endif // USE_ETONESHOT 

    //memset (buf_in, 0x00, 256); 
    readn = read (cfd, buf_in, 255); 
    // if it occured ploblems with reading, delete from epoll event pool and close socket 
    if (readn <= 0) {
        do_del_fd(efd,cfd); 
        close (cfd); 
        printf ("Close fd %d", cfd); 
    } else { 
        printf ("read data %s", buf_in); 
        send (cfd, buf_in, strlen (buf_in), sendflags); 
    } 
    return 1; 
} 

