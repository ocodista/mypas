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

// Enum with variable types
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