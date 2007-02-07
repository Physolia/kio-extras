/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 * --- Well, it was hand-edited now, to fix compilation on other systems.... ---
 */

#include <rpc/types.h>
#include <rpc/xdr.h>
#include <arpa/inet.h>

#include <stdint.h>

/* David: added the #defines below to fix compilation on Mac OS 10.3.9, whose rpc/xdr.h has no int32 macros, only long, short etc. */

#ifndef IXDR_GET_INT32
#define IXDR_GET_INT32(buf)           ((int32_t)ntohl((uint32_t)*(buf)++))
#endif
#ifndef IXDR_GET_U_INT32
#define IXDR_GET_U_INT32(buf)         ((uint32_t)IXDR_GET_INT32(buf))
#endif
#ifndef IXDR_PUT_INT32
#define IXDR_PUT_INT32(buf, v)        (*(buf)++ = (int32_t)htonl((uint32_t)(v)))
#endif
#ifndef IXDR_PUT_U_INT32
#define IXDR_PUT_U_INT32(buf, v)      IXDR_PUT_INT32(buf, (int32_t)(v))
#endif

#include "mount.h"
/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user or with the express written consent of
 * Sun Microsystems, Inc.
 *
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */
/*
 * Copyright (c) 1985, 1990 by Sun Microsystems, Inc.
 */

/* from @(#)mount.x	1.3 91/03/11 TIRPC 1.0 */

bool_t
xdr_fhandle(XDR *xdrs, fhandle objp)
{

	 register int32_t *buf=buf;

	 if (!xdr_opaque(xdrs, objp, FHSIZE)) {
		 return (FALSE);
	 }
	return (TRUE);
}

bool_t
xdr_fhstatus(XDR *xdrs, fhstatus *objp)
{

	 register int32_t *buf=buf;

	 if (!xdr_u_int(xdrs, &objp->fhs_status)) {
		 return (FALSE);
	 }
	switch (objp->fhs_status) {
	case 0:
		 if (!xdr_fhandle(xdrs, objp->fhstatus_u.fhs_fhandle)) {
			 return (FALSE);
		 }
		break;
	default:
		break;
	}
	return (TRUE);
}

bool_t
xdr_dirpath(XDR *xdrs, dirpath *objp)
{

	 register int32_t *buf=buf;

	 if (!xdr_string(xdrs, objp, MNTPATHLEN)) {
		 return (FALSE);
	 }
	return (TRUE);
}

bool_t
xdr_name(XDR *xdrs, name *objp)
{

	 register int32_t *buf=buf;

	 if (!xdr_string(xdrs, objp, MNTNAMLEN)) {
		 return (FALSE);
	 }
	return (TRUE);
}

bool_t
xdr_mountlist(XDR *xdrs, mountlist *objp)
{

	 register int32_t *buf=buf;

	 if (!xdr_pointer(xdrs, (char **)objp, sizeof(struct mountbody), (xdrproc_t)xdr_mountbody)) {
		 return (FALSE);
	 }
	return (TRUE);
}

bool_t
xdr_mountbody(XDR *xdrs, mountbody *objp)
{

	 register int32_t *buf=buf;

	 if (!xdr_name(xdrs, &objp->ml_hostname)) {
		 return (FALSE);
	 }
	 if (!xdr_dirpath(xdrs, &objp->ml_directory)) {
		 return (FALSE);
	 }
	 if (!xdr_mountlist(xdrs, &objp->ml_next)) {
		 return (FALSE);
	 }
	return (TRUE);
}

bool_t
xdr_groups(XDR *xdrs, groups *objp)
{

	 register int32_t *buf=buf;

	 if (!xdr_pointer(xdrs, (char **)objp, sizeof(struct groupnode), (xdrproc_t)xdr_groupnode)) {
		 return (FALSE);
	 }
	return (TRUE);
}

bool_t
xdr_groupnode(XDR *xdrs, groupnode *objp)
{

	 register int32_t *buf=buf;

	 if (!xdr_name(xdrs, &objp->gr_name)) {
		 return (FALSE);
	 }
	 if (!xdr_groups(xdrs, &objp->gr_next)) {
		 return (FALSE);
	 }
	return (TRUE);
}

bool_t
xdr_exports(XDR *xdrs, exports *objp)
{

	 register int32_t *buf=buf;

	 if (!xdr_pointer(xdrs, (char **)objp, sizeof(struct exportnode), (xdrproc_t)xdr_exportnode)) {
		 return (FALSE);
	 }
	return (TRUE);
}

bool_t
xdr_exportnode(XDR *xdrs, exportnode *objp)
{

	 register int32_t *buf=buf;

	 if (!xdr_dirpath(xdrs, &objp->ex_dir)) {
		 return (FALSE);
	 }
	 if (!xdr_groups(xdrs, &objp->ex_groups)) {
		 return (FALSE);
	 }
	 if (!xdr_exports(xdrs, &objp->ex_next)) {
		 return (FALSE);
	 }
	return (TRUE);
}

