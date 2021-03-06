/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: 
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

#include "outputformat.h"
#include <QDebug>

/** OutputFormat related */

OutputFormat::OutputFormat(const QString & name, const QString &mimeType, const QString & driver,
                const QString & extension, const int  & imageMode, const bool & transparent,
                const enum State & state) :
name(name), originalName(name), mimeType(mimeType), extension(extension),
imageMode(imageMode), transparent(transparent), state(state)
{
  setDriver(driver);
}

/** copy constructor */
OutputFormat::OutputFormat(const OutputFormat & of) {
  name          = of.getName();
  originalName  = of.getOriginalName();
  mimeType      = of.getMimeType();
  extension     = of.getExtension();
  imageMode     = of.getImageMode();
  transparent   = of.getTransparent();
  state         = of.getState();
  driver        = of.getDriver();
  gdalDriver    = of.getGdalDriver();
  formatOptions = QHash<QString,QString>(of.getFormatOptions());
}

/** Getters */
QString const & OutputFormat::getName()         const { return name;         }
QString const & OutputFormat::getOriginalName() const { return originalName; }
QString const & OutputFormat::getMimeType()     const { return mimeType;     }

QString const & OutputFormat::getDriver()     const { return driver;     }
QString const & OutputFormat::getGdalDriver() const { return gdalDriver; }

QString const & OutputFormat::getExtension()   const { return extension;   }
int     const & OutputFormat::getImageMode()   const { return imageMode;   }
bool    const & OutputFormat::getTransparent() const { return transparent; }

enum OutputFormat::State const & OutputFormat::getState()    const { return state;       }

QHash<QString, QString> const & OutputFormat::getFormatOptions() const { return formatOptions; }

bool OutputFormat::isEmpty() { return name.isEmpty(); };

void OutputFormat::addFormatOption(const QString &k, const QString &v) {
   formatOptions.insert(k,v);
}

void OutputFormat::removeFormatOption(const QString &k) {
  formatOptions.remove(k);
}

void OutputFormat::setFormatOptions(QHash<QString,QString> const &newFmtOpts) {
  formatOptions = newFmtOpts;
}

void OutputFormat::setName(QString const &v)      { name        = v; }
void OutputFormat::setMimeType(QString const &v)  { mimeType    = v; }

void OutputFormat::setDriver(QString const &driver)    {
  this->gdalDriver = "";
  if (driver.startsWith("GDAL") || driver.startsWith("OGR"))  {
    QStringList split = driver.split("/");
    this->driver = split.at(0);
    // expected case
    if (split.size() == 2)
      this->gdalDriver = split.at(1);
  }
  else
    this->driver = driver;
}

void OutputFormat::setExtension(QString const &v) { extension   = v; }
void OutputFormat::setImageMode(int const &v)     { imageMode   = v; }
void OutputFormat::setTransparent(bool const &v)  { transparent = v; }
void OutputFormat::setState(enum State const &v)  { state       = v; }

/** OutputFormat Model (representation into the UI) */

OutputFormatsModel::OutputFormatsModel(QObject * parent) : QAbstractListModel(parent) {}

OutputFormatsModel::~OutputFormatsModel() {
}

int OutputFormatsModel::rowCount(const QModelIndex & parent) const {
  Q_UNUSED(parent);
  return entries.size();
}

int OutputFormatsModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return OutputFormatsModel::Transparent + 1;
}

QVariant OutputFormatsModel::headerData (int section, Qt::Orientation orientation, int role) const {
  Q_UNUSED(section);
  Q_UNUSED(orientation);
  if (role != Qt::DisplayRole)
    return QVariant();
  return QVariant(QObject::tr("Output formats"));
}

void OutputFormatsModel::setEntries(QList<OutputFormat *> const & items) {
  beginResetModel();
  entries = items;
  endResetModel();
}

QList<OutputFormat *> const & OutputFormatsModel::getEntries(void) const {
  return entries;
}

QList<OutputFormat *> const & OutputFormatsModel::getRemovedEntries(void) const {
  return removedEntries;
}

OutputFormat * OutputFormatsModel::getOutputFormat(const QModelIndex &index) const {
  if ((index.row() < 0) || index.row() > entries.size())
    return NULL;
  return entries.at(index.row());
}

QVariant OutputFormatsModel::data(const QModelIndex &index, int role) const {
  if ((role != Qt::DisplayRole) && (role != Qt::EditRole))
    return QVariant();
  if (index.row() > entries.size())
    return QVariant();

  OutputFormat * of = entries.at(index.row());

  if (of == NULL)
    return QVariant();

  switch (index.column()) {
    case OutputFormatsModel::Name:
      return QVariant(of->getName());
    case OutputFormatsModel::MimeType:
      return QVariant(of->getMimeType());
    case OutputFormatsModel::Driver:
      return QVariant(of->getDriver());
    case OutputFormatsModel::GdalDriver:
      return QVariant(of->getGdalDriver());
    case OutputFormatsModel::Extension:
      return QVariant(of->getExtension());
    case OutputFormatsModel::ImageMode:
      return QVariant(of->getImageMode());
    case OutputFormatsModel::Transparent:
      return QVariant(of->getTransparent());
  }
  return QVariant();
}

bool OutputFormatsModel::setData(const QModelIndex & index, const QVariant & value, int role) {

  // In fact, we cannot use the current method, because we have to consider the
  // case when the user will do some modifications, then click on abort.  In
  // this case, we would have to undo all the modifications made onto the
  // selected outputformat. We will handle the call to the different setters
  // into the event triggered when the user will click on "save" (on output
  // format tab).

  Q_UNUSED(index)
  Q_UNUSED(value)
  Q_UNUSED(role)

  return false;
}

bool OutputFormatsModel::nameAlreadyIn(const QString & key) {
   for (int i = 0; i < entries.size();  ++i) {
     if (entries.at(i)->getName() == key)
      return true;
   }
   return false;
}

void OutputFormatsModel::removeOutputFormat(const QModelIndex & index) {
  if (index.row() < 0 || index.row() > entries.size())
    return;
  beginResetModel();
  OutputFormat *toBeRemoved = entries.takeAt(index.row());
  // if the format has only an existence into the application
  // no need to mark it as to be removed later in the mapfile.
  if ((toBeRemoved->getState() == OutputFormat::ADDED)
      ||(toBeRemoved->getState() == OutputFormat::ADDED_SAVED)) {
    delete toBeRemoved;
  } else {
    removedEntries.append(toBeRemoved);
  }
  endResetModel();
}

