// This file is a part of Treadmill project.
// Copyright 2018 Disco WTMH S.A.

#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

#include <deps/treadmill-t2-biomodel/include/deepModel/Model.h>
#include <BodyPart.h>


using Message = std::vector<uint8_t>;

enum class MessageType
{
    CalibrationNPose,
    CalibrationSPose,
    RequestModel,
    ResponseModel,
    Frame,
    Unknown
};

struct Matcher
{
    static MessageType match(Message message)
    {
        if (message.size() < 1)
        {
            return MessageType::Unknown;
        }
        else
        {
            switch (message[0])
            {
                case 'N':
                    return MessageType::CalibrationNPose;
                case 'S':
                    return MessageType::CalibrationSPose;
                case 'R':
                    return MessageType::RequestModel;
                case 'M':
                    return MessageType::ResponseModel;
                case 'F':
                    return MessageType::Frame;
                default:
                    return MessageType::Unknown;
            }
        }
    }
};

class DriverGamenastix
{
public:
    using MessageHandler = std::function<void(MessageType)>;
    using MessageSender = std::function<void(Message)>;

    DriverGamenastix(biomodel::deepModel::Model& model, MessageHandler messageHandler, MessageSender messageSender)
            : model(model)
            , messageHandler(messageHandler)
            , messageSender(messageSender)
    {
    }

    void handleMessage(Message message)
    {
        auto messageType = Matcher::match(message);
        switch (messageType)
        {
            case MessageType::RequestModel:
            {
                messageSender(model.getSerializedModel());
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
    MessageSender messageSender;
};
