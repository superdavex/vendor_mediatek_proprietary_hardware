/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RtcSuppServController.h"
#include "RfxRilUtils.h"

#define RFX_LOG_TAG "RtcSuppServController"
/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtcSuppServController", RtcSuppServController, RfxController);

RtcSuppServController::RtcSuppServController() {
}

RtcSuppServController::~RtcSuppServController() {
}

void RtcSuppServController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();

    const int request_id_list[] = {
        RFX_MSG_REQUEST_SET_CALL_FORWARD,
        RFX_MSG_REQUEST_QUERY_CALL_FORWARD_STATUS,
        RFX_MSG_REQUEST_GET_CLIR,
        RFX_MSG_REQUEST_SET_CLIR,
        RFX_MSG_REQUEST_QUERY_CALL_WAITING,
        RFX_MSG_REQUEST_SET_CALL_WAITING,
        RFX_MSG_REQUEST_QUERY_CLIP,
        RFX_MSG_REQUEST_SET_CLIP,
        RFX_MSG_REQUEST_GET_COLR,
        RFX_MSG_REQUEST_SET_COLR,
        RFX_MSG_REQUEST_GET_COLP,
        RFX_MSG_REQUEST_SET_COLP,
        RFX_MSG_REQUEST_QUERY_CALL_FORWARD_IN_TIME_SLOT,
        RFX_MSG_REQUEST_SET_CALL_FORWARD_IN_TIME_SLOT,
        RFX_MSG_REQUEST_SET_SUPP_SVC_NOTIFICATION,
    };

    const int urc_id_list[] = {
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int));
    registerToHandleUrc(urc_id_list, sizeof(request_id_list)/sizeof(const int));
}


bool RtcSuppServController::onHandleRequest(const sp<RfxMessage>& message) {
    requestToMcl(message);
    return true;
}

bool RtcSuppServController::onHandleUrc(const sp<RfxMessage>& message) {
    return responseToRilj(message);
}

bool RtcSuppServController::onHandleResponse(const sp<RfxMessage>& message) {
    return responseToRilj(message);
}

bool RtcSuppServController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
        bool isModemPowerOff, int radioState) {
    RFX_UNUSED(message);
    logD(RFX_LOG_TAG, "onCheckIfRejectMessage isModemPowerOff %d, radioState: %d",
            (isModemPowerOff == false) ? 0 : 1, radioState);

    /* If WFC is enabled, bypass all SS requests. */
    if (RfxRilUtils::isWfcEnable(message->getSlotId())) {
        return false;
    }

    /* If WFC is not enabled and radio is off. reject the request in request_id_list*/
    if (radioState == (int)RADIO_STATE_UNAVAILABLE ||
            radioState == (int)RADIO_STATE_OFF ||
            isModemPowerOff == true) {
        return true;
    }

    /* If WFC is not enabled and radio is not off. Bypass all SS request */
    return false;
}