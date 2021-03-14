
#ifndef HAVE_STRLCPY
#define HAVE_STRLCPY

#pragma once

/*	$OpenBSD: strlcpy.c,v 1.4 1999/05/01 18:56:41 millert Exp $	*/

/*
 * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t strlcpy(char *dst, const char *src, size_t siz);

/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t strlcat(char *dst, const char *src, size_t siz);

/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns wcslen(src); if retval >= siz, truncation occurred.
 */
size_t wcslcpy(wchar_t *dst, const wchar_t *src, size_t siz);

/*
 * Appends src to string dst of size siz (unlike wcsncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns wcslen(initial dst) + wcslen(src); if retval >= siz,
 * truncation occurred.
 */
size_t wcslcat(wchar_t *dst, const wchar_t *src, size_t siz);

/*
 * Templated version of strlcpy with offset option
 */
template <size_t Siz>
size_t strlcpy_t(char (&dst)[Siz], const char* src, size_t off = 0)
{
	return strlcpy(dst + off, src, Siz - off);
}

/*
 * Templated version of strlcat with offset option
 */
template <size_t Siz>
size_t strlcat_t(char (&dst)[Siz], const char* src, size_t off = 0)
{
	return strlcat(dst + off, src, Siz - off);
}

/*
 * Templated version of wcslcpy with offset option
 */
template <size_t Siz>
size_t wcslcpy_t(wchar_t (&dst)[Siz], const wchar_t* src, size_t off = 0)
{
	return wcslcpy(dst + off, src, Siz - off);
}

/*
 * Templated version of wcslcat with offset option
 */
template <size_t Siz>
size_t wcslcat_t(wchar_t (&dst)[Siz], const wchar_t* src, size_t off = 0)
{
	return wcslcat(dst + off, src, Siz - off);
}

#endif /* HAVE_STRLCPY */
