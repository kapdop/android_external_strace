/* Generated by ./xlat/gen.sh from ./xlat/seccomp_ops.in; do not edit. */
#if !(defined(SECCOMP_SET_MODE_STRICT) || (defined(HAVE_DECL_SECCOMP_SET_MODE_STRICT) && HAVE_DECL_SECCOMP_SET_MODE_STRICT))
# define SECCOMP_SET_MODE_STRICT 0
#endif
#if !(defined(SECCOMP_SET_MODE_FILTER) || (defined(HAVE_DECL_SECCOMP_SET_MODE_FILTER) && HAVE_DECL_SECCOMP_SET_MODE_FILTER))
# define SECCOMP_SET_MODE_FILTER 1
#endif

#ifdef IN_MPERS

# error static const struct xlat seccomp_ops in mpers mode

#else

static
const struct xlat seccomp_ops[] = {
 XLAT(SECCOMP_SET_MODE_STRICT),
 XLAT(SECCOMP_SET_MODE_FILTER),
 XLAT_END
};

#endif /* !IN_MPERS */
