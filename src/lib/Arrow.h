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

#ifndef __ARROW_H__
#define __ARROW_H__

namespace libmspub
{
enum ArrowStyle
{
  NO_ARROW,
  TRIANGLE_ARROW,
  STEALTH_ANGLE_ARROW,
  ROTATED_SQUARE_ARROW,
  CIRCLE_ARROW,
  LINE_ARROW
};
enum ArrowSize
{
  SMALL,
  MEDIUM,
  LARGE
};
struct Arrow
{
  ArrowStyle m_style;
  ArrowSize m_width;
  ArrowSize m_height;
  Arrow(ArrowStyle style, ArrowSize width, ArrowSize height) :
    m_style(style), m_width(width), m_height(height)
  {
  }
};
} // namespace libmspub

#endif /* __ARROW_H__ */
/* vim:set shiftwidth=2 softtabstop=2 expandtab: */