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

#include "test_i031.h"

client_test_t test_i031_client_tests_list[] = {
    NULL,
    client_test_psa_read_with_invalid_handle,
    NULL,
};

int32_t client_test_psa_read_with_invalid_handle(security_t caller)
{
   int32_t            status = VAL_STATUS_SUCCESS;
   psa_handle_t       handle = 0;
   psa_status_t       status_of_call;

   val->print(PRINT_TEST,
            "[Check1] Test psa_read with invalid handle\n", 0);

   handle = psa->connect(SERVER_UNSPECIFED_MINOR_V_SID, 1);
   if (handle < 0)
   {
       val->print(PRINT_ERROR, "\tConnection failed\n", 0);
       return VAL_STATUS_INVALID_HANDLE;
   }

   status_of_call =  psa->call(handle, NULL, 0, NULL, 0);

   /* Expectation is server test should hang and control shouldn't have come here */
   val->print(PRINT_ERROR, "\tCall should failed but successed\n", 0);

   status = VAL_STATUS_SPM_FAILED;

   psa->close(handle);
   (void)(status_of_call);
   return status;
}
