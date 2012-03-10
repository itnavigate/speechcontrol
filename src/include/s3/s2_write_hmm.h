/* ====================================================================
 * Copyright (c) 1994-2000 Carnegie Mellon University.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * This work was supported in part by funding from the Defense Advanced
 * Research Projects Agency and the National Science Foundation of the
 * United States of America, and the CMU Sphinx Speech Consortium.
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */
/*********************************************************************
 *
 * File: s2_write_hmm.h
 *
 * Description:
 *
 * Author:
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef S2_WRITE_HMM_H
#define S2_WRITE_HMM_H
#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


#include <s3/acmod_set.h>
#include <s3/model_def.h>
#include <sphinxbase/prim_type.h>

int
s2_write_hmm ( float32 ***tmat,
               acmod_set_t *phone_set,
               const char *out_dir_name );

int
s2_write_dhmm ( float32 ***tmat,
                float32 ***mixw,
                model_def_t *mdef,
                const char  *out_dir_name );

#ifdef __cplusplus
}
#endif
#endif /* S2_WRITE_HMM_H */


/*
 * Log record.  Maintained by RCS.
 *
 * $Log$
 * Revision 1.4  2004/07/21  17:46:10  egouvea
 * Changed the license terms to make it the same as sphinx2 and sphinx3.
 *
 * Revision 1.3  2001/04/05 20:02:30  awb
 * *** empty log message ***
 *
 * Revision 1.2  2000/09/29 22:35:12  awb
 * *** empty log message ***
 *
 * Revision 1.1  2000/09/24 21:38:30  awb
 * *** empty log message ***
 *
 * Revision 1.4  97/07/16  11:39:10  eht
 * *** empty log message ***
 *
 * Revision 1.3  95/10/10  13:10:34  eht
 * Changed to use <sphinxbase/prim_type.h>
 *
 *
 */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
