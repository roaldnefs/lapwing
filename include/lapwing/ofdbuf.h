#ifndef INCLUDED_LAPWING_OFDBUF_
#define INCLUDED_LAPWING_OFDBUF_

#include <streambuf>
#include <unistd.h>

namespace lapwing {

// OFdBuf writes to a file descriptor device using an output buffer.
//
// Example:
//    OFdBuf fds(STDOUT_FILENO, 500);
//    std::ostream os(&fdbuf);
class OFdBuf: public std::streambuf {
    // Holds the buffer size.
    size_t d_bufsize;
    // Holds the file descriptor.
    int d_fd;
    // Holds the buffer itself.
    char *d_buffer;

    public:
        OFdBuf();
        OFdBuf(int fd, size_t bufsize = 1);

        virtual ~OFdBuf();
    
        void open(int fd, size_t bufsize = 1);
    private:
        virtual int sync();
        virtual int overflow(int c);
        void cleanup();
};

// The default contstructor merely initializes the buffer to 0.
inline OFdBuf::OFdBuf()
    : d_bufsize(0),
      d_fd(0) {}

// Constructor parsing the file descriptions and buffer size to the open member
// function.
inline OFdBuf::OFdBuf(int fd, size_t bufsize) {
    open(fd, bufsize);
}

// Destructor, calls cleanup.
inline OFdBuf::~OFdBuf() {
    cleanup();
}

// Cleanup calls sync to flush any characters in the output buffer if the
// implementation is using a buffer.
inline void OFdBuf::cleanup() {
    if (d_buffer) {
        sync();
        delete[] d_buffer;
    }
}

// Open initializes the buffer. The begin and the end of the buffer are defined
// using streambuf::setp.
inline void OFdBuf::open(int fd, size_t bufsize) {
    d_fd = fd;
    d_bufsize = bufsize == 0 ? 1 : bufsize;

    cleanup();
    d_buffer = new char[d_bufsize];
    setp(d_buffer, d_buffer + d_bufsize);
}

// Sync flushes the yet unflushed content of the buffer, the buffer is
// reinitialized using streambuf::setp afterwards.
inline int OFdBuf::sync() {
    if (pptr() > pbase()) {
        write(d_fd, d_buffer, pptr() - pbase());
        setp(d_buffer, d_buffer + d_bufsize);
    }
    return 0;
}

// Overflow gets called when the buffer is full, it first calls streambuf::sync
// to flush the buffer and then writes the new character to the now empty
// buffer using the internal streambuf member functions.
inline int OFdBuf::overflow(int c) {
    sync();
    if (c != EOF) {
        *pptr() = c;
        pbump(1);
    }
    return c;
}

} // namespace lapwing

#endif
