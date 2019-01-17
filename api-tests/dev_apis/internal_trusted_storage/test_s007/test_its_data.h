/** @file
 * Copyright (c) 2019, Arm Limited or its affiliates. All rights reserved.
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
#ifndef _TEST_S007_ITS_DATA_TESTS_H_
#define _TEST_S007_ITS_DATA_TESTS_H_

#include "val_internal_trusted_storage.h"

#define SST_FUNCTION val->its_function
#define psa_sst_uid_t psa_its_uid_t

typedef struct {
    char                    test_desc[100];
    enum its_function_code  api;
    psa_its_status_t        status;
} test_data;

static psa_its_create_flags_t flag;
static test_data s007_data[] = {
{
 "This is dummy for index0", 0, 0
},
{
 "Create a valid storage entity with non-zero flag value", VAL_ITS_SET, PSA_ITS_SUCCESS
},
{
 "try to change the flag value with another non-zero value", VAL_ITS_SET, PSA_ITS_ERROR_FLAGS_SET_AFTER_CREATE
},
{
 "try to change the flag value with zero value", VAL_ITS_SET, PSA_ITS_ERROR_FLAGS_SET_AFTER_CREATE
},
{
 "Remove the storage entity ", VAL_ITS_REMOVE, PSA_ITS_SUCCESS
},
{
 "Create a valid storage entity with zero flag value", VAL_ITS_SET, PSA_ITS_SUCCESS
},
{
 "try to change the flag value with  non-zero value", VAL_ITS_SET, PSA_ITS_SUCCESS
},
{
 "try to change the flag again to zero value", VAL_ITS_SET, PSA_ITS_ERROR_FLAGS_SET_AFTER_CREATE
},
{
 "Remove the storage entity ", VAL_ITS_REMOVE, PSA_ITS_SUCCESS
},
};
#endif /* _TEST_S007_ITS_DATA_TESTS_H_ */
