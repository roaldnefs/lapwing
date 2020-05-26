#ifndef INCLUDED_LAPWING_IFDBUF_
#define INCLUDED_LAPWING_IFDBUF_

#include <streambuf>
#include <unistd.h>
#include <cstring>

namespace lapwing {

// IFdBuf reads from a file descriptor device using an input buffer.
//
// Example:
//    IFdBuf fds(STDIN_FILENO, 30);
//    char buf[80];
//    fds.sgetn(buf, 80);  // reads block of 80 chars
class IFdBuf: public std::streambuf {
    // Holds the buffer size.
    size_t d_bufsize = 0;
    // Holds the file descriptor.
    int d_fd = -1;
    // Holds the buffer itself.
    char *d_buffer = 0;

    public:
        IFdBuf();
        IFdBuf(int fd, size_t bufsize = 1);

        virtual ~IFdBuf();
    
        void open(int fd, size_t bufsize = 1);
    private:
        virtual int underflow();
        virtual std::streamsize xsgetn(char *dest, std::streamsize n);
        void cleanup();
};

// The default contstructor merely initializes the buffer to 0.
inline IFdBuf::IFdBuf()
{}

// Constructor parsing the file descriptions and buffer size to the open member
// function.
inline IFdBuf::IFdBuf(int fd, size_t bufsize) {
    open(fd, bufsize);
}

// Destructor, calls cleanup.
inline IFdBuf::~IFdBuf() {
    cleanup();
}

// Cleanup deletes the old buffer and closes the file descriptor.
inline void IFdBuf::cleanup() {
    if (d_buffer)
        delete[] d_buffer;

    // TODO allow the file descriptor to stay open for re-use.
    if (d_fd != -1)
        close(d_fd);
}

// Open initializes the buffer. After allocating the buffer streambuf::setg is
// called.
inline void IFdBuf::open(int fd, size_t bufsize) {
    cleanup();
    
    d_fd = fd;
    d_bufsize = bufsize;
    d_buffer = new char[d_bufsize];

    setg(d_buffer, d_buffer + d_bufsize, d_buffer + d_bufsize);
}

// Underflow fills the buffer by reading from the file descriptor. It returns
// EOF if it fails.
inline int IFdBuf::underflow() {
    if (gptr() < egptr())
        return *gptr();

    int nread = read(d_fd, d_buffer, d_bufsize);

    if (nread <= 0)
        return EOF;

    setg(d_buffer, d_buffer, d_buffer + nread);
    return static_cast<unsigned char>(*gptr());
}

// xsgetn gets sequence of characters.
inline std::streamsize IFdBuf::xsgetn(char *dest, std::streamsize n) {
    int nread = 0;

    while (n) {
        if (!in_avail()) {
            if (underflow() == EOF)
                break;
        }

        int avail = in_avail();

        if (avail > n)
            avail = n;

        memcpy(dest + nread, gptr(), avail);
        gbump(avail);

        nread += avail;
        n -= avail;
    }
    return nread;
}

} // namespace lapwing

#endif
