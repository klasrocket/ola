/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * RDMController.h
 * A RDM Controller that sends a single message at a time.
 * Copyright (C) 2010 Simon Newton
 */

#ifndef INCLUDE_OLA_RDM_RDMCONTROLLERINTERFACE_H_
#define INCLUDE_OLA_RDM_RDMCONTROLLERINTERFACE_H_

#include <ola/Callback.h>
#include <ola/rdm/RDMCommand.h>

namespace ola {
namespace rdm {

/**
 * The various states a request could return
 */
typedef enum {
  // The request/response completed correctly
  RDM_COMPLETED_OK,
  // The request was broadcast, no respone expected
  RDM_WAS_BROADCAST,
  // We failed to send this request
  RDM_FAILED_TO_SEND,
  // The response timed out
  RDM_TIMEOUT,
  // The response was invalid
  RDM_INVALID_RESPONSE,
  // The UID could not be located (may have been removed)
  RDM_UNKNOWN_UID,
} rdm_request_status;


/**
 * This is the type of callback that is run when a request completes or fails.
 * For performance reasons we can take either a single use callback or a
 * permanent callback.
 */
typedef ola::BaseCallback2<void,
                           rdm_request_status,
                           const RDMResponse*> RDMCallback;

/**
 * This is a class that can send RDM messages.
 */
class RDMControllerInterface {
  public:
    RDMControllerInterface() {}
    virtual ~RDMControllerInterface() {}

    /**
     * Assumption: A class that implements this MUST ensure that as time tends
     * to infinity, the probably that the callback is run tends to 1. That is,
     * there must be no way that a request can be dropped in such a way that
     * the callback is never run. Doing so will either block all subsequent
     * requests, or leak memory depending on the implementation.
     *
     * Also the implementor of this class should re-write the transaction #,
     * and possibly the UID (changing src UIDs isn't addresses by the RDM
     * spec).
     */
    virtual void SendRequest(const RDMRequest *request,
                             RDMCallback *on_complete) = 0;
};
}  // rdm
}  // ola
#endif  // INCLUDE_OLA_RDM_RDMCONTROLLERINTERFACE_H_
