/* Pindel to SAM converter
 ****
 *   Copyright (C) 2014 Adam D Scott
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****
 *
 * Args: <pindel data directory> <output directory> config_file fafai_file
 * 
 * Description: Converts Pindel data files (_D & _SI) into SAM format.
 * 
 * NOTES: 
 *  dirent.h taken from users.cis.fiu.edu/~weiss/cop4338_spr06/dirent.h.
 *  Make sure that the output directory does not contain any previously generated output files.
 *  Any files ending with _D or _SI in the pindel data directory will be read.
 *  Sub directories are not checked for input.
 *  One must create any directory path included with the samples listed in the config file before converting.
 *  Filler data is written for FLAG, MAPQ, RNEXT, PNEXT, TLEN, and QUAL for each read.
 *  After running, convert output SAM files to BAM, then sort and index the BAM files.
 *  The BAM files listed in the first column of the config_file are the basis for each of the output SAM files.
 *  All _D & _SI supports are listed in corresponding output files.
 */

///////////////////////////////////////////////////////////////////////////
//The following is a copy of dirent.h from 
// users.cis.fiu.edu/~weiss/cop4338_spr06/dirent.h downloaded 7 July 2014.
/*
 * uce-dirent.h - operating system independent dirent implementation
 * 
 * Copyright (C) 1998-2002  Toni Ronkko
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * ``Software''), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL TONI RONKKO BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * 
 * May 28 1998, Toni Ronkko <tronkko@messi.uku.fi>
 *
 * $Id: uce-dirent.h,v 1.7 2002/05/13 10:48:35 tr Exp $
 *
 * $Log: uce-dirent.h,v $
 * Revision 1.7  2002/05/13 10:48:35  tr
 * embedded some source code directly to the header so that no source
 * modules need to be included in the MS Visual C project using the
 * interface, removed all the dependencies to other headers of the `uce'
 * library so that the header can be made public
 *
 * Revision 1.6  2002/04/12 16:22:04  tr
 * Unified Compiling Environment (UCE) replaced `std' library
 *
 * Revision 1.5  2001/07/20 16:33:40  tr
 * moved to `std' library and re-named defines accordingly
 *
 * Revision 1.4  2001/07/10 16:47:18  tronkko
 * revised comments
 *
 * Revision 1.3  2001/01/11 13:16:43  tr
 * using ``uce-machine.h'' for finding out defines such as `FREEBSD'
 *
 * Revision 1.2  2000/10/08 16:00:41  tr
 * copy of FreeBSD man page
 *
 * Revision 1.1  2000/07/10 05:53:16  tr
 * Initial revision
 *
 * Revision 1.2  1998/07/19 18:29:14  tr
 * Added error reporting capabilities and some asserts.
 *
 * Revision 1.1  1998/07/04 16:27:51  tr
 * Initial revision
 *
 * 
 * MSVC 1.0 scans automatic dependencies incorrectly when your project
 * contains this very header.  The problem is that MSVC cannot handle
 * include directives inside #if..#endif block those are never entered.
 * Since this header ought to compile in many different operating systems,
 * there had to be several conditional blocks that are compiled only in
 * operating systems for what they were designed for.  MSVC 1.0 cannot
 * handle inclusion of sys/dir.h in a part that is compiled only in Apollo
 * operating system.  To fix the problem you need to insert DIR.H into
 * SYSINCL.DAT located in MSVC\BIN directory and restart visual C++.
 * Consult manuals for more informaton about the problem.
 *
 * Since many UNIX systems have dirent.h we assume to have one also.
 * However, if your UNIX system does not have dirent.h you can download one
 * for example at: http://ftp.uni-mannheim.de/ftp/GNU/dirent/dirent.tar.gz.
 * You can also see if you have one of dirent.h, direct.h, dir.h, ndir.h,
 * sys/dir.h and sys/ndir.h somewhere.  Try defining HAVE_DIRENT_H,
 * HAVE_DIRECT_H, HAVE_DIR_H, HAVE_NDIR_H, HAVE_SYS_DIR_H and
 * HAVE_SYS_NDIR_H according to the files found.
 */
#ifndef DIRENT_H
#define DIRENT_H
#define DIRENT_H_INCLUDED

/* find out platform */
#if defined(MSDOS)                             /* MS-DOS */
#elif defined(__MSDOS__)                       /* Turbo C/Borland */
# define MSDOS
#elif defined(__DOS__)                         /* Watcom */
# define MSDOS
#endif

#if defined(WIN32)                             /* MS-Windows */
#elif defined(__NT__)                          /* Watcom */
# define WIN32
#elif defined(_WIN32)                          /* Microsoft */
# define WIN32
#elif defined(__WIN32__)                       /* Borland */
# define WIN32
#endif

/*
 * See what kind of dirent interface we have unless autoconf has already
 * determinated that.
 */
#if !defined(HAVE_DIRENT_H) && !defined(HAVE_DIRECT_H) && !defined(HAVE_SYS_DIR_H) && !defined(HAVE_NDIR_H) && !defined(HAVE_SYS_NDIR_H) && !defined(HAVE_DIR_H)
# if defined(_MSC_VER)                         /* Microsoft C/C++ */
    /* no dirent.h */
# elif defined(__BORLANDC__)                   /* Borland C/C++ */
#   define HAVE_DIRENT_H
#   define VOID_CLOSEDIR
# elif defined(__TURBOC__)                     /* Borland Turbo C */
    /* no dirent.h */
