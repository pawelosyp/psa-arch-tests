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

#ifndef __PROTECTED_STORAGE_H__
#define __PROTECTED_STORAGE_H__

/** @file
@brief This file describes the PSA Protected Storage API
*/

#include <stddef.h>
#include <stdint.h>

#define PSA_PS_API_VERSION_MAJOR  0  /**< The major version number of the PSA PS API. It will be incremented on significant updates that may include breaking changes */
#define PSA_PS_API_VERSION_MINOR  7  /**< The minor version number of the PSA PS API. It will be incremented in small updates that are unlikely to include breaking changes */

/** \brief Flags used when creating a data entry
 */
typedef uint32_t psa_ps_create_flags_t;

/** \brief A type for UIDs used for identifying data
 */
typedef uint64_t psa_ps_uid_t;

#define PSA_PS_FLAG_NONE        0
#define PSA_PS_FLAG_WRITE_ONCE (1 << 0) /**< The data associated with the uid will not be able to be modified or deleted. Intended to be used to set bits in `psa_eps_create_flags_t`*/

/**
 * \brief A container for metadata associated with a specific uid
 */
struct psa_ps_info_t {
    uint32_t size; /**< The size of the data associated with a uid **/
    psa_ps_create_flags_t flags; /**< The flags set when the uid was created **/
};
/**
 * \brief The return status type for the PSA Trusted Storage functions
 */
typedef uint32_t psa_ps_status_t;

#define PSA_PS_SUCCESS                         0   /**<    The operation completed successfully */
#define PSA_PS_ERROR_WRITE_ONCE                1   /**<    The operation failed because the provided key value was already created with PSA_PS_WRITE_ONCE_FLAG */
#define PSA_PS_ERROR_FLAGS_NOT_SUPPORTED       2   /**<    The operation failed because one or more of the flags provided in `create_flags` is not supported or is not valid */
#define PSA_PS_ERROR_INSUFFICIENT_SPACE        4   /**<    The operation failed because there was insufficient space on the storage medium */
#define PSA_PS_ERROR_STORAGE_FAILURE           6   /**<    The operation failed because the physical storage has failed (Fatal error) */
#define PSA_PS_ERROR_BAD_POINTER               7   /**<    The operation failed because one of the provided pointers is invalid, for example is `NULL` or references memory the caller cannot access */
#define PSA_PS_ERROR_KEY_NOT_FOUND             8   /**<    The operation failed because the provided key value was not found in the storage */
#define PSA_PS_ERROR_INCORRECT_SIZE            9   /**<    The operation failed because the data associated with provided key is not the same size as `data_size`, or `offset+data_size` is too large for the data, but `offset` is less than the size */
#define PSA_PS_ERROR_OFFSET_INVALID           10   /**<    The operation failed because an offset was supplied that is invalid for the existing data associated with the uid. For example, offset  is greater that the size of the data */


/**
 * \brief create a new or modify an existing key/value pair
 *
 * \param[in] uid           the identifier for the data
 * \param[in] data_length   The size in bytes of the data in `p_data`
 * \param[in] p_data        A buffer containing the data 
 * \param[in] create_flags  The flags indicating the properties of the data
 * 
 * \return      A status indicating the success/failure of the operation
 
 * \retval      PSA_PS_SUCCESS                              The operation completed successfully
 * \retval      PSA_PS_ERROR_WRITE_ONCE                     The operation failed because the provided uid value was already created with PSA_PS_WRITE_ONCE_FLAG
 * \retval      PSA_PS_ERROR_FLAGS_NOT_SUPPORTED            The operation failed because one or more of the flags provided in `create_flags` is not supported or is not valid
 * \retval      PSA_PS_ERROR_FLAGS_SET_AFTER_CREATE         The operation failed because a non-zero `create_flags` was provided for a previously created uid
 * \retval      PSA_PS_ERROR_INSUFFICIENT_SPACE             The operation failed because there was insufficient space on the storage medium
 * \retval      PSA_PS_ERROR_STORAGE_FAILURE                The operation failed because the physical storage has failed (Fatal error)
 * \retval      PSA_PS_ERROR_BAD_POINTER                    The operation failed because one of the provided pointers(`p_data`) 
 *                                                          is invalid, for example is `NULL` or references memory the caller cannot access
 */
psa_ps_status_t psa_ps_set( psa_ps_uid_t uid, uint32_t data_length,
                            const void *p_data, psa_ps_create_flags_t create_flags );

/**
 * \brief Retrieve the value for a provided uid
 *
 * \param[in] uid               The identifier for the data
 * \param[in] data_offset       The offset within the data associated with the `uid` to start retrieving data
 * \param[in] data_length       The amount of data to read (and the minimum allocated size of the `p_data` buffer)
 * \param[out] p_data           The buffer where the data will be placed upon successful completion
 * 
 * \return      A status indicating the success/failure of the operation
 *
 * \retval      PSA_PS_SUCCESS                      The operation completed successfully
 * \retval      PSA_PS_ERROR_KEY_NOT_FOUND          The operation failed because the provided uid value was not found in the storage
 * \retval      PSA_PS_ERROR_INCORRECT_SIZE        The operation failed because the data associated with provided uid is not the same size as `data_size`
 * \retval      PSA_PS_ERROR_STORAGE_FAILURE        The operation failed because the physical storage has failed (Fatal error)
 * \retval      PSA_PS_ERROR_BAD_POINTER            The operation failed because one of the provided pointers(`p_data`)
 *                                                  is invalid, for example is `NULL` or references memory the caller cannot access
 * \retval      PSA_PS_ERROR_OFFSET_INVALID         The operation failed because an offset was supplied that is invalid for the existing data associated with the 
 *                                                  uid. For example, offset + size is invalid
 * \retval      PSA_PS_ERROR_OFFSET_NOT_SUPPORTED   A non-zero offset was supplied, but the implementation does not support offsets
 */
