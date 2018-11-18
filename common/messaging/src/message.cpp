#include <cstring>

#include "message.hpp"

using namespace std;

namespace messaging {

BytesConsumer& BytesConsumer::add(const string& obj) {
    add(obj.size());
    addBytes(obj.c_str(), obj.size());
    return *this;
}

BytesCounter& BytesCounter::addBytes(const void*, size_t numBytes) {
    bytesConsumed += numBytes;
    return *this;
}

size_t BytesCounter::getNumBytesConsumed() const {
    return bytesConsumed;
}

Message Message::build(const function<void(BytesConsumer&)>& builder) {
    BytesCounter counter;
    builder(counter);
    Message message(counter.getNumBytesConsumed());
    builder(message);
    return message;
}

size_t Message::isSane(const void* ptr, size_t size) {
    if (size < sizeof(size_t)) {
        return 0;
    }
    size_t expectedSize = (*((size_t*)ptr)) + sizeof(size_t);
    return expectedSize <= size ? expectedSize : 0;
}

Message::Message(size_t size): payload(malloc(size + sizeof(size_t))) {
    memcpy(payload, &size, sizeof(size_t));
    cursor = sizeof(size_t);
}

Message::Message(const Message& other) {
    size_t size = *((size_t*)other.payload);
    payload = malloc(size + sizeof(size_t));
    memcpy(payload, other.payload, cursor);
    cursor = other.cursor;
}

Message::Message(Message&& other) noexcept: payload(other.payload), cursor(other.cursor) {
    other.payload = nullptr;
}

Message::Message(void* _payload): payload(_payload), cursor(0) {}

Message::~Message() {
    if (payload != nullptr) {
        free(payload);
    }
}

Message& Message::operator=(const Message& other) {
    if (payload != nullptr) {
        free(payload);
    }
    size_t size = *((size_t*)other.payload);
    payload = malloc(size + sizeof(size_t));
    memcpy(payload, other.payload, cursor);
    cursor = other.cursor;
    return *this;
}

Message& Message::operator=(Message&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    payload = other.payload;
    cursor = other.cursor;
    other.payload = nullptr;
    return *this;
}

Message& Message::addBytes(const void* bytes, size_t numBytes) {
    memcpy((uint8_t*)payload + cursor, bytes, numBytes);
    cursor += numBytes;
    return *this;
}

void* Message::getPayload() const {
    return payload;
}

size_t Message::getSize() const {
    return *((size_t*)payload) + sizeof(size_t);
}

}