# elif defined(__WATCOMC__)                    /* Watcom C/C++ */
#   define HAVE_DIRECT_H
# elif defined(__apollo)                       /* Apollo */
#   define HAVE_SYS_DIR_H
# elif defined(__hpux)                         /* HP-UX */
#   define HAVE_DIRENT_H
# elif defined(__alpha) || defined(__alpha__)  /* Alpha OSF1 */
#   error "not implemented"
# elif defined(__sgi)                          /* Silicon Graphics */
#   define HAVE_DIRENT_H
# elif defined(sun) || defined(_sun)           /* Sun Solaris */
#   define HAVE_DIRENT_H
# elif defined(__FreeBSD__)                    /* FreeBSD */
#   define HAVE_DIRENT_H
# elif defined(__linux__)                      /* Linux */
#   define HAVE_DIRENT_H
# elif defined(__GNUC__)                       /* GNU C/C++ */
#   define HAVE_DIRENT_H
# else
#   error "not implemented"
# endif
#endif

/* include proper interface headers */
#if defined(HAVE_DIRENT_H)
# include <dirent.h>
# ifdef FREEBSD
#   define NAMLEN(dp) ((int)((dp)->d_namlen))
# else
#   define NAMLEN(dp) ((int)(strlen((dp)->d_name)))
# endif

#elif defined(HAVE_NDIR_H)
# include <ndir.h>
# define NAMLEN(dp) ((int)((dp)->d_namlen))

#elif defined(HAVE_SYS_NDIR_H)
# include <sys/ndir.h>
# define NAMLEN(dp) ((int)((dp)->d_namlen))

#elif defined(HAVE_DIRECT_H)
# include <direct.h>
# define NAMLEN(dp) ((int)((dp)->d_namlen))

#elif defined(HAVE_DIR_H)
# include <dir.h>
# define NAMLEN(dp) ((int)((dp)->d_namlen))

#elif defined(HAVE_SYS_DIR_H)
# include <sys/types.h>
# include <sys/dir.h>
# ifndef dirent
#   define dirent direct
# endif
# define NAMLEN(dp) ((int)((dp)->d_namlen))

#elif defined(MSDOS) || defined(WIN32)

  /* figure out type of underlaying directory interface to be used */
# if defined(WIN32)
#   define DIRENT_WIN32_INTERFACE
# elif defined(MSDOS)
#   define DIRENT_MSDOS_INTERFACE
# else
#   error "missing native dirent interface"
# endif

  /*** WIN32 specifics ***/
# if defined(DIRENT_WIN32_INTERFACE)
#   include <windows.h>
#   if !defined(DIRENT_MAXNAMLEN)
#     define DIRENT_MAXNAMLEN (MAX_PATH)
#   endif


  /*** MS-DOS specifics ***/
# elif defined(DIRENT_MSDOS_INTERFACE)
#   include <dos.h>

    /* Borland defines file length macros in dir.h */
#   if defined(__BORLANDC__)
#     include <dir.h>
#     if !defined(DIRENT_MAXNAMLEN)
#       define DIRENT_MAXNAMLEN ((MAXFILE)+(MAXEXT))
#     endif
#     if !defined(_find_t)
#       define _find_t find_t
#     endif

    /* Turbo C defines ffblk structure in dir.h */
#   elif defined(__TURBOC__)
#     include <dir.h>
#     if !defined(DIRENT_MAXNAMLEN)
#       define DIRENT_MAXNAMLEN ((MAXFILE)+(MAXEXT))
#     endif
#     define DIRENT_USE_FFBLK

    /* MSVC */
#   elif defined(_MSC_VER)
#     if !defined(DIRENT_MAXNAMLEN)
#       define DIRENT_MAXNAMLEN (12)
#     endif

    /* Watcom */
#   elif defined(__WATCOMC__)
#     if !defined(DIRENT_MAXNAMLEN)
#       if defined(__OS2__) || defined(__NT__)
#         define DIRENT_MAXNAMLEN (255)
#       else
#         define DIRENT_MAXNAMLEN (12)
#       endif
#     endif

#   endif
# endif

  /*** generic MS-DOS and MS-Windows stuff ***/
# if !defined(NAME_MAX) && defined(DIRENT_MAXNAMLEN)
#   define NAME_MAX DIRENT_MAXNAMLEN
# endif
# if NAME_MAX < DIRENT_MAXNAMLEN
#   error "assertion failed: NAME_MAX >= DIRENT_MAXNAMLEN"
# endif


  /*
   * Substitute for real dirent structure.  Note that `d_name' field is a
   * true character array although we have it copied in the implementation
   * dependent data.  We could save some memory if we had declared `d_name'
   * as a pointer refering the name within implementation dependent data.
   * We have not done that since some code may rely on sizeof(d_name) to be
   * something other than four.  Besides, directory entries are typically so
   * small that it takes virtually no time to copy them from place to place.
   */
  typedef struct dirent {
    char d_name[NAME_MAX + 1];

    /*** Operating system specific part ***/
# if defined(DIRENT_WIN32_INTERFACE)       /*WIN32*/
    WIN32_FIND_DATA data;
# elif defined(DIRENT_MSDOS_INTERFACE)     /*MSDOS*/
#   if defined(DIRENT_USE_FFBLK)
    struct ffblk data;
#   else
    struct _find_t data;
#   endif
# endif
  } dirent;

  /* DIR substitute structure containing directory name.  The name is
   * essential for the operation of ``rewinndir'' function. */
  typedef struct DIR {
    char          *dirname;                    /* directory being scanned */
    dirent        current;                     /* current entry */
    int           dirent_filled;               /* is current un-processed? */

  /*** Operating system specific part ***/
#  if defined(DIRENT_WIN32_INTERFACE)
    HANDLE        search_handle;
#  elif defined(DIRENT_MSDOS_INTERFACE)
#  endif
  } DIR;

