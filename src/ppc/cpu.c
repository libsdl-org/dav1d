/*
 * Copyright © 2019, VideoLAN and dav1d authors
 * Copyright © 2019, Janne Grunau
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include "common/attributes.h"

#include "src/cpu.h"
#include "src/ppc/cpu.h"

#define HAVE_AUX ((HAVE_GETAUXVAL || HAVE_ELF_AUX_INFO) && ARCH_PPC64LE)
#if HAVE_AUX
#include <sys/auxv.h>
#endif

COLD unsigned dav1d_get_cpu_flags_ppc(void) {
    unsigned flags = dav1d_get_default_cpu_flags();
#if HAVE_GETAUXVAL && ARCH_PPC64LE
    unsigned long hw_cap = getauxval(AT_HWCAP);
    unsigned long hw_cap2 = getauxval(AT_HWCAP2);
#elif HAVE_ELF_AUX_INFO && ARCH_PPC64LE
    unsigned long hw_cap = 0;
    unsigned long hw_cap2 = 0;
    elf_aux_info(AT_HWCAP, &hw_cap, sizeof(hw_cap));
    elf_aux_info(AT_HWCAP2, &hw_cap2, sizeof(hw_cap2));
#endif
#if HAVE_AUX
    flags |= (hw_cap & PPC_FEATURE_HAS_VSX) ? DAV1D_PPC_CPU_FLAG_VSX : 0;
    flags |= (hw_cap2 & PPC_FEATURE2_ARCH_3_00) ? DAV1D_PPC_CPU_FLAG_PWR9 : 0;
#endif
    return flags;
}
