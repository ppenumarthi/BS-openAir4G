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
  
  Address      : Eurecom, Campus SophiaTech, 450 Route des Chappes, CS 50193 - 06904 Biot Sophia Antipolis cedex, FRANCE

 *******************************************************************************/
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "SIMULATION/TOOLS/defs.h"
#include "SIMULATION/RF/defs.h"
#include "PHY/types.h"
#include "PHY/defs.h"
#include "PHY/vars.h"
#include "MAC_INTERFACE/vars.h"
#ifdef EMOS
#include "SCHED/phy_procedures_emos.h"
#endif
#include "SCHED/defs.h"
#include "SCHED/vars.h"
#include "LAYER2/MAC/vars.h"

#ifdef XFORMS
#include <forms.h>
#include "../../USERSPACE_TOOLS/SCOPE/lte_scope.h"
#endif //XFORMS


#include "OCG_vars.h"

//#define BW 5.0


PHY_VARS_eNB *PHY_vars_eNB;
PHY_VARS_UE *PHY_vars_UE;

void lte_param_init(unsigned char N_tx, unsigned char N_rx,unsigned char transmission_mode,uint8_t extended_prefix_flag,lte_frame_type_t frame_type, uint16_t Nid_cell,uint8_t tdd_config,uint8_t N_RB_DL,uint8_t osf) {

  LTE_DL_FRAME_PARMS *lte_frame_parms;


  printf("Start lte_param_init\n");
  PHY_vars_eNB = malloc(sizeof(PHY_VARS_eNB));
  PHY_vars_UE = malloc(sizeof(PHY_VARS_UE));
  //PHY_config = malloc(sizeof(PHY_CONFIG));
  mac_xface = malloc(sizeof(MAC_xface));

  srand(1);
  randominit(1);
  set_taus_seed(1);
  
  lte_frame_parms = &(PHY_vars_eNB->lte_frame_parms);

  lte_frame_parms->N_RB_DL            = N_RB_DL;   //50 for 10MHz and 25 for 5 MHz
  lte_frame_parms->N_RB_UL            = N_RB_DL;   
  lte_frame_parms->Ncp                = extended_prefix_flag;
  lte_frame_parms->Nid_cell           = Nid_cell;
  lte_frame_parms->Nid_cell_mbsfn     = 1;
  lte_frame_parms->nushift            = Nid_cell%6;
  lte_frame_parms->nb_antennas_tx     = N_tx;
  lte_frame_parms->nb_antennas_rx     = N_rx;
  lte_frame_parms->phich_config_common.phich_resource         = oneSixth;
  lte_frame_parms->tdd_config         = tdd_config;
  lte_frame_parms->frame_type         = frame_type;
  //  lte_frame_parms->Csrs = 2;
  //  lte_frame_parms->Bsrs = 0;
  //  lte_frame_parms->kTC = 0;44
  //  lte_frame_parms->n_RRC = 0;
  lte_frame_parms->mode1_flag = (transmission_mode == 1)? 1 : 0;

  init_frame_parms(lte_frame_parms,osf);
  
  //copy_lte_parms_to_phy_framing(lte_frame_parms, &(PHY_config->PHY_framing));
  
  phy_init_top(lte_frame_parms); //allocation
  
  lte_frame_parms->twiddle_fft      = twiddle_fft;
  lte_frame_parms->twiddle_ifft     = twiddle_ifft;
  lte_frame_parms->rev              = rev;
  
  PHY_vars_UE->is_secondary_ue = 0;
  PHY_vars_UE->lte_frame_parms = *lte_frame_parms;
  PHY_vars_eNB->lte_frame_parms = *lte_frame_parms;

  phy_init_lte_top(lte_frame_parms);
  dump_frame_parms(lte_frame_parms);

  PHY_vars_UE->PHY_measurements.n_adj_cells=2;
  PHY_vars_UE->PHY_measurements.adj_cell_id[0] = Nid_cell+1;
  PHY_vars_UE->PHY_measurements.adj_cell_id[1] = Nid_cell+2;

  lte_gold_mbsfn(lte_frame_parms,PHY_vars_UE->lte_gold_mbsfn_table,Nid_cell);    
  lte_gold_mbsfn(lte_frame_parms,PHY_vars_eNB->lte_gold_mbsfn_table,Nid_cell);    

  phy_init_lte_ue(PHY_vars_UE,1,0);
  phy_init_lte_eNB(PHY_vars_eNB,0,0,0);

  
  printf("Done lte_param_init\n");


}
DCI1E_5MHz_2A_M10PRB_TDD_t  DLSCH_alloc_pdu2_1E[2];
#define UL_RB_ALLOC 0x1ff;
#define CCCH_RB_ALLOC computeRIV(PHY_vars_eNB->lte_frame_parms.N_RB_UL,0,2)
int main(int argc, char **argv) {

  char c;

  int i,l,aa,aarx;
  double sigma2, sigma2_dB=0,SNR,snr0=-2.0,snr1=0.0;
  uint8_t snr1set=0;
  double snr_step=1,input_snr_step=1;
  //mod_sym_t **txdataF;
  int **txdata;
  double **s_re,**s_im,**r_re,**r_im;
  double iqim = 0.0;
  int subframe=1;
  char fname[40];//, vname[40];
  uint8_t transmission_mode = 1,n_tx=1,n_rx=2;
  uint16_t Nid_cell=0;

  FILE *fd;

  int eNB_id = 0;
  unsigned char mcs=0,awgn_flag=0,round;

  int n_frames=1;
  channel_desc_t *eNB2UE;
  uint32_t nsymb,tx_lev,tx_lev_dB;
  uint8_t extended_prefix_flag=1;
  LTE_DL_FRAME_PARMS *frame_parms;
  int hold_channel=0;

  
  uint16_t NB_RB=25;

  int tdd_config=3;
  
  SCM_t channel_model=MBSFN;


  unsigned char *input_buffer;
  unsigned short input_buffer_length;
  unsigned int ret;
 
  unsigned int trials,errs[4]={0,0,0,0};//,round_trials[4]={0,0,0,0};
  
  uint8_t N_RB_DL=25,osf=1;
  double BW=5.0;

  lte_frame_type_t frame_type = FDD; 


#ifdef XFORMS
  FD_lte_scope *form_dl;
  char title[255];

  fl_initialize (&argc, argv, NULL, 0, 0);
  form_dl = create_form_lte_scope();
  sprintf (title, "LTE DL SCOPE UE");
  fl_show_form (form_dl->lte_scope, FL_PLACE_HOTSPOT, FL_FULLBORDER, title);
#endif

  logInit();
  number_of_cards = 1;
  openair_daq_vars.rx_rf_mode = 1;
  
  /*
    rxdataF    = (int **)malloc16(2*sizeof(int*));
    rxdataF[0] = (int *)malloc16(FRAME_LENGTH_BYTES);
    rxdataF[1] = (int *)malloc16(FRAME_LENGTH_BYTES);
    
    rxdata    = (int **)malloc16(2*sizeof(int*));
    rxdata[0] = (int *)malloc16(FRAME_LENGTH_BYTES);
    rxdata[1] = (int *)malloc16(FRAME_LENGTH_BYTES);
  */
  while ((c = getopt (argc, argv, "ahA:Cp:n:s:S:t:x:y:z:N:F:R:O:dm:i:")) != -1)
    {
      switch (c)
	{
	case 'a':
	  awgn_flag=1;
	  break;
	case 'd':
	  frame_type = 0;
	  break;
	case 'n':
	  n_frames = atoi(optarg);
	  break;
	case 'm':
	  mcs=atoi(optarg);
	  break;
	case 's':
	  snr0 = atof(optarg);
	  msg("Setting SNR0 to %f\n",snr0);
	  break;
	case 'i':
	  input_snr_step = atof(optarg);
	  break;
	case 'S':
	  snr1 = atof(optarg);
	  snr1set=1;
	  msg("Setting SNR1 to %f\n",snr1);
	  break;
	case 'p': // subframe no;
	  subframe=atoi(optarg);
	  break;
	case 'z':
	  n_rx=atoi(optarg);
	  if ((n_rx==0) || (n_rx>2)) {
	    msg("Unsupported number of rx antennas %d\n",n_rx);
	    exit(-1);
	  }
	  break;
	case 'N':
	  Nid_cell = atoi(optarg);
	  break;
	case 'R':
	  N_RB_DL = atoi(optarg);
	  switch (N_RB_DL) {
	  case 6:
	    BW=1.25;
	    break;
	  case 25:
	    BW=5.0;
	    break;
	  case 50:
	    BW=10.0;
	    break;
	  case 100:
	    BW=20.0;
	    break;
	  default:
	    printf("Unsupported Bandwidth %d\n",N_RB_DL);
	    exit(-1);
	    break;
	  }
	  break;
	case 'O':
	  osf = atoi(optarg);
	  break;
	default:
	case 'h':
	  printf("%s -h(elp) -p(subframe) -N cell_id -g channel_model -n n_frames -t Delayspread -s snr0 -S snr1 -i snr increment -z RXant \n",argv[0]);
	  printf("-h This message\n");
	  printf("-a Use AWGN Channel\n");
	  printf("-p Use extended prefix mode\n");
	  printf("-d Use TDD\n");
	  printf("-n Number of frames to simulate\n");
	  printf("-s Starting SNR, runs from SNR0 to SNR0 + 5 dB.  If n_frames is 1 then just SNR is simulated\n");
	  printf("-S Ending SNR, runs from SNR0 to SNR1\n");
	  printf("-t Delay spread for multipath channel\n");
	  printf("-g [A,B,C,D,E,F,G] Use 3GPP SCM (A,B,C,D) or 36-101 (E-EPA,F-EVA,G-ETU) models (ignores delay spread and Ricean factor)\n");
	  printf("-x Transmission mode (1,2,6 for the moment)\n");
	  printf("-y Number of TX antennas used in eNB\n");
	  printf("-z Number of RX antennas used in UE\n");
	  printf("-i Relative strength of first intefering eNB (in dB) - cell_id mod 3 = 1\n");
	  printf("-j Relative strength of second intefering eNB (in dB) - cell_id mod 3 = 2\n");
	  printf("-N Nid_cell\n");
	  printf("-R N_RB_DL\n");
	  printf("-O oversampling factor (1,2,4,8,16)\n");
	  printf("-A Interpolation_filname Run with Abstraction to generate Scatter plot using interpolation polynomial in file\n");
	  printf("-C Generate Calibration information for Abstraction (effective SNR adjustment to remove Pe bias w.r.t. AWGN)\n");
	  printf("-f Output filename (.txt format) for Pe/SNR results\n");
	  printf("-F Input filename (.txt format) for RX conformance testing\n");
	  exit (-1);
	  break;
	}
    }
  if (awgn_flag == 1)
    channel_model = AWGN;

  // check that subframe is legal for eMBMS

  if ((subframe == 0) || (subframe == 5) ||    // TDD and FDD SFn 0,5;
      ((frame_type == FDD) && ((subframe == 4) || (subframe == 9))) || // FDD SFn 4,9;
      ((frame_type == TDD ) && ((subframe<3) || (subframe==6)))) 	  {  // TDD SFn 1,2,6;
    
    printf("Illegal subframe %d for eMBMS transmission (frame_type %d)\n",subframe,frame_type);
    exit(-1);
  } 
  if (transmission_mode==2)
    n_tx=2;

  lte_param_init(n_tx,n_rx,transmission_mode,extended_prefix_flag,frame_type,Nid_cell,tdd_config,N_RB_DL,osf);



  if (snr1set==0) {
    if (n_frames==1)
      snr1 = snr0+.1;
    else
      snr1 = snr0+5.0;
  }

  printf("SNR0 %f, SNR1 %f\n",snr0,snr1);

  frame_parms = &PHY_vars_eNB->lte_frame_parms;

  if (awgn_flag == 0)
    sprintf(fname,"embms_%d_%d.m",mcs,N_RB_DL); 
  else
    sprintf(fname,"embms_awgn_%d_%d.m",mcs,N_RB_DL);
  fd = fopen(fname,"w");
  if (awgn_flag==0) 
    fprintf(fd,"SNR_%d_%d=[];errs_mch_%d_%d=[];mch_trials_%d_%d=[];\n",
	    mcs,N_RB_DL,
	    mcs,N_RB_DL,
	    mcs,N_RB_DL);
  else
    fprintf(fd,"SNR_awgn_%d_%d=[];errs_mch_awgn_%d_%d=[];mch_trials_awgn_%d_%d=[];\n",
	    mcs,N_RB_DL,
	    mcs,N_RB_DL,
	    mcs,N_RB_DL);
  fflush(fd);

  txdata = PHY_vars_eNB->lte_eNB_common_vars.txdata[0];

  s_re = malloc(2*sizeof(double*));
  s_im = malloc(2*sizeof(double*));
  r_re = malloc(2*sizeof(double*));
  r_im = malloc(2*sizeof(double*));
  nsymb = 12;

  printf("FFT Size %d, Extended Prefix %d, Samples per subframe %d, Symbols per subframe %d, AWGN %d\n",NUMBER_OF_OFDM_CARRIERS,
	 frame_parms->Ncp,frame_parms->samples_per_tti,nsymb,awgn_flag);

  for (i=0;i<2;i++) {

    s_re[i] = malloc(FRAME_LENGTH_COMPLEX_SAMPLES*sizeof(double));
    bzero(s_re[i],FRAME_LENGTH_COMPLEX_SAMPLES*sizeof(double));
    s_im[i] = malloc(FRAME_LENGTH_COMPLEX_SAMPLES*sizeof(double));
    bzero(s_im[i],FRAME_LENGTH_COMPLEX_SAMPLES*sizeof(double));
    r_re[i] = malloc(FRAME_LENGTH_COMPLEX_SAMPLES*sizeof(double));
    bzero(r_re[i],FRAME_LENGTH_COMPLEX_SAMPLES*sizeof(double));
    r_im[i] = malloc(FRAME_LENGTH_COMPLEX_SAMPLES*sizeof(double));
    bzero(r_im[i],FRAME_LENGTH_COMPLEX_SAMPLES*sizeof(double));
  }
  
  eNB2UE = new_channel_desc_scm(PHY_vars_eNB->lte_frame_parms.nb_antennas_tx,
				PHY_vars_UE->lte_frame_parms.nb_antennas_rx,
				channel_model,
				BW,
				0,
				0,
				0);
				
    // Create transport channel structures for 2 transport blocks (MIMO)
  PHY_vars_eNB->dlsch_eNB_MCH = new_eNB_dlsch(1,8,N_RB_DL,0);
  
  if (!PHY_vars_eNB->dlsch_eNB_MCH) {
    printf("Can't get eNB dlsch structures\n");
    exit(-1);
  }
  
  PHY_vars_UE->dlsch_ue_MCH[0]  = new_ue_dlsch(1,8,MAX_TURBO_ITERATIONS_MBSFN,N_RB_DL,0);

  PHY_vars_eNB->lte_frame_parms.num_MBSFN_config = 1;
  PHY_vars_eNB->lte_frame_parms.MBSFN_config[0].radioframeAllocationPeriod = 0;
  PHY_vars_eNB->lte_frame_parms.MBSFN_config[0].radioframeAllocationOffset = 0;
  PHY_vars_eNB->lte_frame_parms.MBSFN_config[0].fourFrames_flag = 0;
  PHY_vars_eNB->lte_frame_parms.MBSFN_config[0].mbsfn_SubframeConfig=0xff; // activate all possible subframes
  PHY_vars_UE->lte_frame_parms.num_MBSFN_config = 1;
  PHY_vars_UE->lte_frame_parms.MBSFN_config[0].radioframeAllocationPeriod = 0;
  PHY_vars_UE->lte_frame_parms.MBSFN_config[0].radioframeAllocationOffset = 0;
  PHY_vars_UE->lte_frame_parms.MBSFN_config[0].fourFrames_flag = 0;
  PHY_vars_UE->lte_frame_parms.MBSFN_config[0].mbsfn_SubframeConfig=0xff; // activate all possible subframes

  fill_eNB_dlsch_MCH(PHY_vars_eNB,mcs,1,0,0);
  fill_UE_dlsch_MCH(PHY_vars_UE,mcs,1,0,0);
  if (is_pmch_subframe(0,subframe,&PHY_vars_eNB->lte_frame_parms)==0) {
    printf("eNB is not configured for MBSFN in subframe %d\n",subframe);
    exit(-1);
  }
  else if (is_pmch_subframe(0,subframe,&PHY_vars_UE->lte_frame_parms)==0) {
    printf("UE is not configured for MBSFN in subframe %d\n",subframe);
    exit(-1);    
  }


  input_buffer_length = PHY_vars_eNB->dlsch_eNB_MCH->harq_processes[0]->TBS/8;
  input_buffer = (unsigned char *)malloc(input_buffer_length+4);
  memset(input_buffer,0,input_buffer_length+4);
  for (i=0;i<input_buffer_length+4;i++) {
    input_buffer[i]= (unsigned char)(taus()&0xff);
  }


  snr_step = input_snr_step;
  for (SNR=snr0;SNR<snr1;SNR+=snr_step) {
    PHY_vars_UE->frame_tx=0;
    PHY_vars_eNB->proc[subframe].frame_tx=0;
   
    errs[0]=0;
    errs[1]=0;
    errs[2]=0;
    errs[3]=0;
    /*
    round_trials[0] = 0;
    round_trials[1] = 0;
    round_trials[2] = 0;
    round_trials[3] = 0;*/
    printf("********************** SNR %f (step %f)\n",SNR,snr_step);

    for (trials = 0;trials<n_frames;trials++) {
      //        printf("Trial %d\n",trials);
      fflush(stdout);
      round=0;
      
      //if (trials%100==0)
      //eNB2UE[0]->first_run = 1;
      eNB2UE->first_run = 1;
      memset(&PHY_vars_eNB->lte_eNB_common_vars.txdataF[0][0][0],0,FRAME_LENGTH_COMPLEX_SAMPLES_NO_PREFIX*sizeof(mod_sym_t));

   generate_mch(PHY_vars_eNB,subframe,input_buffer,0);

      PHY_ofdm_mod(PHY_vars_eNB->lte_eNB_common_vars.txdataF[0][0],        // input,
		   txdata[0],         // output
		   frame_parms->log2_symbol_size,                // log2_fft_size
		   LTE_NUMBER_OF_SUBFRAMES_PER_FRAME*nsymb,                 // number of symbols
		   frame_parms->nb_prefix_samples,               // number of prefix samples
		   frame_parms->twiddle_ifft,  // IFFT twiddle factors
		   frame_parms->rev,           // bit-reversal permutation
		   CYCLIC_PREFIX);

      if (n_frames==1) {
	write_output("txsigF0.m","txsF0", &PHY_vars_eNB->lte_eNB_common_vars.txdataF[eNB_id][0][subframe*nsymb*PHY_vars_eNB->lte_frame_parms.ofdm_symbol_size],nsymb*PHY_vars_eNB->lte_frame_parms.ofdm_symbol_size,1,1);
	//if (PHY_vars_eNB->lte_frame_parms.nb_antennas_tx>1)
	//write_output("txsigF1.m","txsF1", &PHY_vars_eNB->lte_eNB_common_vars.txdataF[eNB_id][1][subframe*nsymb*PHY_vars_eNB->lte_frame_parms.ofdm_symbol_size],nsymb*PHY_vars_eNB->lte_frame_parms.ofdm_symbol_size,1,1);
      }

      tx_lev = 0;
      for (aa=0; aa<PHY_vars_eNB->lte_frame_parms.nb_antennas_tx; aa++) {
	tx_lev += signal_energy(&PHY_vars_eNB->lte_eNB_common_vars.txdata[eNB_id][aa]
				[subframe*PHY_vars_eNB->lte_frame_parms.samples_per_tti],
				PHY_vars_eNB->lte_frame_parms.samples_per_tti);
      }
      tx_lev_dB = (unsigned int) dB_fixed(tx_lev);
      
      if (n_frames==1) {
	printf("tx_lev = %d (%d dB)\n",tx_lev,tx_lev_dB);
	//    write_output("txsig0.m","txs0", &PHY_vars_eNB->lte_eNB_common_vars.txdata[0][0][subframe* PHY_vars_eNB->lte_frame_parms.samples_per_tti],
	
	//	   PHY_vars_eNB->lte_frame_parms.samples_per_tti,1,1);
      }
      
      for (i=0;i<2*frame_parms->samples_per_tti;i++) {
	for (aa=0;aa<PHY_vars_eNB->lte_frame_parms.nb_antennas_tx;aa++) {
	  s_re[aa][i] = ((double)(((short *)PHY_vars_eNB->lte_eNB_common_vars.txdata[0][aa]))[(2*subframe*PHY_vars_UE->lte_frame_parms.samples_per_tti) + (i<<1)]);
	  s_im[aa][i] = ((double)(((short *)PHY_vars_eNB->lte_eNB_common_vars.txdata[0][aa]))[(2*subframe*PHY_vars_UE->lte_frame_parms.samples_per_tti) +(i<<1)+1]);
	}
      }
      //Multipath channel
      multipath_channel(eNB2UE,s_re,s_im,r_re,r_im,
			2*frame_parms->samples_per_tti,hold_channel);
      
      //AWGN
      sigma2_dB = 10*log10((double)tx_lev) +10*log10((double)PHY_vars_eNB->lte_frame_parms.ofdm_symbol_size/(NB_RB*12)) - SNR;
      sigma2 = pow(10,sigma2_dB/10);
      if (n_frames==1)
	printf("Sigma2 %f (sigma2_dB %f)\n",sigma2,sigma2_dB);
      
      for (i=0; i<2*frame_parms->samples_per_tti; i++) {
	for (aa=0;aa<PHY_vars_eNB->lte_frame_parms.nb_antennas_rx;aa++) {
	  //printf("s_re[0][%d]=> %f , r_re[0][%d]=> %f\n",i,s_re[aa][i],i,r_re[aa][i]);
	  ((short*) PHY_vars_UE->lte_ue_common_vars.rxdata[aa])[(2*subframe*PHY_vars_UE->lte_frame_parms.samples_per_tti)+2*i] = 
	    (short) (r_re[aa][i] + sqrt(sigma2/2)*gaussdouble(0.0,1.0));
	  ((short*) PHY_vars_UE->lte_ue_common_vars.rxdata[aa])[(2*subframe*PHY_vars_UE->lte_frame_parms.samples_per_tti)+2*i+1] = 
	    (short) (r_im[aa][i] + (iqim*r_re[aa][i]) + sqrt(sigma2/2)*gaussdouble(0.0,1.0));
	}
      }

     for (l=2;l<12;l++) {
	
	slot_fep_mbsfn(PHY_vars_UE,
		       l,
		       subframe%10,
		       0,
		       0);
      }
      
      for (l=2;l<12;l++) {
	rx_pmch(PHY_vars_UE,
		0,
		subframe%10,
		l);
	
	
      }

      PHY_vars_UE->dlsch_ue_MCH[0]->harq_processes[0]->G = get_G(&PHY_vars_UE->lte_frame_parms,
								 PHY_vars_UE->dlsch_ue_MCH[0]->harq_processes[0]->nb_rb,
								 PHY_vars_UE->dlsch_ue_MCH[0]->harq_processes[0]->rb_alloc,
								 get_Qm(PHY_vars_UE->dlsch_ue_MCH[0]->harq_processes[0]->mcs),
								 1,2,
								 PHY_vars_UE->frame_tx,subframe);
      dlsch_unscrambling(&PHY_vars_UE->lte_frame_parms,1,PHY_vars_UE->dlsch_ue_MCH[0],
			 PHY_vars_UE->dlsch_ue_MCH[0]->harq_processes[0]->G,
			 PHY_vars_UE->lte_ue_pdsch_vars_MCH[0]->llr[0],0,subframe<<1);
      
      ret = dlsch_decoding(PHY_vars_UE,
			   PHY_vars_UE->lte_ue_pdsch_vars_MCH[0]->llr[0],		 
			   &PHY_vars_UE->lte_frame_parms,
			   PHY_vars_UE->dlsch_ue_MCH[0],
			   PHY_vars_UE->dlsch_ue_MCH[0]->harq_processes[0],
			   subframe,
			   0,0,0);    
      if (n_frames==1)
	printf("MCH decoding returns %d\n",ret);
      if (ret == (1+PHY_vars_UE->dlsch_ue_MCH[0]->max_turbo_iterations))
	errs[0]++;
      PHY_vars_UE->frame_tx++;
      PHY_vars_eNB->proc[subframe].frame_tx++;
    }
    printf("errors %d/%d (Pe %e)\n",errs[round],trials,(double)errs[round]/trials);

    if (awgn_flag==0)
      fprintf(fd,"SNR_%d_%d = [SNR_%d_%d %f]; errs_mch_%d_%d =[errs_mch_%d_%d  %d]; mch_trials_%d_%d =[mch_trials_%d_%d  %d];\n",
	      mcs,N_RB_DL,mcs,N_RB_DL,SNR,
	      mcs,N_RB_DL,mcs,N_RB_DL,errs[0],
	      mcs,N_RB_DL,mcs,N_RB_DL,trials);
    else
      fprintf(fd,"SNR_awgn_%d = [SNR_awgn_%d %d]; errs_mch_awgn_%d =[errs_mch_awgn_%f  %d]; mch_trials_awgn_%d =[mch_trials_awgn_%d %d];\n",
	      mcs,N_RB_DL,mcs,N_RB_DL,SNR,
	      mcs,N_RB_DL,mcs,N_RB_DL,errs[0],
	      mcs,N_RB_DL,mcs,N_RB_DL,trials);
    fflush(fd);
    if (errs[0] == 0)
      break;
    }


  if (n_frames==1) {
    printf("Dumping PMCH files ( G %d)\n",PHY_vars_UE->dlsch_ue_MCH[0]->harq_processes[0]->G);
    dump_mch(PHY_vars_UE,0,
	     PHY_vars_UE->dlsch_ue_MCH[0]->harq_processes[0]->G,
	     subframe);
  }
  printf("Freeing dlsch structures\n");
  free_eNB_dlsch(PHY_vars_eNB->dlsch_eNB_MCH);
  free_ue_dlsch(PHY_vars_UE->dlsch_ue_MCH[0]);

  fclose(fd);
  
  printf("Freeing channel I/O\n");
  for (i=0;i<2;i++) {
    free(s_re[i]);
    free(s_im[i]);
    free(r_re[i]);
    free(r_im[i]);
  }
  free(s_re);
  free(s_im);
  free(r_re);
  free(r_im);
  
  //  lte_sync_time_free();
  
  return(0);
  }
  
