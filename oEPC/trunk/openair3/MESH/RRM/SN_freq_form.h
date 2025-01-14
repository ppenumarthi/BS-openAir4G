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

/** Header file generated with fdesign on Tue Jun  1 09:25:58 2010.**/

#ifndef FD_Secondary_Network_frequencies_h_
#define FD_Secondary_Network_frequencies_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *Secondary_Network_frequencies;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *Selected_frequencies;
} FD_Secondary_Network_frequencies;

extern FD_Secondary_Network_frequencies * create_form_Secondary_Network_frequencies(void);

#endif /* FD_Secondary_Network_frequencies_h_ */
