
#pragma once
#define MAXIDLEN 32
#define MAXSTBENTRIES 0x10000
#define OFFSETSIZE 32

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