#include "defs.h"

#if defined I386 || defined X86_64 || defined X32

# include <asm/ldt.h>

void
print_user_desc(struct tcb *tcp, const long addr)
{
	struct user_desc desc;

	if (umove_or_printaddr(tcp, addr, &desc))
		return;

	tprintf("{entry_number:%d, "
		"base_addr:%#08x, "
		"limit:%d, "
		"seg_32bit:%d, "
		"contents:%d, "
		"read_exec_only:%d, "
		"limit_in_pages:%d, "
		"seg_not_present:%d, "
		"useable:%d}",
		desc.entry_number,
		desc.base_addr,
		desc.limit,
		desc.seg_32bit,
		desc.contents,
		desc.read_exec_only,
		desc.limit_in_pages,
		desc.seg_not_present,
		desc.useable);
}

SYS_FUNC(modify_ldt)
{
	tprintf("%ld, ", tcp->u_arg[0]);
	if (tcp->u_arg[2] != sizeof(struct user_desc))
		printaddr(tcp->u_arg[1]);
	else
		print_user_desc(tcp, tcp->u_arg[1]);
	tprintf(", %lu", tcp->u_arg[2]);

	return RVAL_DECODED;
}

SYS_FUNC(set_thread_area)
{
	if (entering(tcp)) {
		print_user_desc(tcp, tcp->u_arg[0]);
	} else {
		struct user_desc desc;

		if (!verbose(tcp) || syserror(tcp) ||
		    umove(tcp, tcp->u_arg[0], &desc) < 0) {
			/* returned entry_number is not available */
		} else {
			static char outstr[32];

			sprintf(outstr, "entry_number:%d", desc.entry_number);
			tcp->auxstr = outstr;
			return RVAL_STR;
		}
	}
	return 0;
}

SYS_FUNC(get_thread_area)
{
	if (exiting(tcp))
		print_user_desc(tcp, tcp->u_arg[0]);
	return 0;
}

#endif /* I386 || X86_64 || X32 */

#if defined(M68K) || defined(MIPS)
SYS_FUNC(set_thread_area)
{
	printaddr(tcp->u_arg[0]);

	return RVAL_DECODED;

}
#endif

#if defined(M68K)
SYS_FUNC(get_thread_area)
{
	return RVAL_DECODED | RVAL_HEX;
}
#endif