# ifdef __cplusplus
extern "C" {
# endif

/* supply prototypes for dirent functions */
static DIR *opendir (const char *dirname);
static struct dirent *readdir (DIR *dirp);
static int closedir (DIR *dirp);
static void rewinddir (DIR *dirp);

/*
 * Implement dirent interface as static functions so that the user does not
 * need to change his project in any way to use dirent function.  With this
 * it is sufficient to include this very header from source modules using
 * dirent functions and the functions will be pulled in automatically.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

/* use ffblk instead of _find_t if requested */
#if defined(DIRENT_USE_FFBLK)
# define _A_ARCH   (FA_ARCH)
# define _A_HIDDEN (FA_HIDDEN)
# define _A_NORMAL (0)
# define _A_RDONLY (FA_RDONLY)
# define _A_SUBDIR (FA_DIREC)
# define _A_SYSTEM (FA_SYSTEM)
# define _A_VOLID  (FA_LABEL)
# define _dos_findnext(dest) findnext(dest)
# define _dos_findfirst(name,flags,dest) findfirst(name,dest,flags)
#endif

static int _initdir (DIR *p);
static const char *_getdirname (const struct dirent *dp);
static void _setdirname (struct DIR *dirp);

/*
 * <function name="opendir">
 * <intro>open directory stream for reading
 * <syntax>DIR *opendir (const char *dirname);
 *
 * <desc>Open named directory stream for read and return pointer to the
 * internal working area that is used for retrieving individual directory
 * entries.  The internal working area has no fields of your interest.
 *
 * <ret>Returns a pointer to the internal working area or NULL in case the 
 * directory stream could not be opened.  Global `errno' variable will set
 * in case of error as follows:
 *
 * <table>
 * [EACESS  |Permission denied.
 * [EMFILE  |Too many open files used by the process.
 * [ENFILE  |Too many open files in system.
 * [ENOENT  |Directory does not exist.
 * [ENOMEM  |Insufficient memory.
 * [ENOTDIR |dirname does not refer to directory.  This value is not
 *           reliable on MS-DOS and MS-Windows platforms.  Many
 *           implementations return ENOENT even when the name refers to a
 *           file.]
 * </table>
 * </function>
 */
static DIR *
opendir(
    const char *dirname)
{
  DIR *dirp;
  assert (dirname != NULL);
  
  dirp = (DIR*)malloc (sizeof (struct DIR));
  if (dirp != NULL) {
    char *p;
    
    /* allocate room for directory name */
    dirp->dirname = (char*) malloc (strlen (dirname) + 1 + strlen ("\\*.*"));
    if (dirp->dirname == NULL) {
      /* failed to duplicate directory name.  errno set by malloc() */
      free (dirp);
      return NULL;
    }
    /* Copy directory name while appending directory separator and "*.*".
     * Directory separator is not appended if the name already ends with
     * drive or directory separator.  Directory separator is assumed to be
     * '/' or '\' and drive separator is assumed to be ':'. */
    strcpy (dirp->dirname, dirname);
    p = strchr (dirp->dirname, '\0');
    if (dirp->dirname < p  &&
        *(p - 1) != '\\'  &&  *(p - 1) != '/'  &&  *(p - 1) != ':')
    {
      strcpy (p++, "\\");
    }
# ifdef DIRENT_WIN32_INTERFACE
    strcpy (p, "*"); /*scan files with and without extension in win32*/
# else
    strcpy (p, "*.*"); /*scan files with and without extension in DOS*/
# endif

    /* open stream */
    if (_initdir (dirp) == 0) {
      /* initialization failed */
      free (dirp->dirname);
      free (dirp);
      return NULL;
    }
  }
  return dirp;
}


/*
 * <function name="readdir">
 * <intro>read a directory entry
 * <syntax>struct dirent *readdir (DIR *dirp);
 *
 * <desc>Read individual directory entry and return pointer to a structure
 * containing the name of the entry.  Individual directory entries returned
 * include normal files, sub-directories, pseudo-directories "." and ".."
 * and also volume labels, hidden files and system files in MS-DOS and
 * MS-Windows.   You might want to use stat(2) function to determinate which
 * one are you dealing with.  Many dirent implementations already contain
 * equivalent information in dirent structure but you cannot depend on
 * this.
 *
 * The dirent structure contains several system dependent fields that
 * generally have no interest to you.  The only interesting one is char
 * d_name[] that is also portable across different systems.  The d_name
 * field contains the name of the directory entry without leading path.
 * While d_name is portable across different systems the actual storage
 * capacity of d_name varies from system to system and there is no portable
 * way to find out it at compile time as different systems define the
 * capacity of d_name with different macros and some systems do not define
 * capacity at all (besides actual declaration of the field). If you really
 * need to find out storage capacity of d_name then you might want to try
 * NAME_MAX macro. The NAME_MAX is defined in POSIX standard althought
 * there are many MS-DOS and MS-Windows implementations those do not define
 * it.  There are also systems that declare d_name as "char d_name[1]" and
 * then allocate suitable amount of memory at run-time.  Thanks to Alain
 * Decamps (Alain.Decamps@advalvas.be) for pointing it out to me.
 *
 * This all leads to the fact that it is difficult to allocate space
 * for the directory names when the very same program is being compiled on
 * number of operating systems.  Therefore I suggest that you always
 * allocate space for directory names dynamically.
 *
 * <ret>
 * Returns a pointer to a structure containing name of the directory entry
 * in `d_name' field or NULL if there was an error.  In case of an error the
 * global `errno' variable will set as follows:
 *
 * <table>
 * [EBADF  |dir parameter refers to an invalid directory stream.  This value
 *          is not set reliably on all implementations.]
 * </table>
 * </function>
 */
static struct dirent *
readdir (DIR *dirp)
{
  assert (dirp != NULL);
  if (dirp == NULL) {
    errno = EBADF;
    return NULL;
  }

#if defined(DIRENT_WIN32_INTERFACE)
  if (dirp->search_handle == INVALID_HANDLE_VALUE) {
    /* directory stream was opened/rewound incorrectly or it ended normally */
    errno = EBADF;
    return NULL;
  }
#endif

  if (dirp->dirent_filled != 0) {
    /*
     * Directory entry has already been retrieved and there is no need to
     * retrieve a new one.  Directory entry will be retrieved in advance
     * when the user calls readdir function for the first time.  This is so
     * because real dirent has separate functions for opening and reading
     * the stream whereas Win32 and DOS dirents open the stream
     * automatically when we retrieve the first file.  Therefore, we have to
     * save the first file when opening the stream and later we have to
     * return the saved entry when the user tries to read the first entry.
     */
    dirp->dirent_filled = 0;
  } else {
    /* fill in entry and return that */
#if defined(DIRENT_WIN32_INTERFACE)
    if (FindNextFile (dirp->search_handle, &dirp->current.data) == FALSE) {
      /* Last file has been processed or an error occured */
      FindClose (dirp->search_handle);
      dirp->search_handle = INVALID_HANDLE_VALUE;
      errno = ENOENT;
      return NULL;
    }

# elif defined(DIRENT_MSDOS_INTERFACE)
    if (_dos_findnext (&dirp->current.data) != 0) {
      /* _dos_findnext and findnext will set errno to ENOENT when no
       * more entries could be retrieved. */
      return NULL;
    }
# endif

    _setdirname (dirp);
    assert (dirp->dirent_filled == 0);
  }
  return &dirp->current;
}


/*
 * <function name="closedir">
 * <intro>close directory stream.
 * <syntax>int closedir (DIR *dirp);
 *
 * <desc>Close directory stream opened by the `opendir' function.  Close of
 * directory stream invalidates the DIR structure as well as previously read
 * dirent entry.
 *
 * <ret>The function typically returns 0 on success and -1 on failure but
 * the function may be declared to return void on same systems.  At least
 * Borland C/C++ and some UNIX implementations use void as a return type.
 * The dirent wrapper tries to define VOID_CLOSEDIR whenever closedir is
 * known to return nothing.  The very same definition is made by the GNU
 * autoconf if you happen to use it.
 *
 * The global `errno' variable will set to EBADF in case of error.
 * </function>
 */
static int
closedir (DIR *dirp)
{   
  int retcode = 0;

  /* make sure that dirp points to legal structure */
  assert (dirp != NULL);
  if (dirp == NULL) {
    errno = EBADF;
    return -1;
  }
 
  /* free directory name and search handles */
  if (dirp->dirname != NULL) free (dirp->dirname);

#if defined(DIRENT_WIN32_INTERFACE)
  if (dirp->search_handle != INVALID_HANDLE_VALUE) {
    if (FindClose (dirp->search_handle) == FALSE) {
      /* Unknown error */
      retcode = -1;
      errno = EBADF;
    }
  }
#endif                     

  /* clear dirp structure to make sure that it cannot be used anymore*/
  memset (dirp, 0, sizeof (*dirp));
# if defined(DIRENT_WIN32_INTERFACE)
  dirp->search_handle = INVALID_HANDLE_VALUE;
# endif

  free (dirp);
  return retcode;
}


/*
 * <function name="rewinddir">
 * <intro>rewind directory stream to the beginning
 * <syntax>void rewinddir (DIR *dirp);
 *
 * <desc>Rewind directory stream to the beginning so that the next call of
 * readdir() returns the very first directory entry again.  However, note
 * that next call of readdir() may not return the same directory entry as it
 * did in first time.  The directory stream may have been affected by newly
 * created files.
 *
 * Almost every dirent implementation ensure that rewinddir will update
 * the directory stream to reflect any changes made to the directory entries
 * since the previous ``opendir'' or ``rewinddir'' call.  Keep an eye on
 * this if your program depends on the feature.  I know at least one dirent
 * implementation where you are required to close and re-open the stream to
 * see the changes.
 *
 * <ret>Returns nothing.  If something went wrong while rewinding, you will
 * notice it later when you try to retrieve the first directory entry.
 */
static void
rewinddir (DIR *dirp)
{   
  /* make sure that dirp is legal */
  assert (dirp != NULL);
  if (dirp == NULL) {
    errno = EBADF;
    return;
  }
  assert (dirp->dirname != NULL);
  
  /* close previous stream */
#if defined(DIRENT_WIN32_INTERFACE)
  if (dirp->search_handle != INVALID_HANDLE_VALUE) {
    if (FindClose (dirp->search_handle) == FALSE) {
      /* Unknown error */
      errno = EBADF;
    }
  }
#endif

  /* re-open previous stream */
  if (_initdir (dirp) == 0) {
    /* initialization failed but we cannot deal with error.  User will notice
     * error later when she tries to retrieve first directory enty. */
    /*EMPTY*/;
  }
}


/*
 * Open native directory stream object and retrieve first file.
 * Be sure to close previous stream before opening new one.
 */
static int
_initdir (DIR *dirp)
{ 
  assert (dirp != NULL);
  assert (dirp->dirname != NULL);
  dirp->dirent_filled = 0;

# if defined(DIRENT_WIN32_INTERFACE)
  /* Open stream and retrieve first file */
  dirp->search_handle = FindFirstFile (dirp->dirname, &dirp->current.data);
  if (dirp->search_handle == INVALID_HANDLE_VALUE) {
    /* something went wrong but we don't know what.  GetLastError() could
     * give us more information about the error, but then we should map
     * the error code into errno. */
    errno = ENOENT;
    return 0;
  }

# elif defined(DIRENT_MSDOS_INTERFACE)
  if (_dos_findfirst (dirp->dirname,
          _A_SUBDIR | _A_RDONLY | _A_ARCH | _A_SYSTEM | _A_HIDDEN,
          &dirp->current.data) != 0)
  {
    /* _dos_findfirst and findfirst will set errno to ENOENT when no 
     * more entries could be retrieved. */
    return 0;
  }
# endif

  /* initialize DIR and it's first entry */
  _setdirname (dirp);
  dirp->dirent_filled = 1;
  return 1;
}


/*
 * Return implementation dependent name of the current directory entry.
 */
static const char *
_getdirname (const struct dirent *dp)
{
#if defined(DIRENT_WIN32_INTERFACE)
  return dp->data.cFileName;
  
#elif defined(DIRENT_USE_FFBLK)
  return dp->data.ff_name;
  
#else
  return dp->data.name;
#endif  
}


/*
 * Copy name of implementation dependent directory entry to the d_name field.
 */
static void
_setdirname (struct DIR *dirp) {
  /* make sure that d_name is long enough */
  assert (strlen (_getdirname (&dirp->current)) <= NAME_MAX);
  
  strncpy (dirp->current.d_name,
      _getdirname (&dirp->current),
      NAME_MAX);
  dirp->current.d_name[NAME_MAX] = '\0'; /*char d_name[NAME_MAX+1]*/
}
  
# ifdef __cplusplus
}
# endif
# define NAMLEN(dp) ((int)(strlen((dp)->d_name)))

