/*
 *  Copyright (c) 2008-2010 Lukáš Tvrdý <lukast.dev@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KIS_SUMI_SHAPE_OPTION_H
#define KIS_SUMI_SHAPE_OPTION_H

#include <kis_paintop_option.h>
#include <krita_export.h>

const QString SUMI_RADIUS = "Sumi/radius";
const QString SUMI_SIGMA = "Sumi/sigma";
const QString SUMI_IS_DIMENSION_1D = "Sumi/isDimension1D";

class KisShapeOptionsWidget;

class KisSumiShapeOption : public KisPaintOpOption
{
public:
    KisSumiShapeOption();
    ~KisSumiShapeOption();

    void setRadius(int radius) const;
    void setScaleFactor(qreal scale) const;
    
    int radius() const;
    double sigma() const;
    bool isbrushDimension1D() const;
    bool useMousePressure() const;

    void writeOptionSetting(KisPropertiesConfiguration* config) const;
    void readOptionSetting(const KisPropertiesConfiguration* config);
private:
    KisShapeOptionsWidget * m_options;
};

#endif // KIS_SUMI_SHAPE_OPTION_H

