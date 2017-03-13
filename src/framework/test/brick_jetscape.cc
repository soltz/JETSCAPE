// Copyright @ Chun Shen
#include <stdio.h>
#include <sys/stat.h>

#include <cstring>
#include <cmath>
#include <iostream>

#include "JetScapeLogger.h"

//#include "fluid_dynamics.h"
#include "brick_jetscape.h"

using namespace std;


Brick::Brick() : FluidDynamics() {
    // initialize the parameter reader
    T_brick = 0.0;  // GeV

    hydro_status = NOT_START;
    SetId("Brick");
    VERBOSE(8);
}


Brick::~Brick() {VERBOSE(8);}


// not yet happy, get initialize_hydro(parameter_list); in parent class !???

void Brick::InitTask()
{
  // kind of stupid ... do pointer GetHydroXML() via XML instance ...
  //FluidDynamics::Init();
  
  DEBUG<<"Initialize Brick (Test) ...";
  VERBOSE(8);
  tinyxml2::XMLElement *brick=GetHydroXML()->FirstChildElement("Brick");

  if (brick)
    {
      string s = brick->FirstChildElement( "name" )->GetText();

      DEBUG << s << " to be initilizied ...";
      
      brick->FirstChildElement("T")->QueryDoubleText(&T_brick);

      DEBUG << s << " with T = "<<T_brick;
      INFO<<"Brick Temperature T = "<<T_brick;

      //Parameter parameter_list;
      GetParameterList().hydro_input_filename = (char*) "dummy"; //*(argv+1);

    }
  else
    {
      WARN << " : Brick not properly initialized in XML file ...";
      exit(-1);
    }
  
}

/*
void Brick::Exec()
{
  INFO<<"Run Brick Hydro ...";
  //INFO<<"Found "<<GetNumberOfTasks()<<" Hydro Tasks/Modules Execute them ... ";
  VERBOSE(8)<<"Current Event #"<<GetCurrentEvent();
  // With current event number as static it is easy to define now for hydro event reading from file how often to be reused
  // can add aslo an other (static) variable for that in Fluiddynamics ...

  //evolve_hydro();

  FluidDynamics::Exec();
  //JetScapeTask::ExecuteTasks();
}
*/

void Brick::initialize_hydro(Parameter parameter_list) {
  VERBOSE(8)<<parameter_list.hydro_input_filename;
    hydro_status = INITIALIZED;
}


void Brick::evolve_hydro() {
  VERBOSE(8);
  //cout<<hydro_status<<endl;

  hydro_status = FINISHED;

  /*
  if (hydro_status == INITIALIZED)
    {
      
    }
  else
    {
     WARN<<"Hydro not properly initialized ...";
      exit(-1);
    }
  */		    
}


void Brick::get_hydro_info(real t, real x, real y, real z,
                           FluidCellInfo* fluid_cell_info_ptr) {
    // assign all the quantites to JETSCAPE output
    // thermodyanmic quantities

  if (hydro_status == FINISHED)
    {
      fluid_cell_info_ptr->energy_density = 0.0;
      fluid_cell_info_ptr->entropy_density = 0.0;
      fluid_cell_info_ptr->temperature = T_brick;
      fluid_cell_info_ptr->pressure = 0.0;
      // QGP fraction
      fluid_cell_info_ptr->qgp_fraction = 1.0;
      // chemical potentials
      fluid_cell_info_ptr->mu_B = 0.0;
      fluid_cell_info_ptr->mu_C = 0.0;
      fluid_cell_info_ptr->mu_S = 0.0;
      // dynamical quantites
      fluid_cell_info_ptr->vx = 0.0;
      fluid_cell_info_ptr->vy = 0.0;
      fluid_cell_info_ptr->vz = 0.0;
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
	  fluid_cell_info_ptr->pi[i][j] = 0.0;
        }
      }
      fluid_cell_info_ptr->bulk_Pi = 0.0;
    }
  else
    {
      WARN<<"Hydro not run yet ...";
      exit(-1);
    }
}
