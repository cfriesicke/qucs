/*
 * vpm.cpp - PM modulated AC voltage source class implementation
 *
 * Copyright (C) 2006 Stefan Jahn <stefan@lkcc.org>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this package; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA 02110-1301, USA.  
 *
 * $Id: vpm.cpp,v 1.1 2006/03/02 08:06:03 raimi Exp $
 *
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "complex.h"
#include "object.h"
#include "node.h"
#include "circuit.h"
#include "component_id.h"
#include "consts.h"
#include "constants.h"
#include "vpm.h"

vpm::vpm () : circuit (3) {
  type = CIR_VPM;
  setVSource (true);
  setVoltageSources (1);
}

void vpm::initSP (void) {
  allocMatrixS ();
  setS (NODE_1, NODE_2, 1.0);
  setS (NODE_2, NODE_1, 1.0);
  setS (NODE_3, NODE_3, 1.0);
}

void vpm::initDC (void) {
  allocMatrixMNA ();
  voltageSource (VSRC_1, NODE_1, NODE_2);
}

void vpm::initAC (void) {
  initDC ();
  nr_double_t a = getPropertyDouble ("U");
  nr_double_t p = getPropertyDouble ("Phase");
  setE (VSRC_1, polar (a, rad (p)));
}

void vpm::initTR (void) {
  initDC ();
}

void vpm::calcTR (nr_double_t t) {
  nr_double_t f = getPropertyDouble ("f");
  nr_double_t p = getPropertyDouble ("Phase");
  nr_double_t d = getPropertyDouble ("M") * 2 * M_PI;
  nr_double_t a = getPropertyDouble ("U");
  nr_double_t v = real (getV  (NODE_3));
  nr_double_t b = 2 * M_PI * f * t + rad (p) + v * d;
  nr_double_t u = a * sin (b);
  nr_double_t g = a * cos (b) * d;
  setE (VSRC_1, g * v - u);
  setC (VSRC_1, NODE_3, g);
}
