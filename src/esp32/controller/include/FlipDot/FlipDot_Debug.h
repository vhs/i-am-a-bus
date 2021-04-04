#pragma once

#include <Arduino.h>

#include "FlipDot/FlipDot_Protocol.h"

class FlipDotDebugger
{
public:
    String getFrameType(uint8_t msgType, uint8_t *msgData, uint8_t msgDataLen)
    {
        String result = "";

        switch (msgType)
        {
        case FLIPDOT_MSG_TYPE_SEND_DATA:
            result = "FLIPDOT_MSG_TYPE_SEND_DATA";
            break;
        case FLIPDOT_MSG_TYPE_DATA_CHUNKS_SENT:
            result = "FLIPDOT_MSG_TYPE_DATA_CHUNKS_SENT";
            switch (msgData[0])
            {
            case FLIPDOT_DATA_PIXELS_COMPLETE:
                result = "FLIPDOT_DATA_PIXELS_COMPLETE";
                break;
            default:
                result = "FLIPDOT_DATA_UNKNOWN";
                break;
            }
            break;
        case FLIPDOT_MSG_TYPE_BUS:
            switch (msgData[0])
            {
            case FLIPDOT_BUS_DISCOVER:
                result = "FLIPDOT_BUS_DISCOVER";
                break;
            case FLIPDOT_BUS_QUERY_STATE:
                result = "FLIPDOT_BUS_QUERY_STATE";
                break;
            case FLIPDOT_BUS_GOODBYE:
                result = "FLIPDOT_BUS_GOODBYE";
                break;
            default:
                result = "FLIPDOT_BUS_UNKNOWN";
            }
            break;
        case FLIPDOT_MSG_TYPE_REQUEST_OPERATION:
            switch (msgData[0])
            {
            case FLIPDOT_OPS_REQUEST_RECEIVE_CONFIG:
                result = "FLIPDOT_OPS_REQUEST_RECEIVE_CONFIG";
                break;
            case FLIPDOT_OPS_REQUEST_RECEIVE_PIXELS:
                result = "FLIPDOT_OPS_REQUEST_RECEIVE_PIXELS";
                break;
            case FLIPDOT_OPS_REQUEST_START_RESET:
                result = "FLIPDOT_OPS_REQUEST_START_RESET";
                break;
            case FLIPDOT_OPS_REQUEST_FINISH_RESET:
                result = "FLIPDOT_OPS_REQUEST_FINISH_RESET";
                break;
            case FLIPDOT_OPS_REQUEST_MORE_DATA:
                result = "FLIPDOT_OPS_REQUEST_MORE_DATA";
                break;
            case FLIPDOT_OPS_REQUEST_SHOW_LOADED_PAGE:
                result = "FLIPDOT_OPS_REQUEST_SHOW_LOADED_PAGE";
                break;
            case FLIPDOT_OPS_REQUEST_LOAD_NEXT_PAGE:
                result = "FLIPDOT_OPS_REQUEST_LOAD_NEXT_PAGE";
                break;
            case FLIPDOT_OPS_REQUEST_NEXT_DATA:
                result = "FLIPDOT_OPS_REQUEST_NEXT_DATA";
                break;
            default:
                result = "FLIPDOT_OPS_REQUEST_UNKNOWN";
            }
            break;
        case FLIPDOT_MSG_TYPE_REPORT_STATE:
            switch (msgData[0])
            {
            case FLIPDOT_STATE_OK:
                result = "FLIPDOT_STATE_OK";
                break;
            case FLIPDOT_STATE_SAVED:
                result = "FLIPDOT_STATE_SAVED";
                break;
            case FLIPDOT_STATE_PIXELS_IN_PROGRESS:
                result = "FLIPDOT_STATE_PIXELS_IN_PROGRESS";
                break;
            case FLIPDOT_STATE_CONFIG_RECEIVED:
                result = "FLIPDOT_STATE_CONFIG_RECEIVED";
                break;
            case FLIPDOT_STATE_READY_TO_RESET:
                result = "FLIPDOT_STATE_READY_TO_RESET";
                break;
            case FLIPDOT_STATE_PIXELS_FAILED:
                result = "FLIPDOT_STATE_PIXELS_FAILED";
                break;
            case FLIPDOT_STATE_CONFIG_FAILED:
                result = "FLIPDOT_STATE_CONFIG_FAILED";
                break;
            case FLIPDOT_STATE_CONFIG_IN_PROGRESS:
                result = "FLIPDOT_STATE_CONFIG_IN_PROGRESS";
                break;
            case FLIPDOT_STATE_UNCONFIGURED:
                result = "FLIPDOT_STATE_UNCONFIGURED";
                break;
            case FLIPDOT_STATE_PAGE_LOADED:
                result = "FLIPDOT_STATE_PAGE_LOADED";
                break;
            case FLIPDOT_STATE_PAGE_SHOW_IN_PROGRESS:
                result = "FLIPDOT_STATE_PAGE_SHOW_IN_PROGRESS";
                break;
            case FLIPDOT_STATE_PAGE_SHOWN:
                result = "FLIPDOT_STATE_PAGE_SHOWN";
                break;
            case FLIPDOT_STATE_PAGE_LOAD_IN_PROGRESS:
                result = "FLIPDOT_STATE_PAGE_LOAD_IN_PROGRESS";
                break;
            default:
                result = "FLIPDOT_STATE_UNKNOWN";
            }
            break;
        case FLIPDOT_MSG_TYPE_ACK_OPERATION:
            switch (msgData[0])
            {
            case FLIPDOT_ACK_RECEIVE_SIGN_CONFIG:
                result = "FLIPDOT_ACK_RECEIVE_SIGN_CONFIG";
                break;
            case FLIPDOT_ACK_RECEIVE_PIXELS:
                result = "FLIPDOT_ACK_RECEIVE_PIXELS";
                break;
            case FLIPDOT_ACK_START_RESET:
                result = "FLIPDOT_ACK_START_RESET";
                break;
            case FLIPDOT_ACK_FINISH_RESET:
                result = "FLIPDOT_ACK_FINISH_RESET";
                break;
            case FLIPDOT_ACK_RECEIVE_CONFIG:
                result = "FLIPDOT_ACK_RECEIVE_CONFIG";
                break;
            // case FLIPDOT_ACK_SHOW_LOADED_PAGE:
            //     result = "FLIPDOT_ACK_SHOW_LOADED_PAGE";
            //     break;
            case FLIPDOT_ACK_LOAD_NEXT_PAGE:
                result = "FLIPDOT_ACK_LOAD_NEXT_PAGE";
                break;
            default:
                result = "FLIPDOT_ACK_UNKNOWN";
            }
            break;
        case FLIPDOT_MSG_TYPE_PIXEL_COMPLETE:
            result = "FLIPDOT_MSG_TYPE_PIXEL_COMPLETE";
            break;
        default:
            result = "FLIPDOT_MSG_TYPE_UNKNOWN";
            break;
        }

        return result;
    }
};