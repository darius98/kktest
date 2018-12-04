#ifndef KKTEST_COMMON_INTERPROC_INCLUDE_KKTEST_COMMON_INTERPROC_IMPL_MESSAGE_READER_HPP_
#define KKTEST_COMMON_INTERPROC_INCLUDE_KKTEST_COMMON_INTERPROC_IMPL_MESSAGE_READER_HPP_

#include <string>

#include <kktest_common/interproc_impl/message.hpp>

namespace kktest {
namespace interproc {

class MessageReader {
 public:
    explicit MessageReader(const Message& _message);

    MessageReader& operator<<(std::string& obj);

    template<class T>
    MessageReader& operator<<(T& obj) {
        void* objAddress = static_cast<std::uint8_t*>(message.getPayload()) + cursor;
        obj = *static_cast<T*>(objAddress);
        cursor += sizeof(obj);
        return *this;
    }

    template<class T>
    T read() {
        T obj;
        (*this) << obj;
        return obj;
    }

 private:
    int cursor = sizeof(std::size_t);
    const Message& message;
};

}  // namespace interproc
}  // namespace kktest

#endif  // KKTEST_COMMON_INTERPROC_INCLUDE_KKTEST_COMMON_INTERPROC_IMPL_MESSAGE_READER_HPP_
