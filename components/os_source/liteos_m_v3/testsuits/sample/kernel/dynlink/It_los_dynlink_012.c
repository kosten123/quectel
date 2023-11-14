/*
 * Copyright (c) 2021-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "los_dynlink.h"
#include "It_los_dynlink.h"

/* Test that global symbol int dynamic_static.so */
STATIC UINT32 TestCase(VOID)
{
    VOID *handle = NULL;
    INT32 (*func)(INT32, INT32) = NULL;
    CHAR *symbolName = "add_test";
    CHAR *dsoName = DSO_FULL_PATH("dynamic_static.so");
    INT32 ret;

    handle = (VOID *)LOS_SoLoad(dsoName, NULL);
    ICUNIT_ASSERT_NOT_EQUAL(handle, NULL, handle);

    func = (INT32 (*)(INT32, INT32))LOS_FindSym(handle, symbolName);
    ICUNIT_GOTO_NOT_EQUAL(func, NULL, func, EXIT);

    ret = func(2000, 16);
    ICUNIT_GOTO_EQUAL(ret, 2017, ret, EXIT);

    ret = func(2017, -1);
    ICUNIT_GOTO_EQUAL(ret, 2017, ret, EXIT);

EXIT:
    ret = LOS_SoUnload(handle);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    return LOS_OK;
}

VOID ItLosDynlink012(VOID)
{
    TEST_ADD_CASE("ItLosDynlink012", TestCase, TEST_LOS, TEST_DYNLINK, TEST_LEVEL1, TEST_FUNCTION);
}