bool_t
xdr_ppathcnf(XDR *xdrs, ppathcnf *objp)
{

	 register int32_t *buf=buf;

	 int i=i;

	 if (xdrs->x_op == XDR_ENCODE) {
	 buf = XDR_INLINE(xdrs, 6 * BYTES_PER_XDR_UNIT);
	   if (buf == NULL) {
		 if (!xdr_int(xdrs, &objp->pc_link_max)) {
			 return (FALSE);
		 }
		 if (!xdr_short(xdrs, &objp->pc_max_canon)) {
			 return (FALSE);
		 }
		 if (!xdr_short(xdrs, &objp->pc_max_input)) {
			 return (FALSE);
		 }
		 if (!xdr_short(xdrs, &objp->pc_name_max)) {
			 return (FALSE);
		 }
		 if (!xdr_short(xdrs, &objp->pc_path_max)) {
			 return (FALSE);
		 }
		 if (!xdr_short(xdrs, &objp->pc_pipe_buf)) {
			 return (FALSE);
		 }

	  }
	  else {
		 IXDR_PUT_U_INT32(buf,objp->pc_link_max);
		 IXDR_PUT_SHORT(buf,objp->pc_max_canon);
		 IXDR_PUT_SHORT(buf,objp->pc_max_input);
		 IXDR_PUT_SHORT(buf,objp->pc_name_max);
		 IXDR_PUT_SHORT(buf,objp->pc_path_max);
		 IXDR_PUT_SHORT(buf,objp->pc_pipe_buf);
	  }
	 if (!xdr_u_char(xdrs, &objp->pc_vdisable)) {
		 return (FALSE);
	 }
	 if (!xdr_char(xdrs, &objp->pc_xxx)) {
		 return (FALSE);
	 }
		buf = XDR_INLINE(xdrs,   2  * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
		 if (!xdr_vector(xdrs, (char *)objp->pc_mask, 2, sizeof(short), (xdrproc_t)xdr_short)) {
			 return (FALSE);
		 }

	  }
	  else {
		{ register short *genp; 
		  for ( i = 0,genp=objp->pc_mask;
 			i < 2; i++){
				 IXDR_PUT_SHORT(buf,*genp++);
		   }
		 };
	  }

 	 return (TRUE);
	} else if (xdrs->x_op == XDR_DECODE) {
	 buf = XDR_INLINE(xdrs,6 * BYTES_PER_XDR_UNIT);
	   if (buf == NULL) {
		 if (!xdr_int(xdrs, &objp->pc_link_max)) {
			 return (FALSE);
		 }
		 if (!xdr_short(xdrs, &objp->pc_max_canon)) {
			 return (FALSE);
		 }
		 if (!xdr_short(xdrs, &objp->pc_max_input)) {
			 return (FALSE);
		 }
		 if (!xdr_short(xdrs, &objp->pc_name_max)) {
			 return (FALSE);
		 }
		 if (!xdr_short(xdrs, &objp->pc_path_max)) {
			 return (FALSE);
		 }
		 if (!xdr_short(xdrs, &objp->pc_pipe_buf)) {
			 return (FALSE);
		 }

	  }
	  else {
		 objp->pc_link_max = IXDR_GET_U_INT32(buf);
		 objp->pc_max_canon = IXDR_GET_SHORT(buf);
		 objp->pc_max_input = IXDR_GET_SHORT(buf);
		 objp->pc_name_max = IXDR_GET_SHORT(buf);
		 objp->pc_path_max = IXDR_GET_SHORT(buf);
		 objp->pc_pipe_buf = IXDR_GET_SHORT(buf);
	  }
	 if (!xdr_u_char(xdrs, &objp->pc_vdisable)) {
		 return (FALSE);
	 }
	 if (!xdr_char(xdrs, &objp->pc_xxx)) {
		 return (FALSE);
	 }
		buf = XDR_INLINE(xdrs, 2  * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
		 if (!xdr_vector(xdrs, (char *)objp->pc_mask, 2, sizeof(short), (xdrproc_t)xdr_short)) {
			 return (FALSE);
		 }

	  }
	  else {
		{ register short *genp; 
		  for ( i = 0,genp=objp->pc_mask;
 			i < 2; i++){
				 *genp++ = IXDR_GET_SHORT(buf);
		   }
		 };
	  }
	 return(TRUE);
	}

	 if (!xdr_int(xdrs, &objp->pc_link_max)) {
		 return (FALSE);
	 }
	 if (!xdr_short(xdrs, &objp->pc_max_canon)) {
		 return (FALSE);
	 }
	 if (!xdr_short(xdrs, &objp->pc_max_input)) {
		 return (FALSE);
	 }
	 if (!xdr_short(xdrs, &objp->pc_name_max)) {
		 return (FALSE);
	 }
	 if (!xdr_short(xdrs, &objp->pc_path_max)) {
		 return (FALSE);
	 }
	 if (!xdr_short(xdrs, &objp->pc_pipe_buf)) {
		 return (FALSE);
	 }
	 if (!xdr_u_char(xdrs, &objp->pc_vdisable)) {
		 return (FALSE);
	 }
	 if (!xdr_char(xdrs, &objp->pc_xxx)) {
		 return (FALSE);
	 }
	 if (!xdr_vector(xdrs, (char *)objp->pc_mask, 2, sizeof(short), (xdrproc_t)xdr_short)) {
		 return (FALSE);
	 }
	return (TRUE);
}
