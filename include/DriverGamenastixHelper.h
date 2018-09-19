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
