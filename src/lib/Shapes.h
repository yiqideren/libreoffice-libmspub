/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* libmspub
 * Version: MPL 1.1 / GPLv2+ / LGPLv2+
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License or as specified alternatively below. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * Major Contributor(s):
 * Copyright (C) 2012 Brennan Vincent <brennanv@email.arizona.edu>
 *
 * All Rights Reserved.
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPLv2+"), or
 * the GNU Lesser General Public License Version 2 or later (the "LGPLv2+"),
 * in which case the provisions of the GPLv2+ or the LGPLv2+ are applicable
 * instead of those above.
 */
#ifndef __SHAPES_H__
#define __SHAPES_H__

#include <vector>
#include <map>

namespace libmspub
{
class MSPUBCollector;
struct CustomShape;
struct Coordinate
{
  Coordinate(int xs, int ys, int xe, int ye) : m_xs(xs), m_ys(ys), m_xe(xe), m_ye(ye) { }
  Coordinate() : m_xs(0), m_ys(0), m_xe(0), m_ye(0) { }
  int m_xs, m_ys, m_xe, m_ye;
};
struct Shape
{
  Shape(MSPUBCollector *o) : props(), graphicsProps(), owner(o) { }
  virtual void output(libwpg::WPGPaintInterface *painter, Coordinate coord);
  virtual ~Shape()
  {
  }
  WPXPropertyList props;
  WPXPropertyList graphicsProps;
protected:
  virtual void setCoordProps(Coordinate coord);
  virtual void write(libwpg::WPGPaintInterface *painter) = 0;
  MSPUBCollector *owner;

  virtual WPXPropertyListVector updateGraphicsProps();

  Shape() : props(), graphicsProps(), owner(NULL) { }
private:
  Shape(const Shape &) : props(), graphicsProps(), owner(NULL) { }
  Shape &operator=(const Shape &)
  {
    return *this;
  }
};
struct FillableShape : public Shape
{
  FillableShape(MSPUBCollector *o) : Shape(o), m_fill(NULL) { }
  Fill *m_fill;
  void setFill(Fill *fill);
protected:
  virtual WPXPropertyListVector updateGraphicsProps();
private:
  FillableShape(const FillableShape &) : Shape(NULL), m_fill(NULL) { }
  FillableShape &operator=(const FillableShape &)
  {
    return *this;
  }
};
struct TextShape : public FillableShape
{
  TextShape(std::vector<TextParagraph> s, MSPUBCollector *o) : FillableShape(o), str(s) { }
  std::vector<TextParagraph> str;
protected:
  void write(libwpg::WPGPaintInterface *painter);
  WPXPropertyListVector updateGraphicsProps();
private:
  TextShape(const TextShape &) : FillableShape(NULL), str() { }
  TextShape &operator=(const TextShape &)
  {
    return *this;
  }
};
struct GeometricShape : public FillableShape
{
  void setLine(ColorReference line);
  void fillDefaultAdjustValues();
  void setAdjustValue(unsigned index, int adjustValue);
  void setClockwiseRotation(short rotation);
  void setText(std::vector<TextParagraph> str);
  double getCalculationValue(unsigned index, bool recursiveEntry = false) const;
  double getSpecialValue(const CustomShape &shape, int arg) const;

  std::vector<TextParagraph> m_str;
  bool m_hasText;
  unsigned m_pageSeqNum;
  unsigned m_imgIndex;
  ShapeType m_type;
  ColorReference m_line;
  bool m_lineSet;
  double m_x, m_y, m_width, m_height;
  std::vector<int> m_adjustValues;
  short m_clockwiseRotation;
  GeometricShape(unsigned psn, MSPUBCollector *o)
    : FillableShape(o), m_str(), m_hasText(false), m_pageSeqNum(psn), m_imgIndex(0), m_type(RECTANGLE),
      m_line(0x08000000), m_lineSet(false), m_x(0), m_y(0), m_width(0), m_height(0), m_adjustValues(),
      m_clockwiseRotation(0), m_valuesSeen(), m_filledDefaultAdjustValues(false), m_textCoord(), m_closeEverything(false) { }
  void output(libwpg::WPGPaintInterface *painter, Coordinate coord);
protected:
  void setCoordProps(Coordinate coord);
  virtual void write(libwpg::WPGPaintInterface *painter);
  WPXPropertyListVector updateGraphicsProps();
  GeometricShape()
    : FillableShape(NULL), m_str(), m_hasText(false), m_pageSeqNum(0), m_imgIndex(0), m_type(RECTANGLE),
      m_line(0x08000000), m_lineSet(false), m_x(0), m_y(0), m_width(0), m_height(0), m_adjustValues(),
      m_clockwiseRotation(0), m_valuesSeen(), m_filledDefaultAdjustValues(false), m_textCoord(), m_closeEverything(false) { }
private:
  // This copy constructor is exactly like the default one. Does it make sense to have it?
  GeometricShape(const GeometricShape &)
    : FillableShape(NULL), m_str(), m_hasText(false), m_pageSeqNum(0), m_imgIndex(0), m_type(RECTANGLE),
      m_line(0x08000000), m_lineSet(false), m_x(0), m_y(0), m_width(0), m_height(0), m_adjustValues(),
      m_clockwiseRotation(0), m_valuesSeen(), m_filledDefaultAdjustValues(false), m_textCoord(), m_closeEverything(false) { }
  GeometricShape &operator=(const GeometricShape &)
  {
    return *this;
  }
  mutable std::vector<bool> m_valuesSeen;
  bool m_filledDefaultAdjustValues;
  Coordinate m_textCoord;
  bool m_closeEverything;
};
struct ImgShape : public GeometricShape
{
  ImgShape(const GeometricShape &from, ImgType imgType, WPXBinaryData i, MSPUBCollector *o);
  ImgShape(ImgType type, WPXBinaryData i, WPXPropertyList /* p */, unsigned psn, MSPUBCollector *o) : GeometricShape(psn, o), img(i)
  {
    setMime_(type);
  }
  WPXBinaryData img;
  static const char *mimeByImgType(ImgType type);
protected:
  virtual void write(libwpg::WPGPaintInterface *painter);
private:
  void setMime_(ImgType type);
  ImgShape(const ImgShape &) : GeometricShape(), img() { }
  ImgShape &operator=(const ImgShape &)
  {
    return *this;
  }
};
} // namespace libmspub
#endif // __SHAPES_H__
/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
