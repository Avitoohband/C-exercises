#include "opcodes.h"

/*  the data of the operators */
aOperator aOperators[NUMBER_OF_OPERATORS] =
        {
                {
                        "mov", 4, 3, {INSTANT, DIRECT, REG_INDIRECT, REG_DIRECT },
                        {INVALID,DIRECT, REG_INDIRECT, REG_DIRECT }, 2,0
                },

                {
                        "cmp", 4, 4, {INSTANT, DIRECT, REG_INDIRECT, REG_DIRECT },
                        {INSTANT,DIRECT, REG_INDIRECT, REG_DIRECT }, 2,1
                },

                {
                        "add", 4, 3, {INSTANT, DIRECT, REG_INDIRECT, REG_DIRECT },
                        {INVALID,DIRECT, REG_INDIRECT, REG_DIRECT} , 2,2
                },

                {
                        "sub", 4, 3, {INSTANT, DIRECT, REG_INDIRECT, REG_DIRECT },
                        {INVALID,DIRECT, REG_INDIRECT, REG_DIRECT }, 2,3
                },

                {
                        "lea", 1, 3, {INVALID, DIRECT, INVALID, INVALID },
                        {INVALID,DIRECT, REG_INDIRECT, REG_DIRECT }, 2,4
                },

                {
                        "clr", 0, 3, {INVALID, INVALID, INVALID, INVALID },
                        {INVALID,DIRECT, REG_INDIRECT, REG_DIRECT }, 1,5
                },

                {
                        "not", 0, 3, {INVALID, INVALID, INVALID, INVALID },
                        {INVALID,DIRECT, REG_INDIRECT, REG_DIRECT }, 1,6
                },
                {
                        "inc", 0, 3, {INVALID, INVALID, INVALID, INVALID },
                        {INVALID,DIRECT, REG_INDIRECT, REG_DIRECT }, 1,7
                },

                {
                        "dec", 0, 3, {INVALID, INVALID, INVALID, INVALID },
                        {INVALID,DIRECT, REG_INDIRECT, REG_DIRECT}, 1,8
                },

                {
                        "jmp", 0, 2, {INVALID, INVALID, INVALID, INVALID },
                        {INVALID,DIRECT, REG_INDIRECT, INVALID} , 1,9
                },

                {
                        "bne", 0, 2, {INVALID, INVALID, INVALID, INVALID },
                        {INVALID,DIRECT, REG_INDIRECT, INVALID }, 1,10
                },

                {
                        "red", 0, 3, {INVALID, INVALID, INVALID, INVALID },
                        {INVALID,DIRECT, REG_INDIRECT, REG_DIRECT }, 1,11
                },

                {
                        "prn", 0, 4, {INVALID, INVALID, INVALID, INVALID },
                        {INSTANT,DIRECT, REG_INDIRECT, REG_DIRECT }, 1,12
                },

                {
                        "jsr", 0, 2, {INVALID, INVALID, INVALID, INVALID },
                        {INVALID,DIRECT, REG_INDIRECT, INVALID } ,1,13
                },

                {
                        "rts", 0, 0, {INVALID, INVALID, INVALID, INVALID },
                        {INVALID,INVALID, INVALID, INVALID },1,14
                },

                {
                        "stop", 0, 0, {INVALID, INVALID, INVALID, INVALID },
                        {INVALID,INVALID, INVALID, INVALID },0,15
                }
        };