#else
# error "missing dirent interface"
#endif


#endif /*DIRENT_H*/
///////////////////////////////////////////////////////////////////////////

/* pindel2sam */ 
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
//#include <string>
#include <vector>
#include <map>

//#include <dirent.h>

const std::string SAMVERSION = "1.5";
const std::string PINDELVERSION = "0.2.4";

const int NUMBEROFPOUNDS = 100;
const int NUMBEROFSUMMARYFIELDS = 31;
const int NUMBEROFSUMMARYSAMPLEFIELDS = 7;
const int UPDATEFREQUENCY = 10000;
int NUMBEROFSAMPLES;
int linenum = 0;
int value = 0; //use for error values
std::string outputDirectoryName = "";

int str2int( const std::string& );
std::string int2str( const int& );

bool check_ending( const std::string );
void check_separation( std::ifstream& , const char& );

int read_config_file( const std::string& , std::map<std::string,std::string>& , std::map<std::string,int>& );
int read_fafai_file( const std::string& , struct header& );

void set_header_custom( struct header& , const std::string& );
void set_header_bottom( struct header& );
int set_reference_detail( std::ifstream& , struct pindel_fields& );
int set_pindel_fields( std::ifstream& , struct pindel_fields& );
void set_support( std::ifstream& , int , struct support_data& , std::map<std::string,std::string>& , std::map<std::string,int>& , const int );
void set_supports( std::ifstream& , struct pindel_fields& , std::map<std::string,std::string>& , std::map<std::string,int>& , const int ); //fills supports field of pindel_field struct

