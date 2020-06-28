

#pragma once


#include "muduo/base/noncopyable.h"
#include "muduo/net/Callbacks.h"

#include <google/protobuf/message.h>

#include <map>

typedef boost::shared_ptr<google::protobuf::Message> MessagePtr;


class ProtobufDispatcherLite : muduo::noncopyable
{
public:
    typedef boost::function<void(const muduo::net::TcpConnectionPtr&,
                                 const MessagePtr&,
                                 muduo::Timestamp)> ProtobufMessageCallback;

    explicit ProtobufDispatcherLite(const ProtobufMessageCallback& defaultCb)
        :defaultCallback_(defaultCb) {
    }

    void onProtobufMessage(const muduo::net::TcpConnectionPtr& conn,
                           const MessagePtr& message,
                           muduo::Timestamp receiveTime) const {
        CallbackMap::const_iterator it = callbacks_.find(message->GetDescriptor());
        if (it != callbacks_.end()) {
            it->second(conn, message, receiveTime);
        } else {
            defaultCallback_(conn, message, receiveTime);
        }
    }

    void registerMessageCallback(const google::protobuf::Descriptor* desc,
                                 const ProtobufMessageCallback& callback)
    {
        callbacks_[desc] = callback;
    }

private:
    typedef std::map< const google::protobuf::Descriptor*, ProtobufMessageCallback > CallbackMap;
    CallbackMap callbacks_;
    ProtobufMessageCallback defaultCallback_;
};
