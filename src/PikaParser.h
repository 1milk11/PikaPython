/*
 * This file is part of the PikaPython project.
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

#ifndef __PIKA_PARSER__H
#define __PIKA_PARSER__H
#include "PikaVM.h"         // TODO: don't include this
#include "dataQueueObj.h"   // TODO: don't include this
#include "dataStack.h"      // TODO: don't include this

typedef QueueObj AST;

typedef enum TokenType {
    TOKEN_strEnd = 0,
    TOKEN_symbol,
    TOKEN_keyword,
    TOKEN_operator,
    TOKEN_devider,
    TOKEN_literal,
} TokenType;

enum StmtType {
    STMT_reference,
    STMT_tuple,
    STMT_string,
    STMT_bytes,
    STMT_number,
    STMT_method,
    STMT_chain,
    STMT_operator,
    STMT_inhert,
    STMT_import,
    STMT_list,
    STMT_slice,
    STMT_dict,
    STMT_none,
};

typedef struct Asmer Asmer;
struct Asmer {
    char* asm_code;
    uint8_t block_deepth_now;
    uint8_t is_new_line;
    char* line_pointer;
};

typedef enum _GenRuleValType {
    VAL_NONEVAL,
    VAL_DYNAMIC,
    VAL_STATIC_,
} GenRuleValType;

typedef struct GenRule {
    char* ins;
    GenRuleValType type;
    char* ast;
    char* val;
} GenRule;

typedef struct BlockState {
    Stack* stack;
    int deepth;
} BlockState;

typedef struct Parser Parser;
typedef char* (*fn_parser_Ast2Target)(Parser* self, AST* ast);
typedef char* (*fn_parser_Lines2Target)(Parser* self, char* sPyLines);

#define _VAL_NEED_INIT -1
#define PIKA_BLOCK_SPACE 4

struct Parser {
    Args lineBuffs;
    Args genBuffs;
    BlockState blockState;
    int blockDeepthOrigin;
    fn_parser_Ast2Target fn_ast2Target;
    pika_bool isGenBytecode;
    ByteCodeFrame* bytecode_frame;
    uint8_t thisBlockDeepth;
    uint32_t label_pc;
};

typedef struct LexToken LexToken;
struct LexToken {
    char* tokenStream;
    enum TokenType type;
    char* pyload;
};

typedef struct Cursor ParsetState;
struct Cursor {
    char* tokenStream;
    uint16_t length;
    uint16_t iter_index;
    int8_t bracket_deepth;
    struct LexToken token1;
    struct LexToken token2;
    Arg* last_token;
    Args* iter_buffs;
    Args* buffs_p;
    PIKA_RES result;
};

char* pika_lines2Asm(Args* outBuffs, char* multiLine);
char* pika_line2Asm(Args* buffs_p, char* line, Stack* blockStack);
int parser_file2DocFile(Parser* self, char* sPyFile, char* sDocFile);
PIKA_RES pika_lines2Bytes(ByteCodeFrame* bf, char* py_lines);
Parser* parser_create(void);
int parser_deinit(Parser* parser);
int32_t AST_deinit(AST* ast);
ByteCodeFrame* byteCodeFrame_appendFromAsm(ByteCodeFrame* bf, char* pikaAsm);

#endif

#ifdef __cplusplus
}
#endif
