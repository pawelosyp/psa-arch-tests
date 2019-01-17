/** @file
 * Copyright (c) 2019, Arm Limited or ps affiliates. All rights reserved.
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
#ifndef _TEST_S009_PS_DATA_TESTS_H_
#define _TEST_S009_PS_DATA_TESTS_H_

#include "val_protected_storage.h"

#define SST_FUNCTION val->ps_function
#define psa_sst_uid_t psa_ps_uid_t

typedef struct {
    char                   test_desc[100];
    enum ps_function_code  api;
    psa_ps_status_t        status;
} test_data;

static struct psa_ps_info_t info;
static test_data s009_data[] = {
{
 "This is dummy for index0", 0, 0
},
{
 "Call set api with 0 write buffer", VAL_PS_SET, PSA_PS_ERROR_BAD_POINTER
},
{
 "Call to get_info api should fail", VAL_PS_GET_INFO, PSA_PS_ERROR_KEY_NOT_FOUND
},
{
 "Create storage of zero size", VAL_PS_SET, PSA_PS_SUCCESS
},
{
 "try to set 0 buffer for previous created storage", VAL_PS_SET, PSA_PS_ERROR_BAD_POINTER
},
{
 "Call get_info api to check data size", VAL_PS_GET_INFO, PSA_PS_SUCCESS
},
{
 "This is dummy for index6", 0, 0
},
{
 "Call get api with 0 read buffer", VAL_PS_GET, PSA_PS_ERROR_BAD_POINTER
},
{
 "Call get_info api with 0 info buffer", VAL_PS_GET_INFO, PSA_PS_ERROR_BAD_POINTER
},
{
 "Remove the storage entity ", VAL_PS_REMOVE, PSA_PS_SUCCESS
},
};
#endif /* _TEST_S009_PS_DATA_TESTS_H_ */
