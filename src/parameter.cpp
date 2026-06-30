/* 

Copyright 1996-2006 Roeland Merks

This file is part of Tissue Simulation Toolkit.

Tissue Simulation Toolkit is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

Tissue Simulation Toolkit is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tissue Simulation Toolkit; if not, write to the Free
Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
02110-1301 USA

*/


#include "parameter.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <iostream>
#include <fstream>
#include "output.h"
#include "parse.h"

Parameter::Parameter() {

  T = 50.;
  target_area = 100;
  target_length = 60;
  lambda = 50;
  lambda2 = 5.0;
  target_perimeter = 0;
  lambda_perimeter = 0;
  Jtable = strdup("J.dat");
  TypesParamTable = 0;
  n_cell_types = 0;
  target_area_per_type = 0;
  target_length_per_type = 0;
  target_perimeter_per_type = 0;
  lambda_per_type = 0;
  lambda2_per_type = 0;
  lambda_perimeter_per_type = 0;
  init_chem_per_type = 0;
  conn_diss = 2000;
  vecadherinknockout = false;
  extensiononly = false;
  chemotaxis = 1000;
  border_energy = 100;
  neighbours = 2;
  periodic_boundaries = false;
  n_chem = 1;
  diff_coeff = new double[1];
  diff_coeff[0] = 1e-13;
  decay_rate = new double[1];
  decay_rate[0] = 1.8e-4;
  secr_rate = new double[1];
  secr_rate[0] = 1.8e-4;
  saturation = 0;
  dt = 2.0;
  dx = 2.0e-6;
  pde_its = 15;
  n_init_cells = 100;
  size_init_cells = 10;
  sizex = 200;
  sizey = 200;
  divisions = 0;
  mcs = 10000;
  rseed = -1;
  subfield = 1.0;
  relaxation = 0;
  storage_stride = 10;
  active_motion = false;
  graphics = true;
  store = false;
  datadir = strdup("data_film");
}

Parameter::~Parameter() {
  
  // destruct parameter object

  // free string parameter

  CleanUp();

}

void Parameter::CleanUp(void) {
  if (Jtable) 
     free(Jtable);
  if (diff_coeff) 
     free(diff_coeff);
  if (decay_rate) 
     free(decay_rate);
  if (secr_rate) 
     free(secr_rate);
  if (datadir)
     free(datadir);
  if (TypesParamTable)
     free(TypesParamTable);
  if (target_area_per_type)
     free(target_area_per_type);
  if (target_length_per_type)
     free(target_length_per_type);
  if (target_perimeter_per_type)
     free(target_perimeter_per_type);
  if (lambda_per_type)
     free(lambda_per_type);
  if (lambda2_per_type)
     free(lambda2_per_type);
  if (lambda_perimeter_per_type)
     free(lambda_perimeter_per_type);
  if (init_chem_per_type) {
    for (int i = 0; i < n_cell_types; i++)
      free(init_chem_per_type[i]);
    free(init_chem_per_type);
    init_chem_per_type = 0;
  }

}

void Parameter::Read(const char *filename) {
  
  static bool ReadP=false;

  if (ReadP) {

    //throw "Run Time Error in parameter.cpp: Please Read parameter file only once!!";
    CleanUp();
    
  } else
    ReadP=true;

  FILE *fp=OpenReadFile(filename);


  T = fgetpar(fp, "T", 50., true);
  target_area = igetpar(fp, "target_area", 100, true);
  target_length = igetpar(fp, "target_length", 60, true);
  lambda = fgetpar(fp, "lambda", 50, true);
  lambda2 = fgetpar(fp, "lambda2", 5.0, true);
  target_perimeter = igetpar(fp, "target_perimeter", 0, true);
  lambda_perimeter = igetpar(fp, "lambda_perimeter", 0, true);
  Jtable = sgetpar(fp, "Jtable", "J.dat", true);
  TypesParamTable = sgetpar(fp, "TypesParamTable", "", true);
  conn_diss = igetpar(fp, "conn_diss", 2000, true);
  vecadherinknockout = bgetpar(fp, "vecadherinknockout", false, true);
  extensiononly = bgetpar(fp, "extensiononly", false, true);
  chemotaxis = igetpar(fp, "chemotaxis", 1000, true);
  border_energy = igetpar(fp, "border_energy", 100, true);
  neighbours = igetpar(fp, "neighbours", 2, true);
  periodic_boundaries = bgetpar(fp, "periodic_boundaries", false, true);
  n_chem = igetpar(fp, "n_chem", 1, true);
  if (TypesParamTable && strlen(TypesParamTable) > 0)
    ReadTypesParamTable(TypesParamTable);
  diff_coeff = dgetparlist(fp, "diff_coeff", n_chem, true);
  decay_rate = dgetparlist(fp, "decay_rate", n_chem, true);
  secr_rate = dgetparlist(fp, "secr_rate", n_chem, true);
  saturation = fgetpar(fp, "saturation", 0, true);
  dt = fgetpar(fp, "dt", 2.0, true);
  dx = fgetpar(fp, "dx", 2.0e-6, true);
  pde_its = igetpar(fp, "pde_its", 15, true);
  n_init_cells = igetpar(fp, "n_init_cells", 100, true);
  size_init_cells = igetpar(fp, "size_init_cells", 10, true);
  sizex = igetpar(fp, "sizex", 200, true);
  sizey = igetpar(fp, "sizey", 200, true);
  divisions = igetpar(fp, "divisions", 0, true);
  mcs = igetpar(fp, "mcs", 10000, true);
  rseed = igetpar(fp, "rseed", -1, true);
  subfield = fgetpar(fp, "subfield", 1.0, true);
  relaxation = igetpar(fp, "relaxation", 0, true);
  storage_stride = igetpar(fp, "storage_stride", 10, true);
  active_motion = bgetpar(fp, "active_motion", false, true);
  graphics = bgetpar(fp, "graphics", true, true);
  store = bgetpar(fp, "store", false, true);
  datadir = sgetpar(fp, "datadir", "data_film", true);

}