void field_conversion( struct pindel_fields& , int , struct sam_fields& );
std::string create_CIGAR( std::string , std::string , std::string , int , int , bool& ); //indelType, indelSize, NT_size, readLength, leftIndelPos = BPLeft_plus_one - POS + 1, do true CIGAR
std::string determine_POS( const std::string , const int ); //leftReadLength, BPLeft_plus_one

void print_update( int& );
void print_header( const struct header& );
void print_pindel_fields( const struct pindel_fields& ); //prints all strings in pindel_fields struct
void print_support_with_summary( const struct pindel_fields& , int );
void print_support_data( const struct support_data& );
void print_supports( const std::vector<struct support_data>& ); //prints int and all strings in support_data struct
void print_sam( const struct sam_fields& );

void clear_support_data( struct support_data& ); //clears all data in support_data struct
void clear_supports( std::vector<struct support_data>& );

void save_header( const struct header& , std::map<std::string,std::string>& );
void save_sam( const struct sam_fields& , const std::string& );
void write_files( struct pindel_fields& , std::map<std::string,std::string>& , std::map<std::string,int>& ); //writes the Pindel conversion to SAM

struct pindel_fields {
	std::string indelType;
	std::string indelSize;
	std::string NT_size;
	std::string NT_sequence;
	std::string chrID;
	std::string BPLeft_plus_one;
	std::string NumSupports;
	std::string NumSupSamples;
	std::vector<struct support_data> supports;
};

struct support_data {
	int leftOfIndel;
	std::string readSequence;
	std::string readBAMsource;
	std::string readBarcode;
};

struct sam_fields {
	std::string QNAME;
	std::string FLAG;
	std::string MAPQ;
	std::string RNAME;
	std::string POS;
	std::string CIGAR;
	std::string RNEXT;
	std::string PNEXT;
	std::string TLEN;
	std::string SEQ;
	std::string QUAL;
	std::string optional;
};

struct header {
	std::string top; //@HD\tVN:SAMVERSION
	std::string custom; //reference sequence info
	std::string bottom; //@PG\tID:Pindel\tVN:PINDELVERSION
	std::map<std::string,int> chrLen; //if need to track references within file
};

