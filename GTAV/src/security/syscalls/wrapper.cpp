#include "wrapper.h"
#include "syscall.h"
#include <mutex>

namespace security::syscalls {
	std::mutex m_mutex[10];

	LONG NtOpenProcessToken(HANDLE handle, ACCESS_MASK mask, PHANDLE token) {
        LONG return_ = 0;

        syscalls::syscall_context* context = syscalls::get_context(XOR("NtOpenProcessToken"));
        if (context) {
            m_mutex[0].lock();
            syscalls::decrypt_context(context);

            return_ = ((LONG(*)(...))context->m_shellcode)(handle, mask, token);

            syscalls::encrypt_context(context);
            m_mutex[0].unlock();

            return return_;
        }

        return -1;
	}

    LONG NtClose(HANDLE handle) {
        LONG return_ = 0;

        syscalls::syscall_context* context = syscalls::get_context(XOR("NtClose"));
        if (context) {
            m_mutex[1].lock();
            syscalls::decrypt_context(context);

            return_ = ((LONG(*)(...))context->m_shellcode)(handle);

            syscalls::encrypt_context(context);
            m_mutex[1].unlock();

            return return_;
        }

        return -1;
    }

    LONG NtAdjustPrivilegesToken(HANDLE handle, BOOLEAN DisableAllPrivileges, PTOKEN_PRIVILEGES TokenPrivileges, ULONG PreviousPrivilegesLength, PTOKEN_PRIVILEGES PreviousPrivileges, PULONG RequiredLength) {
        LONG return_ = 0;

        syscalls::syscall_context* context = syscalls::get_context(XOR("NtAdjustPrivilegesToken"));
        if (context) {
            m_mutex[2].lock();
            syscalls::decrypt_context(context);

            return_ = ((LONG(*)(...))context->m_shellcode)(handle, DisableAllPrivileges, TokenPrivileges, PreviousPrivilegesLength, PreviousPrivileges, RequiredLength);

            syscalls::encrypt_context(context);
            m_mutex[2].unlock();

            return return_;
        }

        return -1;
    }

    LONG NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, int UnicodeStringParameterMask, PVOID* Parameters, int ResponseOption, PULONG Response) {
        LONG return_ = 0;

        syscalls::syscall_context* context = syscalls::get_context(XOR("NtRaiseHardError"));
        if (context) {
            m_mutex[3].lock();
            syscalls::decrypt_context(context);

            return_ = ((LONG(*)(...))context->m_shellcode)(ErrorStatus, NumberOfParameters, UnicodeStringParameterMask, Parameters, ResponseOption, Response);

            syscalls::encrypt_context(context);
            m_mutex[3].unlock();

            return return_;
        }

        return -1;
    }

    LONG NtSuspendProcess(HANDLE handle) {
        LONG return_ = 0;

        syscalls::syscall_context* context = syscalls::get_context(XOR("NtSuspendProcess"));
        if (context) {
            m_mutex[4].lock();
            syscalls::decrypt_context(context);

            return_ = ((LONG(*)(...))context->m_shellcode)(handle);

            syscalls::encrypt_context(context);
            m_mutex[4].unlock();

            return return_;
        }

        return -1;
    }
}