psa_ps_status_t psa_ps_get( psa_ps_uid_t uid, uint32_t data_offset,
                            uint32_t data_length, void *p_data );

/**
 * \brief Retrieve the metadata about the provided uid
 * 
 * \param[in] uid           The identifier for the data
 * \param[out] p_info       A pointer to the `psa_ps_info_t` struct that will be populated with the metadata
 * 
 * \return      A status indicating the success/failure of the operation
 * 
 * \retval      PSA_PS_SUCCESS                  The operation completed successfully
 * \retval      PSA_PS_ERROR_KEY_NOT_FOUND      The operation failed because the provided uid value was not found in the storage
 * \retval      PSA_PS_ERROR_STORAGE_FAILURE    The operation failed because the physical storage has failed (Fatal error)
 * \retval      PSA_PS_ERROR_BAD_POINTER        The operation failed because one of the provided pointers(`p_info`)
 *                                              is invalid, for example is `NULL` or references memory the caller cannot access
 */
psa_ps_status_t psa_ps_get_info( psa_ps_uid_t uid, struct psa_ps_info_t *p_info);

/**
 * \brief Remove the provided uid and its associated data from the storage
 * 
 * \param[in] uid   The identifier for the data to be removed
 * 
 * \return  A status indicating the success/failure of the operation
 * 
 * \retval      PSA_PS_SUCCESS                  The operation completed successfully
 * \retval      PSA_PS_ERROR_KEY_NOT_FOUND      The operation failed because the provided uid value was not found in the storage
 * \retval      PSA_PS_ERROR_WRITE_ONCE         The operation failed because the provided uid value was created with psa_eps_WRITE_ONCE_FLAG
 * \retval      PSA_PS_ERROR_STORAGE_FAILURE    The operation failed because the physical storage has failed (Fatal error)
 */
psa_ps_status_t psa_ps_remove( psa_ps_uid_t uid);

/**
 *  Creates an asset based on the given identifier, the maximum size and
 *  creation flags. This create allocates the space in the secure storage
 *  area without setting any data in the asset.
 *  It is only necessary to call this function for items that will be written
 *  with the \ref psa_ps_set_extended function. If only the \ref psa_ps_set function
 *  is needed, calls to this function are redundant.
 * 
 *  This function is optional. Not all PSA Protected Storage Implementations
 *  will implement this function. Consult the documentation of your chosen
 *  platform to determine if it is present.
 *
 * \param[in] uid           A unique identifier for the asset.
 * \param[in] size          The maximum size in bytes of the asset.
 * \param[in] create_flags  Create flags \ref psa_ps_create_flags_t.
 *
 * \retval PSA_PS_SUCCESS                   The assest does not exist and the input parameters are correct or
 *                                          the asset already exists, the input parameters are the same that
 *                                          have been used to create the asset and the owner is the same and the current asset content is kept
 *                                          TDB: "Owner is the same" doesn't really make sense from a PSA perspective, as each partition 
 *                                          has its own UID space, making other partitions' data unadressable
 * \retval PSA_PS_ERROR_STORAGE_FAILURE     The create action has a physical storage error
 * \retval PSA_PS_ERROR_INSUFFICIENT_SPACE  The maximum size is bigger of the current available space
 * \retval PSA_PS_ERROR_FLAGS_NOT_SUPPORTED One or more create_flags are not valid or supported
 * \retval PSA_PS_ERROR_INVALID_KEY         The the asset exists and the input paramters are not the same as the existing asset
 */
psa_its_status_t psa_ps_create( psa_ps_uid_t uid, uint32_t size,
                                psa_ps_create_flags_t create_flags);

/**
 * Sets partial data into an asset based on the given identifier, data_offset,
 * data length and p_data.
 * 
 *  Before calling this function, the asset must have been created with a cal
 *  to \ref psa_ps_create
 *
 *  This function is optional. Not all PSA Protected Storage Implementations
 *  will implement this function. Consult the documentation of your chosen
 *  platform to determine if it is present.
 * 
 * \param[in] uid          The unique identifier for the asset.
 * \param[in] data_offset  Offset within the asset to start the write.
 * \param[in] data_length  The size in bytes of the data in p_data to write.
 * \param[in] p_data       Pointer to a buffer which contains the data to write.
 *
 * \retval PSA_SUCCESS                      If the asset exists, the input parameters are correct and the data
 *                                          is correctly written in the physical storage
 * \retval PSA_ITS_ERROR_STORAGE_FAILURE    If the data is not written correctly in the physical storage
 * \retval PSA_PS_ERROR_OFFSET_INVALID      The operation failed because an offset was supplied that is invalid
 *                                          for the allocated size of the space
 *                                          reserved for the `uid` when \ref psa_psa_create
 *                                          was called. For example, offset + size
 *                                          is too large
 * \retval PSA_PS_ERROR_BAD_POINTER         If p_data is NULL or references memory the caller cannot access
 * \retval PSA_PS_ERROR_INVALID_KEY         If the asset does not exist
 */
psa_its_status_t psa_ps_set_extended( psa_ps_uid_t uid, uint32_t data_offset,
                                      uint32_t data_length, const void *p_data);

#endif // __PROTECTED_STORAGE_H__