int main( int argc, char* argv[] )
{
/* TAKE INPUTS FROM COMMAND LINE: PINDEL DATA FILE, PINDEL CONFIG FILE */
//	handleInputs(argc,argv,first_line,last_line);

	std::string inputDirectoryName = argv[1];
	if ( inputDirectoryName[inputDirectoryName.length()] != '/' )
		inputDirectoryName += "/";
	outputDirectoryName = argv[2];
	if ( outputDirectoryName[outputDirectoryName.length()] != '/' )
		outputDirectoryName += "/";
	DIR *dirp = opendir( inputDirectoryName.c_str() );
	struct dirent *indir;

	std::string pindelFilename;
	std::ifstream fromPindel;

	struct pindel_fields PIN;
	struct header head;

	int nextprint;

	char dummy;
	std::string line, temp;
	int leftRefLength;

//	std::iostream error_value;
//	std::iostream error_log;

/* GET INFO FROM CONFIG FILE - file with primary output filename piece */
	std::string configFilename = argv[3];
	std::map<std::string,std::string> sampleMap;
	std::map<std::string,int> outputMap;
	int configIn = read_config_file( configFilename , sampleMap , outputMap );
	NUMBEROFSAMPLES = configIn;

/* GET HEADER INFO FROM REFERENCE INDEX FILE - file with SAM header sequence info */
	std::string referenceIndexFilename = argv[4];
	int referenceIn = read_fafai_file( referenceIndexFilename , head );
	save_header( head , sampleMap );

/* GET INFO FROM PINDEL DATA FILE */
	int pFnlen;
	while ( indir = readdir( dirp ) ) //directory position returns 0 when done
	{
		pindelFilename = (std::string)indir->d_name;
		pFnlen = pindelFilename.length()-1;
		if ( check_ending( pindelFilename ) ) //checks for _D & _SI
		{
			fromPindel.open( ( inputDirectoryName+pindelFilename ).c_str() );
			if ( fromPindel.good() && configIn && referenceIn ) //have a file to check
			{
				std::cout << "\t\tOpened: " << inputDirectoryName+pindelFilename << std::endl;

				linenum = 0;
				nextprint = UPDATEFREQUENCY;

				while ( fromPindel >> dummy )
				{
					temp = "";
					leftRefLength = 0;

					print_update( nextprint );

					// SUMMARY SEPARATION LINE
					check_separation( fromPindel , dummy ); //check # count

					// SUMMARY DATA LINE
					value = set_pindel_fields( fromPindel , PIN ); //set summary data

					if ( value == 0 ) //no errors from summary section
					{
						// REFERENCE LINE
						leftRefLength = set_reference_detail( fromPindel , PIN );

						// READ SUPPORTS
						set_supports( fromPindel , PIN , sampleMap , outputMap , leftRefLength );

						// WRITE TO FILE
						write_files( PIN , sampleMap , outputMap );
					}
					else
					{//Error in summary	
						std::cout << "PINDEL2SAM_ERROR: bad summary on line = " << linenum << std::endl;
						std::getline( fromPindel , line );
						linenum++;
					}//if reading supports
				}//while reading file
				fromPindel.close();
				std::cout << "\t\t\t\tClosed: " << inputDirectoryName+pindelFilename << std::endl;
			}//if file opened
			else
			{//Error opening file
				std::cout << "PINDEL2SAM_ERROR: could not open " << inputDirectoryName+pindelFilename << std::endl;
			}
		}//if _D or _SI
	}//while files available to read in
	int tempint = closedir( dirp );

	return 0;
}//main

/* FUNCTIONS */
int str2int( const std::string& str )
{
	return atoi( str.c_str() );
}

std::string int2str( const int& ent )
{
	std::stringstream ss;
	ss << ent;
	std::string str = ss.str();

	return str;
}

bool check_ending( const std::string filename )
{
	int fnlen = filename.length()-1;
	if ( ( filename[fnlen] == 'D' && filename[fnlen-1] == '_' ) || 
	     ( filename[fnlen] == 'I' && filename[fnlen-1] == 'S' && filename[fnlen-2] == '_' ) )
		return true;
	else
		return false;
}

void check_separation( std::ifstream& file , const char& dummy )
{
	std::string line;

	if ( dummy == '#' )	file >> line;
	if ( line.length() != NUMBEROFPOUNDS-1 )
	{
		std::cout << "PINDEL2SAM_ERROR: bad number of #'s = " << line.length() << std::endl;
		value = 1;
	}
	linenum++;
}

int read_config_file( const std::string& filename , std::map<std::string,std::string>& sampleMap , std::map<std::string,int>& outputMap )
{
	std::ifstream file( filename.c_str() );
	int numsamples = 0;

	if ( file.good() )//successfully opened config file
	{
		std::cout << "\t\tOpened: " << filename << std::endl;

		std::string name, sample, temp;
		while ( file >> name >> temp >> sample )
		{
			for ( int c = 0; c < name.length(); c++ )
			{
				if ( (char)name[c] == '/' )
					name[c] = '_';
			}
			sampleMap[sample] = name;
			outputMap[name] = numsamples++;
		}

		file.close();
		std::cout << "\t\t\t\tClosed: " << filename << std::endl;

		return numsamples;
	}
	else
	{//Error opening config file
		std::cout << "PINDEL2SAM_ERROR: could not open " << filename << std::endl;

		return numsamples;
	}
}

int read_fafai_file( const std::string& filename , struct header& h )
{
	std::ifstream file( filename.c_str() );
	std::string chr, chrlen, temp;
	int numrefs = 0;

	if ( file.good() )
	{
		std::cout << "\t\tOpened: " << filename << std::endl;

		h.top = "@HD\tVN:"+SAMVERSION+"\n";
		while ( file >> chr >> chrlen >> temp >> temp >> temp )
		{
				h.chrLen[chr] = str2int( chrlen );
				set_header_custom( h , chr );
			numrefs++;
		}
		set_header_bottom( h );

		file.close();
		std::cout << "\t\t\t\tClose: " << filename << std::endl;

		return numrefs;
	}
	else
	{//Error opening reference index file
		std::cout << "PINDEL2SAM_ERROR: could not open " << filename << std::endl;

		return numrefs;
	}
}

void set_header_custom( struct header& h , const std::string& chr )
{
	h.custom += "@SQ\tSN:"+chr+"\tLN:"+int2str( h.chrLen[chr] )+"\n";
}

void set_header_bottom( struct header& h )
{
	h.bottom = "@PG\tPN:Pindel\tVN:"+PINDELVERSION+"\n";
}

int set_pindel_fields( std::ifstream& file , struct pindel_fields& pid )
{
	std::string temp;

	pid.supports.clear();

	file >> temp; //SVIndex
	file >> pid.indelType >> pid.indelSize;
	file >> temp; //NT
	file >> pid.NT_size >> pid.NT_sequence;
	if ( pid.NT_sequence.length()-2 != str2int( pid.NT_size ) )
	{//Error NT sequence/size mismatch
		std::cout << "PINDEL2SAM_ERROR: NT sequence/size mismatch ( " << pid.NT_sequence.length() << " ";
		std::cout << pid.NT_size << " )\nSkipping support from line = " << linenum << std::endl;
		std::getline( file , temp );

		return 1;
	}
	file >> temp; //Chr
	file >> pid.chrID;
	file >> temp; //BP
	file >> pid.BPLeft_plus_one;
	file >> temp >> temp >> temp >> temp >> temp; //BPright BP_range left right NumSupports
	file >> pid.NumSupports;
	file >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> temp;
	file >> pid.NumSupSamples;
	if ( str2int( pid.NumSupSamples ) > NUMBEROFSAMPLES )
	{//Error number of samples mismatch
		std::cout << "PINDEL2SAM_ERROR: Number of samples mismatch\nSkipping supports from line = " << linenum << std::endl;
		std::getline( file , temp );

		return 2;
	}

	std::getline( file , temp ); //read rest of line
	linenum++;

	return 0;
	//check specific header sequences
}

