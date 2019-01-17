/** @file
 * Copyright (c) 2018, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifdef NONSECURE_TEST_BUILD
#include "val_interfaces.h"
#include "val_target.h"
#else
#include "val_client_defs.h"
#include "val_partition_common.h"
#endif

#include "test_i035.h"

client_test_t test_i035_client_tests_list[] = {
    NULL,
    client_test_psa_skip_at_ipc_disconnect,
    NULL,
};

int32_t client_test_psa_skip_at_ipc_disconnect(security_t caller)
{
   psa_handle_t       handle = 0;

   val->print(PRINT_TEST, "[Check1] Test psa_skip at PSA_IPC_DISCONNECT\n", 0);

   handle = psa->connect(SERVER_RELAX_MINOR_VERSION_SID, 1);
   if (handle > 0)
   {
       psa->close(handle);
   }

   /* Shouldn't have reached here */
   val->print(PRINT_ERROR, "\tCheck for psa_skip at PSA_IPC_DISCONNECT failed\n", 0);

   return VAL_STATUS_SPM_FAILED;
}
