/*******************************************************************************
    OpenAirInterface 
    Copyright(c) 1999 - 2014 Eurecom

    OpenAirInterface is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.


    OpenAirInterface is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenAirInterface.The full GNU General Public License is 
   included in this distribution in the file called "COPYING". If not, 
   see <http://www.gnu.org/licenses/>.

  Contact Information
  OpenAirInterface Admin: openair_admin@eurecom.fr
  OpenAirInterface Tech : openair_tech@eurecom.fr
  OpenAirInterface Dev  : openair4g-devel@eurecom.fr
  
  Address      : Eurecom, Campus SophiaTech, 450 Route des Chappes, CS 50193 - 06410 Biot Sophia Antipolis cedex, FRANCE

 *******************************************************************************/

/** Header file generated with fdesign on Fri Nov  5 17:11:31 2010.**/

#ifndef FD_sens_CH1_scen_2_h_
#define FD_sens_CH1_scen_2_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *sens_CH1_scen_2;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *User_1_sens;
	FL_OBJECT *User_2_sens;
	FL_OBJECT *User_3_sens;
	FL_OBJECT *User_4_sens;
	FL_OBJECT *Cluster_2_sensing;
} FD_sens_CH1_scen_2;

extern FD_sens_CH1_scen_2 * create_form_sens_CH1_scen_2(void);

#endif /* FD_sens_CH1_scen_2_h_ */
