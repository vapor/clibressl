/*	$OpenBSD: getentropy_netbsd.c,v 1.1 2015/01/19 20:21:40 bcook Exp $	*/

/*
 * Copyright (c) 2014 Pawel Jakub Dawidek <pjd@FreeBSD.org>
 * Copyright (c) 2014 Brent Cook <bcook@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Emulation of getentropy(2) as documented at:
 * http://www.openbsd.org/cgi-bin/man.cgi/OpenBSD-current/man2/getentropy.2
 */

#if defined(__NetBSD__)

#include <sys/types.h>
#include <sys/sysctl.h>

#include <errno.h>
#include <stddef.h>

/*
 * Derived from lib/libc/gen/arc4random.c from FreeBSD.
 */
static size_t
getentropy_sysctl(u_char *buf, size_t size)
{
	int mib[2];
	size_t len, done;

	mib[0] = CTL_KERN;
	mib[1] = KERN_ARND;
	done = 0;

	do {
		len = size;
		if (sysctl(mib, 2, buf, &len, NULL, 0) == -1)
			return (done);
		done += len;
		buf += len;
		size -= len;
	} while (size > 0);

	return (done);
}

int
getentropy(void *buf, size_t len)
{
	if (len <= 256 &&
	    getentropy_sysctl(buf, len) == len) {
		return (0);
	}

	errno = EIO;
	return (-1);
}

#endif
