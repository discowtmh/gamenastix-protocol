// This file is a part of Treadmill project.
// Copyright 2018 Disco WTMH S.A.

#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>


using Message = std::vector<uint8_t>;

enum class MessageType
{
    CalibrationNPose,
    CalibrationSPose,
    RequestModel,
    ResponseModel,
    Frame,
    BumpFrameRate,
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
                case 'B':
                    return MessageType::BumpFrameRate;
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
