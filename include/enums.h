#pragma once

/*********************************
 * OBJ TYPE TABLE
 * 
 * 1 - Variable
 * 2 - Procedure
 * 3 - Function
 *********************************/
enum
{
    OT_VARIABLE = 1,
    OT_PROCEDURE,
    OT_FUNCTION
};

/*********************************
 * TRANSP TYPE TABLE
 * 
 * 1 - Local Storage
 * 2 - Passage by value
 * 3 - Passage by reference
 *********************************/
enum
{
    LOCAL_STORAGE = 1,
    PASS_BY_VALUE,
    PASS_BY_REF,
};

/*********************************
 * VAR TYPE TABLE
 * 
 * -1 - Incompatible
 * 0 - Void
 * 1 - Boolean
 * 2 - Int 32
 * 3 - Int 64
 * 4 - Float 32
 * 5 - Float 64
 *********************************/
enum
{
    INCOMPTBL = -1,
    VOID,
    BOOL,
    INT32,
    INT64,
    FLT32,
    FLT64,
};

/*********************************
 * Pascal accepted keywords enum
 *********************************/
enum
{
    BEGIN = 10001,
    NOT,
    AND,
    OR,
    DIV,
    MOD,
    PROGRAM,
    PROCEDURE,
    FUNCTION,
    VAR,
    BOOLEAN,
    INTEGER,
    REAL,
    DOUBLE,
    IF,
    THEN,
    ELSE,
    WHILE,
    DO,
    REPEAT,
    UNTIL,
    TRUE,
    FALSE,
    RETURN,
    END,
};