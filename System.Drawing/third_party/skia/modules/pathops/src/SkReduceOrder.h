/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef SkReduceOrder_DEFINED
#define SkReduceOrder_DEFINED

#include "include/core/SkPath.h"
#include "modules/pathops/src/SkPathOpsCubic.h"
#include "modules/pathops/src/SkPathOpsLine.h"
#include "modules/pathops/src/SkPathOpsQuad.h"

struct SkConic;
struct SkPoint;

union SkReduceOrder {
    enum Quadratics {
        kNo_Quadratics,
        kAllow_Quadratics
    };

    int reduce(const SkDCubic& cubic, Quadratics);
    int reduce(const SkDLine& line);
    int reduce(const SkDQuad& quad);

    static SkPath::Verb Conic(const SkConic& conic, SkPoint* reducePts);
    static SkPath::Verb Cubic(const SkPoint pts[4], SkPoint* reducePts);
    static SkPath::Verb Quad(const SkPoint pts[3], SkPoint* reducePts);

    SkDLine fLine;
    SkDQuad fQuad;
    SkDCubic fCubic;
};

#endif
