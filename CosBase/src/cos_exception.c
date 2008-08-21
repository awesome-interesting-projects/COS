/*
 o---------------------------------------------------------------------o
 |
 | COS exception
 |
 o---------------------------------------------------------------------o
 |
 | C Object System
 |
 | Copyright (c) 2006+ Laurent Deniau, laurent.deniau@cern.ch
 |
 | For more information, see:
 | http://cern.ch/laurent.deniau/cos.html
 |
 o---------------------------------------------------------------------o
 |
 | This file is part of the C Object System framework.
 |
 | The C Object System is free software; you can redistribute it and/or
 | modify it under the terms of the GNU Lesser General Public License
 | as published by the Free Software Foundation; either version 3 of
 | the License, or (at your option) any later version.
 |
 | The C Object System is distributed in the hope that it will be
 | useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 | of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 |
 | See <http://www.gnu.org/licenses> for more details.
 |
 o---------------------------------------------------------------------o
 |
 | $Id: cos_exception.c,v 1.1 2008/06/27 16:17:18 ldeniau Exp $
 |
*/

#include <cos/Object.h>
#include <cos/Exception.h>
#include <cos/gen/object.h>

#include <stdlib.h>

static struct cos_exception_context cxt0;
__thread struct cos_exception_context *cos_exception_cxt = &cxt0;

static void
unwind_stack(struct cos_exception_context *cxt)
{
  struct cos_exception_protect *p = cxt->stk;

  cxt->unstk = YES;

  while (p) {
    if (*p->obj) grelease(*p->obj);
    p = p->nxt;
  }

  cxt->unstk = NO;
  cxt->stk   = 0;
}

static void
verbose_terminate(OBJ ex, STR file, int line)
{
  if (cos_exception_uncaught() == YES)
    cos_abort("exception %s thrown at (%s:%d) during stack unwinding "
              "leading to an undefined behavior", gclassName(ex), file, line);
  else
    cos_trace("exiting with uncaught exception %s thrown at (%s:%d)",
              gclassName(ex), file, line);
}

static cos_exception_handler handler = verbose_terminate;

static void
terminate(void)
{
  if (handler)
    handler(cos_exception_cxt->ex,
            cos_exception_cxt->file,
            cos_exception_cxt->line);

  exit(EXIT_FAILURE);
}

// ----- low-level API

BOOL
cos_exception_catch(OBJ ex, OBJ cls)
{
  return gisKindOf(ex,cls) == True;
}

void
cos_exception_throwLoc(OBJ ex, STR file, int line)
{
  gthrow(ex,file,line);
  terminate(); // safer if an overridden gThrow returns.
}

BOOL
cos_exception_uncaught(void)
{
  return cos_exception_cxt->unstk;
}

// -----

cos_exception_handler
cos_exception_setTerminate(cos_exception_handler new)
{
  cos_exception_handler old = handler;
  handler = new;
  return old;
}

// ----- methods

defmethod(void, gthrow, Any, (STR)file, (int)line)

  struct cos_exception_context *cxt = cos_exception_cxt;

  cxt->ex   = self->rc == COS_RC_AUTO ? gclone(_1) : _1;
  cxt->file = file;
  cxt->line = line;

  if (cos_exception_uncaught() == YES)
    terminate();

  unwind_stack(cxt);

  if (cxt == &cxt0)
    terminate();

  cos_exception_lngjmp(cxt->buf, cxt->st | cos_exception_throw_st);

endmethod

/*
 * ----------------------------------------------------------------------------
 *  Debug Functions
 * ----------------------------------------------------------------------------
 */

#include <cos/cos/debug.h>

void
cos_exception_showStack(FILE *fp)
{
  struct cos_exception_protect *p = cos_exception_cxt->stk;
  U32 i = 0;

  if (!fp) fp = stderr;

  while (p) {
    fprintf(fp, "prt[%4u] = %-25s\n",
            i, *p->obj ? gclassName(*p->obj) : "NIL");
    ++i, p = p->nxt;
  }
}