#pragma once

#include <stdint.h>
#include "emulator_export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates TransactionEmulator object
 * @param config_params_boc Base64 encoded BoC serialized Config dictionary (Hashmap 32 ^Cell)
 * @param shardchain_libs_boc Base64 encoded BoC serialized shardchain libraries dictionary (HashmapE 256 ^Cell). Can be NULL if no shardchain libraries needed.
 * @param vm_log_verbosity Verbosity level of VM log. 0 - log truncated to last 256 characters. 1 - unlimited length log.
 * 2 - for each command prints its cell hash and offset. 3 - for each command log prints all stack values.
 * @return Pointer to TransactionEmulator or nullptr in case of error
 */
EMULATOR_EXPORT void *transaction_emulator_create(const char *config_params_boc, int vm_log_verbosity);

EMULATOR_EXPORT bool transaction_emulator_set_unixtime(void *transaction_emulator, uint32_t unixtime);

EMULATOR_EXPORT bool transaction_emulator_set_lt(void *transaction_emulator, uint64_t lt);

EMULATOR_EXPORT bool transaction_emulator_set_rand_seed(void *transaction_emulator, const char* rand_seed_hex);

EMULATOR_EXPORT bool transaction_emulator_set_ignore_chksig(void *transaction_emulator, bool ignore_chksig);

EMULATOR_EXPORT bool transaction_emulator_set_config(void *transaction_emulator, const char* config_boc);

EMULATOR_EXPORT bool transaction_emulator_set_libs(void *transaction_emulator, const char* libs_boc);

/**
 * @brief Emulate transaction
 * @param transaction_emulator Pointer to TransactionEmulator object
 * @param shard_account_boc Base64 encoded BoC serialized ShardAccount
 * @param message_boc Base64 encoded BoC serialized inbound Message (internal or external)
 * @return Json object with error:
 * { "success": false, "error": "Error description" } and optional fields "vm_exit_code" and "vm_log" in case external message was not accepted.
 * Or success:
 * { "success": true, "transaction": "Base64 encoded Transaction boc", "shard_account": "Base64 encoded ShardAccount boc", "vm_log": "execute DUP..." }
 */
EMULATOR_EXPORT const char *transaction_emulator_emulate_transaction(void *transaction_emulator, const char *shard_account_boc, const char *message_boc, const char *other_params);

/**
 * @brief Destroy TransactionEmulator object
 * @param transaction_emulator Pointer to TransactionEmulator object
 */
EMULATOR_EXPORT void transaction_emulator_destroy(void *transaction_emulator);

/**
 * @brief Set global verbosity level of the library
 * @param verbosity_level New verbosity level (0 - never, 1 - error, 2 - warning, 3 - info, 4 - debug)
 */
EMULATOR_EXPORT bool emulator_set_verbosity_level(int verbosity_level);

/**
 * @brief Create TVM emulator
 * @param code_boc Base64 encoded BoC serialized smart contract code cell
 * @param data_boc Base64 encoded BoC serialized smart contract data cell
 * @param vm_log_verbosity Verbosity level of VM log
 * @return Pointer to TVM emulator object
 */
EMULATOR_EXPORT void *tvm_emulator_create(const char *code_boc, const char *data_boc, int vm_log_verbosity);

/**
 * @brief Set libraries for TVM emulator
 * @param libs_boc Base64 encoded BoC serialized libraries dictionary (HashmapE 256 ^Cell).
 */
EMULATOR_EXPORT bool tvm_emulator_set_libraries(void *tvm_emulator, const char *libs_boc);

/**
 * @brief Set c7 parameters
 * @param tvm_emulator Pointer to TVM emulator
 * @param address Adress of smart contract
 * @param unixtime Unix timestamp
 * @param balance Smart contract balance
 * @param rand_seed_hex Random seed as hex string of length 64
 * @param config Base64 encoded BoC serialized Config dictionary (Hashmap 32 ^Cell)
 * @return EMULATOR_EXPORT 
 */
EMULATOR_EXPORT bool tvm_emulator_set_c7(void *tvm_emulator, const char *address, uint32_t unixtime, uint64_t balance, const char *rand_seed_hex, const char *config);

/**
 * @brief Set c7 parameters
 * @param tvm_emulator Pointer to TVM emulator
 * @param gas_limit Gas limit
 */
EMULATOR_EXPORT bool tvm_emulator_set_gas_limit(void *tvm_emulator, int64_t gas_limit);

/**
 * @brief Run get method
 * @param tvm_emulator Pointer to TVM emulator
 * @param method_id Integer method id
 * @param stack_json Json array with stack entries the following format:
 * [
 *     {
 *          "type": "cell, cell_slice, number or tuple",
 *          "value": value (string or in case of tuple array of stack entries)
 *     },
 *     { ... }
 * ]
 * @return Json object with result:
 * {
 *     "vm_log": "...", 
 *     "vm_exit_code": 0, 
 *     "stack": [], 
 *     "missing_library": null, 
 *     "gas_used": 1212
 * }
 */
EMULATOR_EXPORT const char *tvm_emulator_run_get_method(void *tvm_emulator, int method_id, const char *stack_json);

EMULATOR_EXPORT const char *tvm_emulator_send_external_message(void *tvm_emulator, const char *message_body_boc);

EMULATOR_EXPORT const char *tvm_emulator_send_internal_message(void *tvm_emulator, const char *message_body_boc, uint64_t amount);

/**
 * @brief Destroy TVM emulator object
 * @param tvm_emulator Pointer to TVM emulator object
 */
EMULATOR_EXPORT void tvm_emulator_destroy(void *tvm_emulator);

#ifdef __cplusplus
}  // extern "C"
#endif