int set_reference_detail( std::ifstream& file , struct pindel_fields& pid )
{
	std::string tempL, tempR;

	linenum++;
	if ( str2int( pid.NT_size ) > 0 ) //gap in reference
	{
		file >> tempL; //reads in left half
		std::getline( file , tempR ); //reads in right half

		return tempL.length();
	}
	else //no gap in reference
	{
		std::getline( file , tempL ); //no info to get, eat whole line

		return tempL.length();
	}
}

void field_conversion( struct pindel_fields& pid , int isup , struct sam_fields& sam )
{
	bool iscomplex = false;
	sam.QNAME = pid.supports[isup].readBarcode;
	sam.FLAG = "2";
	sam.RNAME = pid.chrID;
	sam.POS = determine_POS( pid.BPLeft_plus_one , pid.supports[isup].leftOfIndel );
	sam.MAPQ = "60"; //filler value
	sam.CIGAR = create_CIGAR( pid.indelType , pid.indelSize , pid.NT_size , pid.supports[isup].readSequence.length() , pid.supports[isup].leftOfIndel , iscomplex );
	sam.RNEXT = "*"; //"*" or "=" ("set as '=' if RNEXT is identical RNAME"...should be = for set between summary lines, right?)
	sam.PNEXT = "0"; //"0" or "This field equals POS at the primary line of the next read. If PNEXT is 0, no assumptions can be made on RNEXT and bit 0x20")
	sam.TLEN = "0"; //"0" for "single-segment template or when the information is unavailable." "If all segments are mapped to the same reference, the unsigned observed template length equals the number of bases from the leftmost mapped base to the rightmost mapped base."
	sam.SEQ = pid.supports[isup].readSequence; //pid.sequence minus white-space
	sam.QUAL = "*"; //"*" "ASCII of base QUALity plus 33...This field can be a '*' when quality is not stored. If not a '*', SEQ must not be a '*' and the length of the quality string ought to equal the length of SEQ."
	sam.optional = "PG:Z:Pindel"; 
	if ( iscomplex )	sam.optional += ",CI:Z:"+sam.CIGAR;
}

std::string create_CIGAR( std::string type , std::string size , std::string NTsize , int readLength , int readIndelLeftPos , bool& iscomplex )
{
	std::string cigar;
	std::string finalM;

	cigar = int2str( readIndelLeftPos ) + "M";
	if ( type[0] == 'D' && str2int( NTsize ) > 0 ) //complex indel
	{
		cigar += NTsize+"I"+size+"D";
		finalM = int2str( readLength - readIndelLeftPos - str2int( NTsize ) ); //total-left-insert = right
		cigar += finalM+"M";

		iscomplex = true;

		return cigar;
	}
	else
	{
		if ( str2int( NTsize ) > 0 ) //if insertion
		{
			cigar += NTsize + "I";
			finalM = int2str( readLength - readIndelLeftPos - str2int( NTsize ) );
		}
		if ( type[0] == 'D' ) //if deletion
		{
			cigar += size+type; //deletion size
			finalM = int2str( readLength - readIndelLeftPos );
		}

		return cigar += finalM+"M";
	}
}

std::string determine_POS( const std::string indelPos , const int leftof )
{
	return int2str( str2int( indelPos ) - leftof + 1 );
}

void set_support( std::ifstream& file , int Isize , struct support_data& support , std::map<std::string,std::string>& sm , std::map<std::string,int>& om , const int lrl )
{
	char dummy;
	int eat = 0;
	std::string line, temppm, tempn1, tempn2;
	std::string readLeft, readRight;
	std::map<std::string,int>::iterator omitlast = om.end();

	if ( Isize > 0 ) //read is continuous
	{
		file >> std::noskipws >> dummy;
		while ( isspace( dummy ) ) //need white space to get POS
		{
			file >> dummy;
			eat++;
		}
		support.leftOfIndel = lrl -eat;
		file >> std::skipws >> line;
		support.readSequence = dummy+line;
	}
	else //read has gap
	{
		file >> readLeft >> readRight;
		support.leftOfIndel = readLeft.length();
		support.readSequence = readLeft+readRight;
	}//if has gap

	file >> temppm >> tempn1 >> tempn2; //+- num num
	file >> support.readBAMsource;
	if ( om.find( sm[support.readBAMsource] ) == omitlast ) //find returns om.end() if key not found
	{//Error readBAMsource not in the map
		std::cout << "PINDEL2SAM_ERROR: readBAMsource not in the map: ";
		std::cout << support.readBAMsource << "\n\tbad line read as ";
		std::cout << support.readSequence << "\t" << temppm << "\t";
		std::cout << tempn1 << "\t" << tempn2 << "\t" << support.readBAMsource;
		std::cout << "\nSkipping support for this read from line = ";
		std::cout << linenum << std::endl;
		std::getline( file , line );

		value = 3;
	}
	else
	{
		file >> support.readBarcode;

		support.readBarcode.erase( 0 , 1 ); //removes @ from beginning
		support.readBarcode.erase( (size_t)(support.readBarcode.length()-2) , 2 ); //removes /1 or /2 from ending

		std::getline( file , line ); //finish line to advance

		value = 0;
	}
}

