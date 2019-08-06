/***************************************************************************
 *   Copyright 2019 CNRS and University of Strasbourg                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02111-1301, USA.         *
 ***************************************************************************/

/*
  returns the length of the longest value in a set
  input parameters:
  data (string)
  output:
  length of longest value (int)
  registering the function:
  CREATE AGGREGATE FUNCTION tanimoto RETURNS INTEGER SONAME 'udf_tanimoto.so';
  getting rid of the function:
  DROP FUNCTION tanimoto;
*/

#ifdef STANDARD
#include <stdio.h>
#include <string.h>
#ifdef __WIN__
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif // __WIN__
#else
#include <my_global.h>
#include <my_sys.h>
#endif // STANDARD
#include <mysql.h>
#include <m_ctype.h>
#include <m_string.h>

#ifdef HAVE_DLOPEN

extern "C" {

  my_bool tanimoto_init(UDF_INIT* initid, UDF_ARGS* args, char* message);
  void tanimoto_deinit(UDF_INIT* initid);
  double tanimoto(UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error);

  my_bool tanimoto_init( UDF_INIT* initid, UDF_ARGS* args, char* message ) {
    if (args->arg_count != 2) {
      strcpy(message, "wrong number of arguments: tanimoto() requires exactly two arguments");
      return 1;
    }

    if ( (args->arg_type[0] != STRING_RESULT) || (args->arg_type[1] != STRING_RESULT) ) {
      strcpy(message, "wrong argument type: tanimoto() requires only string as parameters");
      return 1;
    }

    return 0;
  }

  void tanimoto_deinit( UDF_INIT* initid ) {
  }

  double tanimoto( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error ) {
    int *intptr1 = NULL;
    int *intptr2 = NULL;
    unsigned long int *ulintptr1 = NULL;
    unsigned long int *ulintptr2 = NULL;
    unsigned long int fpLength1 = 0;
    unsigned long int fpLength2 = 0;
    int andfp = 0, orfp = 0;
    unsigned long int andbits = 0, orbits = 0;
    unsigned long int i = 0;

    if ((args->args[0] == NULL) || (args->args[1] == NULL)) {
      /* Arguments can not be NULL */
      *is_null = 1;
      *error = 1;

      return 0;
    }  *error = 0;
    ulintptr1 = (unsigned long int *) args->args[0];
    fpLength1 = ulintptr1[0];
    ulintptr2 = (unsigned long int *) args->args[1];
    fpLength2 = ulintptr2[0];

    if (fpLength1 != fpLength2) {
      /* The both fingerprint must have the same length) */
      *is_null = 1;
      *error = 1;

      return 0;
    }

    if ((fpLength1 % sizeof(int)) || (fpLength2 % sizeof(int))) {
      /* The length of the fingerprint must be a multiple of long int */
      *is_null = 1;
      *error = 1;

      return 0;
    }

    ++ulintptr1;
    ++ulintptr2;
    intptr1 = (int*) ulintptr1;
    intptr2 = (int*) ulintptr2;
    /**
     * Tanimoto coefficient is defined by:
     * Number of bits set in (patternFP & targetFP) /
     * Number of bits in (patternFP | targetFP)
     */
    for(i = 0; i < fpLength1/sizeof(int); ++i) {
      andfp = intptr1[i] & intptr2[i];
      orfp = intptr1[i] | intptr2[i];
      for (; andfp; andfp = andfp<<1) {
        if (andfp < 0) { ++andbits; }
      }
      for (; orfp; orfp = orfp<<1) {
        if (orfp < 0) { ++orbits; }
      }
    }

    if (orbits == 0) {
      return 0.0;
    }

    return (double) andbits / (double) orbits;
  }
}
#endif // HAVE_DLOPEN
