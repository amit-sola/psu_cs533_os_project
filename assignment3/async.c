#include "scheduler.h"
#include <aio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

ssize_t read_wrap(int fd, void* buf, size_t count){
    struct aiocb asyncStruct;
    asyncStruct.aio_fildes = fd;
    asyncStruct.aio_offset = 0; //Is this right?
    asyncStruct.aio_buf = buf;
    asyncStruct.aio_nbytes = count;
    asyncStruct.aio_reqprio = 0;
    struct sigevent sigE;
    sigE.sigev_notify = SIGEV_NONE;
    asyncStruct.aio_sigevent = sigE;

    //Call async read
    aio_read(&asyncStruct);
    while (aio_error(&asyncStruct) == EINPROGRESS){
        yield();
    }

    //Seek to end of spot
    ssize_t numRead = aio_return(&asyncStruct);
    lseek(fd, numRead, SEEK_CUR);
    return numRead;
}
