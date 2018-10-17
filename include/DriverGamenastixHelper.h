// This file is a part of Treadmill project.
// Copyright 2018 Disco WTMH S.A.

#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

#include <Protocol.h>
#include <deps/treadmill-t2-biomodel/include/deepModel/Model.h>



class DriverGamenastix
{
public:
    using MessageHandler = std::function<void(MessageType)>;
    using MessageWithArgumentHandler = std::function<void(MessageType, int)>;
    using MessageSender = std::function<void(Message)>;

    DriverGamenastix(
            biomodel::deepModel::Model& model,
            MessageHandler messageHandler,
            MessageWithArgumentHandler messageWithArgumentHandler,
            MessageSender messageSender)
            : model(model)
            , messageHandler(messageHandler)
            , messageWithArgumentHandler(messageWithArgumentHandler)
            , messageSender(messageSender)
    {
    }

    static Message decorateMessage(char c, Message rawMessage)
    {
        Message message(rawMessage.size()+2);
        message[0] = static_cast<uint8_t>(c);
        message[rawMessage.size()+1] = static_cast<uint8_t>('\n');
        std::copy(rawMessage.begin(), rawMessage.end(), message.begin()+1);
        return message;
    }

    void handleMessage(Message message)
    {
        auto messageType = Matcher::match(message);
        switch (messageType)
        {
            case MessageType::RequestModel:
            {
                auto serializedModel = model.getSerializedModel();
                messageSender(decorateMessage('M', serializedModel));
                break;
            }
            case MessageType::CalibrationNPose:
            {
                messageHandler(MessageType::CalibrationNPose);
                break;
            }
            case MessageType::CalibrationSPose:
            {
                messageHandler(MessageType::CalibrationSPose);
                break;
            }
            case MessageType::BumpFrameRate:
            {
                int frameRate;
                sscanf(reinterpret_cast<char*>(&message[1]), "%d", &frameRate);
                messageWithArgumentHandler(MessageType::BumpFrameRate, frameRate);
                break;
        }
            default:
                break;
        }
    }

    void sendFrame()
    {
        messageSender(model.getSerializedFrame());
    }

    biomodel::deepModel::Model model;
    MessageHandler messageHandler;
    MessageWithArgumentHandler messageWithArgumentHandler;
    MessageSender messageSender;
};
