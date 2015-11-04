/**********************************************************************
*
* Copyright (c) 2012-2014 Barbara Geller
* Copyright (c) 2006-2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*
* * Redistributions of source code must retain the above copyright notice,
*   this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the Nokia Corporation nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
***********************************************************************/

#include "draglabel.h"
#include "fridgemag.h"

#include <QtGui>

static const QString qmPath = ":/resources";

FridgeMagnet::FridgeMagnet(QWidget *parent)
   : QWidget(parent)
{
   setWindowTitle(tr("Fridge Magnets"));  
   setAcceptDrops(true);

   //
   QString fileName;
   fileName = qmPath + "/words.txt";

   QFile dictionaryFile(fileName);
   dictionaryFile.open(QFile::ReadOnly);

   QTextStream inputStream(&dictionaryFile);

   int x = 5;
   int y = 5;

   while (! inputStream.atEnd()) {
      QString word;
      inputStream >> word;

      if (!word.isEmpty()) {
         DragLabel *wordLabel = new DragLabel(word, this);
         wordLabel->move(x, y);
         wordLabel->show();
         wordLabel->setAttribute(Qt::WA_DeleteOnClose);
         x += wordLabel->width() + 2;

         if (x >= 245) {
            x = 5;
            y += wordLabel->height() + 2;
         }
      }
   }

}

void FridgeMagnet::dragEnterEvent(QDragEnterEvent *event)
{
   if (event->mimeData()->hasFormat("application/x-fridgemagnet")) {
      if (children().contains(event->source())) {
         event->setDropAction(Qt::MoveAction);
         event->accept();

      } else {
         event->acceptProposedAction();
      }

   } else if (event->mimeData()->hasText()) {
      event->acceptProposedAction();

   } else {
      event->ignore();
   }
}

void FridgeMagnet::dragMoveEvent(QDragMoveEvent *event)
{
   if (event->mimeData()->hasFormat("application/x-fridgemagnet")) {
      if (children().contains(event->source())) {
         event->setDropAction(Qt::MoveAction);
         event->accept();
      } else {
         event->acceptProposedAction();
      }

   } else if (event->mimeData()->hasText()) {
      event->acceptProposedAction();

   } else {
      event->ignore();
   }
}

void FridgeMagnet::dropEvent(QDropEvent *event)
{
   if (event->mimeData()->hasFormat("application/x-fridgemagnet")) {
      const QMimeData *mime = event->mimeData();

      QByteArray itemData = mime->data("application/x-fridgemagnet");
      QDataStream dataStream(&itemData, QIODevice::ReadOnly);

      QString text;
      QPoint offset;
      dataStream >> text >> offset;

      DragLabel *newLabel = new DragLabel(text, this);
      newLabel->move(event->pos() - offset);
      newLabel->show();
      newLabel->setAttribute(Qt::WA_DeleteOnClose);

      if (event->source() == this) {
         event->setDropAction(Qt::MoveAction);
         event->accept();

      } else {
         event->acceptProposedAction();

      }

   } else if (event->mimeData()->hasText()) {
      QStringList pieces = event->mimeData()->text().split(QRegExp("\\s+"), QString::SkipEmptyParts);
      QPoint position = event->pos();

      foreach (QString piece, pieces) {
         DragLabel *newLabel = new DragLabel(piece, this);
         newLabel->move(position);
         newLabel->show();
         newLabel->setAttribute(Qt::WA_DeleteOnClose);

         position += QPoint(newLabel->width(), 0);
      }

      event->acceptProposedAction();

   } else {
      event->ignore();

   }
}

void FridgeMagnet::mousePressEvent(QMouseEvent *event)
{
   DragLabel *child = dynamic_cast<DragLabel*>(childAt(event->pos()));

   if (! child) {
      return;
   }

   QPoint hotSpot = event->pos() - child->pos();

   QByteArray itemData;
   QDataStream dataStream(&itemData, QIODevice::WriteOnly);
   dataStream << child->labelText() << QPoint(hotSpot);

   QMimeData *mimeData = new QMimeData;
   mimeData->setData("application/x-fridgemagnet", itemData);
   mimeData->setText(child->labelText());

   QDrag *drag = new QDrag(this);
   drag->setMimeData(mimeData);
   drag->setPixmap(*child->pixmap());
   drag->setHotSpot(hotSpot);

   child->hide();

   if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)  {
      child->close();

   } else {
      child->show();

   }
}

QSize FridgeMagnet::sizeHint() const
{
   return QSize(400,400);
}
