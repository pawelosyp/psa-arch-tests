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

#include "val_crypto.h"

#define EMPTY_KEY_SLOT      31

typedef struct {
    char                    test_desc[75];
    psa_key_handle_t        key_handle;
    psa_key_type_t          key_type;
    uint8_t                 key_data[64];
    uint32_t                key_length;
    psa_key_usage_t         usage;
    psa_algorithm_t         key_alg;
    psa_status_t            expected_status;
} test_data;

static test_data check1[] = {
{"Test psa_mac_verify_setup 64 Byte HMAC\n", 1, PSA_KEY_TYPE_HMAC,
{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,
 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33,
 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f},
 64, PSA_KEY_USAGE_VERIFY, PSA_ALG_HMAC(PSA_ALG_SHA_256),
 PSA_SUCCESS
},

{"Test psa_mac_verify_setup 16 Byte AES - CMAC\n", 2, PSA_KEY_TYPE_AES,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9, 0x00},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_VERIFY, PSA_ALG_CMAC,
 PSA_SUCCESS
},

{"Test psa_mac_verify_setup 16 Byte AES - GMAC\n", 3, PSA_KEY_TYPE_AES,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9, 0x00},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_VERIFY, PSA_ALG_GMAC,
 PSA_ERROR_NOT_SUPPORTED
},

{"Test psa_mac_verify_setup incompactible HMAC for CMAC\n", 4, PSA_KEY_TYPE_HMAC,
{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C,
 0x0D, 0x0E, 0x0F},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_VERIFY, PSA_ALG_CMAC,
 PSA_ERROR_NOT_SUPPORTED
},

{"Test psa_mac_verify_setup invalid usage\n", 5, PSA_KEY_TYPE_AES,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_EXPORT, PSA_ALG_CMAC,
 PSA_ERROR_NOT_PERMITTED
},

{"Test psa_mac_verify_setup invalid key type\n", 6, PSA_KEY_TYPE_RAW_DATA,
{0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
 0x0B, 0x0B, 0x0B},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_VERIFY, PSA_ALG_HMAC(PSA_ALG_SHA_256),
 PSA_ERROR_INVALID_ARGUMENT
},

{"Test psa_mac_verify_setup truncated MAC too large\n", 7, PSA_KEY_TYPE_HMAC,
{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,
 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33,
 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f},
 64, PSA_KEY_USAGE_VERIFY, PSA_ALG_TRUNCATED_MAC(PSA_ALG_HMAC(PSA_ALG_SHA_256), 33),
 PSA_ERROR_INVALID_ARGUMENT
},

{"Test psa_mac_verify_setup truncated MAC too small\n", 8, PSA_KEY_TYPE_HMAC,
{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,
 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33,
 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f},
 64, PSA_KEY_USAGE_VERIFY, PSA_ALG_TRUNCATED_MAC(PSA_ALG_HMAC(PSA_ALG_SHA_256), 1),
 PSA_ERROR_NOT_SUPPORTED
},

{"Test psa_mac_verify_setup bad algorithm (unknown MAC algorithm)\n", 9, PSA_KEY_TYPE_AES,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9, 0x00},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_VERIFY, PSA_ALG_HMAC(0),
 PSA_ERROR_NOT_SUPPORTED
},

{"Test psa_mac_verify_setup bad algorithm (not a MAC algorithm)\n", 10, PSA_KEY_TYPE_AES,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9, 0x00},
 AES_16B_KEY_SIZE, PSA_KEY_USAGE_VERIFY, PSA_ALG_CBC_NO_PADDING,
 PSA_ERROR_INVALID_ARGUMENT
},
};

static test_data check2[] = {
{"Test psa_mac_verify_setup 64 negative case\n", 11, PSA_KEY_TYPE_HMAC,
{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,
 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33,
 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f},
 64, PSA_KEY_USAGE_VERIFY, PSA_ALG_HMAC(PSA_ALG_SHA_256),
 PSA_SUCCESS
},
};