const char *sbool(const bool &p) {

  const char *true_str="true";
  const char *false_str="false";
  if (p)
    return true_str;
  else
    return false_str;
}

void Parameter::Write(ostream &os) const {
  setlocale(LC_NUMERIC, "C");

  os << " T = " << T << endl;
  os << " target_area = " << target_area << endl;
  os << " target_length = " << target_length << endl;
  os << " lambda = " << lambda << endl;
  os << " lambda2 = " << lambda2 << endl;
  os << " target_perimeter = " << target_perimeter << endl;
  os << " lambda_perimeter = " << lambda_perimeter << endl;

  if (Jtable)
    os << " Jtable = " << Jtable << endl;
  if (TypesParamTable && strlen(TypesParamTable) > 0)
    os << " TypesParamTable = " << TypesParamTable << endl;
  os << " conn_diss = " << conn_diss << endl;
  os << " vecadherinknockout = " << sbool(vecadherinknockout) << endl;
  os << " extensiononly = " << sbool(extensiononly) << endl;
  os << " chemotaxis = " << chemotaxis << endl;
  os << " border_energy = " << border_energy << endl;
  os << " neighbours = " << neighbours << endl;
  os << " periodic_boundaries = " << sbool(periodic_boundaries) << endl;
  os << " n_chem = " << n_chem << endl;
  os << " diff_coeff = "<< diff_coeff[0] << endl;
  os << " decay_rate = "<< decay_rate[0] << endl;
  os << " secr_rate = "<< secr_rate[0] << endl;
  os << " saturation = " << saturation << endl;
  os << " dt = " << dt << endl;
  os << " dx = " << dx << endl;
  os << " pde_its = " << pde_its << endl;
  os << " n_init_cells = " << n_init_cells << endl;
  os << " size_init_cells = " << size_init_cells << endl;
  os << " sizex = " << sizex << endl;
  os << " sizey = " << sizey << endl;
  os << " divisions = " << divisions << endl;
  os << " mcs = " << mcs << endl;
  os << " rseed = " << rseed << endl;
  os << " subfield = " << subfield << endl;
  os << " relaxation = " << relaxation << endl;
  os << " storage_stride = " << storage_stride << endl;
  os << " active_motion = " << sbool(active_motion) << endl;
  os << " graphics = " << sbool(graphics) << endl;
  os << " store = " << sbool(store) << endl;

  if (datadir) 
    os << " datadir = " << datadir << endl;
}


ostream &operator<<(ostream &os, Parameter &p) {
  p.Write(os);
  return os;
}

void Parameter::ReadTypesParamTable(const char *fname) {

  cerr << "Reading per-type parameters...\n";
  ifstream f(fname);
  if (!f) {
    perror(fname);
    exit(1);
  }

  f >> n_cell_types;
  cerr << "Number of cell types: " << n_cell_types << endl;

  target_area_per_type = (int *)malloc(n_cell_types * sizeof(int));
  target_length_per_type = (int *)malloc(n_cell_types * sizeof(int));
  target_perimeter_per_type = (int *)malloc(n_cell_types * sizeof(int));
  lambda_per_type = (double *)malloc(n_cell_types * sizeof(double));
  lambda2_per_type = (double *)malloc(n_cell_types * sizeof(double));
  lambda_perimeter_per_type = (int *)malloc(n_cell_types * sizeof(int));

  init_chem_per_type = (double **)malloc(n_cell_types * sizeof(double *));
  for (int i = 0; i < n_cell_types; i++)
    init_chem_per_type[i] = (double *)calloc(n_chem, sizeof(double));

  for (int i = 0; i < n_cell_types; i++) {
    f >> target_area_per_type[i]
      >> target_length_per_type[i]
      >> target_perimeter_per_type[i]
      >> lambda_per_type[i]
      >> lambda2_per_type[i]
      >> lambda_perimeter_per_type[i];
    for (int ch = 0; ch < n_chem; ch++) {
      if (!(f >> init_chem_per_type[i][ch]))
        init_chem_per_type[i][ch] = 0.0;
    }
  }
}

Parameter par;
