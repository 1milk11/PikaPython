/*
 * This file is part of the PikaScript project.
 * http://github.com/pikastech/pikapython
 *
 * MIT License
 *
 * Copyright (c) 2021 lyon liang6516@outlook.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PIKA_VM__H
#define __PIKA_VM__H
#include "PikaObj.h"        // TODO: cause PikaParser include PikaObj
#include "dataQueue.h"      // TODO: data* files should be public lib
#include "dataQueueObj.h"
#include "dataStack.h"
#if PIKA_SETJMP_ENABLE
#include <setjmp.h>
#endif

typedef struct {
    int8_t n_positional;
    int8_t n_positional_got;
    int8_t n_default;
    int8_t n_arg;
    int8_t i_arg;
    int8_t n_input;
    pika_bool is_vars;
    pika_bool is_keys;
    pika_bool is_default;
    ArgType method_type;
    PikaTuple* tuple;
    PikaDict* kw;
    PikaDict* kw_keys;
    char* var_tuple_name;
    char* kw_dict_name;
    char* type_list;
} FunctionArgsInfo;

typedef struct OperatorInfo OperatorInfo;
struct OperatorInfo {
    char* opt;
    ArgType t1;
    ArgType t2;
    Arg* a1;
    Arg* a2;
    pika_float f1;
    pika_float f2;
    int64_t i1;
    int64_t i2;
    Arg* res;
    uint32_t num;
    PikaVMFrame* vm;
};

typedef enum VM_SIGNAL_CTRL {
    VM_SIGNAL_CTRL_NONE = 0,
    VM_SIGNAL_CTRL_EXIT,
} VM_SIGNAL_CTRL;

typedef union _EventDataType {
    int signal;
    Arg* arg;
} EventDataType;

typedef struct PikaArgEventQueue {
    uint32_t id[PIKA_EVENT_LIST_SIZE];
    EventDataType data[PIKA_EVENT_LIST_SIZE];
    PikaEventListener* listener[PIKA_EVENT_LIST_SIZE];
    Arg* res[PIKA_EVENT_LIST_SIZE];
    int head;
    int tail;
} PikaEventQueue;

#if PIKA_SETJMP_ENABLE
typedef struct JmpBufCQ {
    jmp_buf* buf[PIKA_JMP_BUF_LIST_SIZE];
    int head;
    int tail;
} JmpBufCQ;
#endif

typedef struct VMState VMState;
struct VMState {
    VM_SIGNAL_CTRL signal_ctrl;
    int vm_cnt;
#if PIKA_EVENT_ENABLE
    PikaEventQueue cq;      // TODO: seems like nvme protocol
    PikaEventQueue sq;      // cq: completion queue, sq: submission queue
    int event_pickup_cnt;
    pika_platform_thread_t* event_thread;
    pika_bool event_thread_exit;
    pika_bool event_thread_exit_done;
#endif
#if PIKA_DEBUG_BREAK_POINT_MAX > 0
    Hash break_module_hash[PIKA_DEBUG_BREAK_POINT_MAX];
    uint32_t break_point_pc[PIKA_DEBUG_BREAK_POINT_MAX];
    int break_point_cnt;
#endif
};

typedef Arg* (*VM_instruct_handler)(PikaObj* self,
                                    PikaVMFrame* vm,
                                    char* data,
                                    Arg* arg_ret_reg);

typedef struct VMInstruction VMInstruction;
struct VMInstruction {
    VM_instruct_handler handler;
    const char* op_str;
    uint16_t op_idx;
    uint16_t op_str_len : 4;
    uint16_t : 12;
};

typedef struct VMInstructionSet VMInstructionSet;
struct VMInstructionSet {
    const VMInstruction* instructions;
    uint16_t count;
    uint16_t signature;
    uint16_t op_idx_start;
    uint16_t op_idx_end;
};

VMParameters* pikaVM_run(PikaObj* self, char* pyLine);
VMParameters* pikaVM_runAsm(PikaObj* self, char* pikaAsm);
// TODO: why PikaObj.c call this interface
VMParameters* pikaVM_runByteCodeFrame(PikaObj* self, ByteCodeFrame* byteCode_frame);

// TODO: called by PikaCompier
static inline enum InstructIndex instructUnit_getInstructIndex(
    InstructUnit* self) {
    return (enum InstructIndex)(self->isNewLine_instruct & 0x7F);
}

// TODO: called by PikaParser
static inline void instructUnit_setBlockDeepth(InstructUnit* self, int val) {
    self->deepth |= (0x0F & val);
}
static inline void instructUnit_setConstPoolIndex(InstructUnit* self, int val) {
    self->const_pool_index = val;
}
static inline void instructUnit_setInvokeDeepth(InstructUnit* self, int val) {
    self->deepth |= ((0x0F & val) << 4);
}
static inline void instructUnit_setInstruct(InstructUnit* self, int val) {
    self->isNewLine_instruct |= (0x7F & val);
}
static inline void instructUnit_setIsNewLine(InstructUnit* self, int val) {
    self->isNewLine_instruct |= ((0x01 & val) << 7);
}
enum InstructIndex pikaVM_getInstructFromAsm(char* line);
void constPool_append(ConstPool* self, char* content);
static inline int constPool_getLastOffset(ConstPool* self) {
    return self->size;
}

static inline void* constPool_getStart(ConstPool* self) {
    return self->content_start;
}
// TODO: called by PikaCompier
static inline char* constPool_getByOffset(ConstPool* self, int offset) {
    return (char*)((uintptr_t)constPool_getStart(self) + (uintptr_t)offset);
}

typedef struct {
    PikaObj* globals;
    pika_bool in_repl;  // repl means Read-Evaluate-Print-Loop, it's interactive cmd console
    char* module_name;
} pikaVM_run_ex_cfg;

void byteCodeFrame_init(ByteCodeFrame* bf);
void byteCodeFrame_deinit(ByteCodeFrame* bf);
// TODO: called by PikaParser
void instructArray_append(InstructArray* ins_array, InstructUnit* ins_unit);
void byteCodeFrame_print(ByteCodeFrame* self);
// TODO: called by PikaCompier
static inline size_t instructUnit_getSize(void) {
    return (size_t)sizeof(InstructUnit);
}
// TODO: called by PikaParser
uint16_t constPool_getOffsetByData(ConstPool* self, char* data);
// TODO: called by PikaCompier
void byteCodeFrame_loadByteCode(ByteCodeFrame* self, uint8_t* bytes);
// TODO: called by PikaParser
void byteCodeFrame_printAsArray(ByteCodeFrame* self);

VMParameters* pikaVM_runByteCode(PikaObj* self, const uint8_t* bytecode);
// TODO: called by PikaObj, and why
VMParameters* pikaVM_runByteCodeInconstant(PikaObj* self, uint8_t* bytecode);
Arg* pikaVM_runByteCodeReturn(PikaObj* self,
                              const uint8_t* bytecode,
                              char* returnName);
// TODO: called by PikaCompier
InstructUnit* instructArray_getNow(InstructArray* self);
InstructUnit* instructArray_getNext(InstructArray* self);

VMParameters* pikaVM_runSingleFile(PikaObj* self, char* filename);
VMParameters* pikaVM_runByteCodeFile(PikaObj* self, char* filename);
// TODO: called by PikaObj, and why
Arg* obj_runMethodArg(PikaObj* self, PikaObj* method_args_obj, Arg* method_arg);
PikaObj* pikaVM_runFile(PikaObj* self, char* file_name);

typedef struct {
    VMParameters* locals;
    VMParameters* globals;
    char* name;
    PikaVMThread* vm_thread;
    pika_bool is_const_bytecode;
} pikaVM_runBytecode_ex_cfg;

// TODO: called by PikaObj, and why
VMParameters* pikaVM_runByteCode_ex(PikaObj* self,
                                    uint8_t* bytecode,
                                    pikaVM_runBytecode_ex_cfg* cfg);
Arg* _vm_get(PikaVMFrame* vm, PikaObj* self, Arg* key, Arg* obj);
void pika_vm_exit(void);
void pika_vm_exit_await(void);
PIKA_RES __eventListener_pushEvent(PikaEventListener* lisener,
                                   uintptr_t eventId,
                                   Arg* eventData);
PIKA_RES __eventListener_pushSignal(PikaEventListener* lisener,
                                    uintptr_t eventId,
                                    int signal);
int _VMEvent_getVMCnt(void);
void __VMEvent_pickupEvent(char* info);
int _VM_lock_init(void);
int _VM_is_first_lock(void);
PIKA_RES pika_debug_set_break(char* module_name, int pc_break);
void pika_debug_set_trace(PikaObj* self);
PIKA_RES pika_debug_reset_break(char* module_name, int pc_break);

#define _VMEvent_pickupEvent() __VMEvent_pickupEvent(__FILE__)
#define PIKA_INS(__INS_NAME) _##PIKA_VM_INS_##__INS_NAME

typedef struct {
    PikaObj* lreg[PIKA_REGIST_SIZE];
} VMLocals;

VMParameters* pikaVM_run_ex(PikaObj* self,
                            char* py_lines,
                            pikaVM_run_ex_cfg* cfg);

#endif

#ifdef __cplusplus
}
#endif