void set_supports( std::ifstream& file , struct pindel_fields& pid , std::map<std::string,std::string>& sm , std::map<std::string,int>& om , const int lrl )
{
	struct support_data sd;
	int eat;
	std::string line, readLeft, readRight;
	bool hasgap;

	for ( unsigned supportIndex = 0; supportIndex < str2int( pid.NumSupports ); supportIndex++ )
	{
		clear_support_data( sd ); //support data
		set_support( file , str2int( pid.NT_size ) , sd , sm , om , lrl );
		linenum++;
		if ( value == 0 ) //Support was read successfully
		{
			pid.supports.push_back( sd );
		}
		else 
		{//Error from set_suport skip to end of supports
			std::getline( file , line ); //could try another method of finding the next good line
			linenum++;
			supportIndex++;
		}
	}
}

void print_update( int& next )
{
	if ( linenum == 0 )
		std::cout << "\t\t\tConverting.\n";
	else if ( linenum >= next )
	{
		std::cout << "\t\t\tStill converting. At line " << linenum << "." << std::endl;
		next += UPDATEFREQUENCY;
	}
}

void print_header( const struct header& h )
{
	std::cout << h.top << h.custom << h.bottom;
}

void print_pindel_fields( const struct pindel_fields& pid )
{
	std::cout << pid.indelType << '\t' << pid.indelSize << '\t';
	std::cout << pid.NT_size << pid.NT_sequence << '\t';
	std::cout << pid.chrID << '\t' << pid.BPLeft_plus_one << '\t';
	std::cout << pid.NumSupports << '\t' << pid.NumSupSamples << std::endl;
	print_supports( pid.supports );
}

void print_support_with_summary( const struct pindel_fields& pid , int isup )
{
	std::cout << pid.indelType << '\t' << pid.indelSize << '\t';
	std::cout << pid.NT_size << pid.NT_sequence << '\t';
	std::cout << pid.chrID << '\t' << pid.BPLeft_plus_one << '\t';
	std::cout << pid.NumSupports << '\t' << pid.NumSupSamples << std::endl;
	print_support_data( pid.supports[isup] );
}

void print_support_data( const struct support_data& sd )
{
	std::cout << sd.leftOfIndel << '\t' << sd.readSequence << '\t';
	std::cout << sd.readBAMsource << '\t' << sd.readBarcode << '\n';
}

void print_supports( const std::vector<struct support_data>& supports )
{
	int i = 0;
	while ( i < supports.size() )
	{
		print_support_data( supports[i] );
		i++;
	}
}

void print_sam( const struct sam_fields& sam )
{
	std::cout << sam.QNAME << "\t" << sam.FLAG << "\t" << sam.RNAME << "\t";
	std::cout << sam.POS << "\t" << sam.MAPQ << "\t" << sam.CIGAR << "\t";
	std::cout << sam.RNEXT << "\t" << sam.PNEXT << "\t" << sam.TLEN << "\t";
	std::cout << sam.SEQ << "\t" << sam.QUAL << "\t" << sam.optional << std::endl;
}

void clear_support_data( struct support_data& sd )
{
	sd.leftOfIndel = 0;
	sd.readSequence = "";
	sd.readBAMsource = "";
	sd.readBarcode = "";
}

void save_header( const struct header& h , std::map<std::string,std::string>& sampleMap )
{
	std::string outname;
	std::fstream file;
	for ( std::map<std::string,std::string>::iterator sit = sampleMap.begin(); sit!=sampleMap.end(); ++sit )
	{
		outname = outputDirectoryName+( sit->second )+".sam";
		file.open( outname.c_str() );
		if ( file.good() ) //file existed
		{
			std::cout << "PINDEL2SAM_WARNING: File exists: " << outname;
			std::cout << "\n\tAssuming header present. Will append to existing files.\n";
			file.close();
		}
		else //file did not exist
		{
			file.open( outname.c_str() , std::ios::out );
			if ( file.is_open() ) //new file
			{
				std::cout << "\t\tInitializing output file: " << outname << std::endl;
				file << h.top << h.custom << h.bottom;
				file.close();
			}
			else //Error opening file
				std::cout << "PINDEL2SAM_ERROR: could not open " << outname << std::endl;
		}
	}//for each sample
}

void save_sam( const struct sam_fields& sam , const std::string& filename )
{
	std::fstream file;
	std::string outname = outputDirectoryName+filename+".sam";
	file.open( outname.c_str() , std::ios::out | std::ios::app );
	
	if ( file.good() ) //file exists and opened
	{
		file << sam.QNAME << "\t" << sam.FLAG << "\t" << sam.RNAME << "\t";
		file << sam.POS << "\t" << sam.MAPQ << "\t" << sam.CIGAR << "\t";
		file << sam.RNEXT << "\t" << sam.PNEXT << "\t" << sam.TLEN << "\t";
		file << sam.SEQ << "\t" << sam.QUAL << "\t" << sam.optional << std::endl;
		file.close();
	}
	else
	{//Error opening file
		std::cout << "PINDEL2SAM_ERROR: Could not write to " << outname << std::endl;
	}
}

void write_files( struct pindel_fields& pid , std::map<std::string,std::string>& sm , std::map<std::string,int>& om )
{
	struct support_data sd;
	std::vector<struct support_data> sds;
	std::map<std::string,int>::iterator omit = om.begin();
	struct sam_fields sam;
	
	for ( unsigned sampleIndex = 0; sampleIndex < str2int( pid.NumSupSamples ); sampleIndex++ )
	{
		for ( unsigned supportIndex = 0; supportIndex < str2int( pid.NumSupports ); supportIndex++ )
		{
			if ( om[sm[pid.supports[supportIndex].readBAMsource]] == omit->second )
			{
				field_conversion( pid , supportIndex , sam );
				if ( sam.CIGAR.length() > 0 )
					save_sam( sam , sm[pid.supports[supportIndex].readBAMsource] );
			}//if sample filename match
		}//for each support
		++omit; //advance through map
	}//for each output file
}
