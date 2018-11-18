#include <unistd.h>

#include <cstring>

#include "output_pipe.hpp"

using namespace std;

namespace messaging {

OutputPipe::OutputPipe(const int& _outputFD): outputFD(_outputFD) {}

void OutputPipe::close() {
    int ret = ::close(outputFD);
    if (ret < 0) {
        perror("close");
        exit(errno);
    }
}

void OutputPipe::pipe(const Message& message) const {
    const void* bytes = message.getPayload();
    size_t numBytes = message.getSize();
    size_t written = 0;
    while (written < numBytes) {
        uint8_t* target = ((uint8_t*)bytes) + written;
        size_t remaining = numBytes - written;
        ssize_t currentWriteBlockSize = write(outputFD, target, remaining);
        if (currentWriteBlockSize < 0) {
            perror("write");
            exit(errno);
        }
        written += currentWriteBlockSize;
    }
}

}