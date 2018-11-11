#ifndef KKTEST_UTILS_MESSAGE_H_
#define KKTEST_UTILS_MESSAGE_H_

#include <functional>
#include <string>


namespace kktest {

class BytesConsumer {
public:
    virtual BytesConsumer& addBytes(const void* bytes, std::size_t numBytes) = 0;

    // Specialization for string, for better usage.
    BytesConsumer& add(const std::string& str);

    template<class T>
    BytesConsumer& add(const T& obj) {
        return addBytes(&obj, sizeof(obj));
    }

    template<class T>
    BytesConsumer& operator<<(const T& obj) {
        return add(obj);
    }
};

class BytesCounter: public BytesConsumer {
public:
    BytesCounter& addBytes(const void* bytes, std::size_t numBytes) override;

    std::size_t getNumBytesConsumed() const;

private:
    std::size_t bytesConsumed = 0;
};

class Message: public BytesConsumer {
public:
    static Message build(const std::function<void(BytesConsumer&)>& builder);

    explicit Message(std::size_t size);
    Message(const Message& other);
    Message(Message&& other) noexcept;

    ~Message();

    Message& addBytes(const void* bytes, size_t numBytes) override;

    void* getPayload() const;

    std::size_t getSize() const;

private:
    void* payload;
    std::size_t cursor;
};

}

#endif
