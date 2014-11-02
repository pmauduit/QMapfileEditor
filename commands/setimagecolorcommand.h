/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose:.
 * Author: Pierre Mauduit
 *
 **********************************************************************
 * Copyright (c) 2014, Pierre Mauduit
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 ****************************************************************************/

#ifndef SETIMAGECOLORCOMMAND_H
#define SETIMAGECOLORCOMMAND_H

#include <QColor>
#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class SetImageColorCommand : public QUndoCommand {

 public:
   SetImageColorCommand(QColor color, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   QColor oldColor, newColor;
   MapfileParser * parser;
};


#endif // SETIMAGECOLORCOMMAND_H


