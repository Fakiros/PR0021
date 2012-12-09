
#include  "Appl/scheduler.h"

#include  "Drv/DS18B20/Ds18x20.h"


#include  "Temperature/Temperature.h"




#define TEMP_GET_COUNTS_MS(x)  ((x)/MAIN_LOOP_PERIOD)

#define MovingAvg_new(Name, SamplesNo) \
   static signed int Name##_buffer[(SamplesNo)];\
	static MovingAvg_t Name = {SamplesNo, &##Name##_buffer, 0};


typedef enum Temp_ReadState
{
   TEMPERATURE_MEASURE_REQUEST,
   TEMPERATURE_WAITING_FOR_RESPONSE,
   TEMPERATURE_UPDATE,
}Temp_ReadState_t;





typedef struct
{
	unsigned char samples_no;
    signed int  * buffer_ptr;
	unsigned char write_index;
}MovingAvg_t;


void MovingAvg_SetSample(MovingAvg_t * object_name, signed int sample);
signed int MovingAvg_GetAvgSamples(MovingAvg_t * object_name, signed int samples_no);
signed int MovingAvg_GetAvg(MovingAvg_t * object_name);
signed char MovingAvg_NoOfAvgSamples(MovingAvg_t * object_name);


MovingAvg_new( Temp_avg_10s , 10u);
MovingAvg_new( Temp_avg_1min , 6u);
MovingAvg_new( Temp_avg_10min , 10u);
MovingAvg_new( Temp_avg_1h , 6u);
MovingAvg_new( Temp_avg_1d , 24u);
MovingAvg_new( Temp_avg_1t , 7u);

#define NO_OF_FILTERS  (sizeof(Filters_list) / sizeof (MovingAvg_t *))
MovingAvg_t * Filters_list[] = 
{
   &Temp_avg_10s, 
   &Temp_avg_1min, 
   &Temp_avg_10min, 
   &Temp_avg_1h, 
   &Temp_avg_1d,
   &Temp_avg_1t
};




signed int Temp_decicelsius;
signed int Temp_decicelsiusAvg_64;

static unsigned char gSensorIDs[OW_ROMCODE_SIZE];
static unsigned char diff, i, subzero, cel, cel_frac_bits;




void Temp_init(void)
{
   #ifndef OW_PORT
		OW_set_bus(&PORTA,2);
   #endif
	//	DS18X20_find_sensor(&diff, &gSensorIDs[0]);
}


void Temp_handler(void)
{
      static Temp_ReadState_t state = TEMPERATURE_MEASURE_REQUEST;
      unsigned char counter = 0;

      switch (state) 
      {
   		case TEMPERATURE_MEASURE_REQUEST:
         {
            //		i = gSensorIDs[0]; // family-code for conversion-routine
            DS18X20_start_meas( DS18X20_POWER_EXTERN, NULL );
            counter = 0;
            state  = TEMPERATURE_WAITING_FOR_RESPONSE;
            break;
         }
      
         case TEMPERATURE_WAITING_FOR_RESPONSE:
         {
            counter++;
            if(counter > TEMP_GET_COUNTS_MS(DS18B20_TCONV_12BIT))
            {
               state  = TEMPERATURE_UPDATE;
            }
            break;      
         }
         
         case TEMPERATURE_UPDATE:
         {
            DS18X20_read_meas_single(0x28, &subzero, &cel, &cel_frac_bits);
		      Temp_decicelsius = DS18X20_temp_to_decicel(subzero, cel, cel_frac_bits);
            Temp_decicelsiusAvg = MovingAvg(Temp_decicelsius, 6u);
            state  = TEMPERATURE_MEASURE_REQUEST;
            break;
         }
      }
}


void Temp_Avg_handler(signed int sample)
{
   static unsigned char state;
   static unsigned char filterIndex = 0;
   static unsigned char iterator[NO_OF_FILTERS];
   static unsigned char level = 0;
   
   static MovingAvg_t * filter_ptr;
   signed int averageValue;
   signed int sample;
   static signed int StepDown_sample;

   if(0 == filterIndex)
   {
      sample = Temp_decicelsius;
   }
   else
   {
      sample = StepDown_sample;
   } 

   if(MovingAvg_NoOfAvgSamples(filter_ptr) >=  iterator[filterIndex])
   {

      filter_ptr = Filters_list[filterIndex];
      MovingAvg_SetSample(filter_ptr,  sample);
      iterator[filterIndex]++;
	        /* get back to lower level filter */
      if(filterIndex > 0 )
      {
      	 
         filterIndex--;
      }

   }
   else
   {
      iterator[filterIndex] =0;
      StepDown_sample = MovingAvg_GetAvg(filter_ptr);
      filterIndex++;
	  iterator[filterIndex] =0;
      filter_ptr = Filters_list[filterIndex];
      MovingAvg_SetSample(filter_ptr,  StepDown_sample);
   }
   
   if()
   filter_ptr = Filters_list[filterIndex];
   MovingAvg_SetSample(filter_ptr,  sample);
   averageValue = MovingAvg_GetAvg(filter_ptr);
   
   
   
   
#if 0  
   switch (state) 
   {

	case 0u:
    {
		
         break;
    }
   	case 1u:
      {
		  /* get back to lower level filter */
	      if(filterIndex > 0 )
	      {
	         filterIndex--;
	         filter_ptr = Filters_list[filterIndex];
	      }
	      
	      MovingAvg_SetSample(filter_ptr,  sample);
	      iterator[filterIndex]++;
		  if(MovingAvg_NoOfAvgSamples(filter_ptr) <  iterator[filterIndex])
		  {
			state = 2u;	
		  }

		  
          break;
      }

      case 2u:
      {	 
		  iterator[filterIndex] =0;
	      StepDown_sample = MovingAvg_GetAvg(filter_ptr);
	      filterIndex++;
	  	  state = 3u;
         break;
      }
      
      case 3u:
      {
	  	  filter_ptr = Filters_list[filterIndex];
	      MovingAvg_SetSample(filter_ptr,  StepDown_sample);
         break;
      }


   }
#endif   



}






signed char Temp_GetTemp(void)
{


}







void MovingAvg_init(MovingAvg_t * object_name, unsigned char samples_no)
{

}


void MovingAvg_SetSample(MovingAvg_t * object_name, signed int sample)
{
   unsigned char pointer;
   unsigned char samples;
   unsigned char zakres;
   signed int  avg_out;
   signed int samples_sum = 0;
		 
  object_name->write_index++;
  if(object_name->write_index > (object_name->samples_no ) ) {object_name->write_index = 0;};
   // object_name->write_pointer &=((1u << object_name->samples_no) -1u);
  object_name->buffer_ptr[object_name->write_index] = sample;

  
}


signed int MovingAvg_GetAvgSamples(MovingAvg_t * object_name, signed int samples_no)
{
   unsigned char index;
   unsigned char sample;
   //unsigned char samples_no;
   signed int  avg_out;
   signed int samples_sum = 0;

  index = object_name->write_index;
  sample = 0;
  //samples_no = (1u << object_name->samples_no);
  do
  {
    samples_sum += object_name->buffer_ptr[index--];    
  	if(index > (object_name->samples_no - 1u)) 
	{ 
		index = (object_name->samples_no - 1u);
	}
	sample++;
  }while(sample < samples_no);
  
 
  //avg_out = samples_sum >> object_name->samples_no; //Zaokraglenie w dol - rozwazyc zaokraglenie rownomierne 

  avg_out = samples_sum / samples_no; //Zaokraglenie w dol - rozwazyc zaokraglenie rownomierne 
  
  return(avg_out);
}


signed int MovingAvg_GetAvg(MovingAvg_t * object_name)
{
   unsigned char index;
   unsigned char sample;
   //unsigned char samples_no;
   signed int  avg_out;
   signed int samples_sum = 0;

  index = object_name->write_index;
  sample = 0;
  //samples_no = (1u << object_name->samples_no);
  do
  {
    samples_sum += object_name->buffer_ptr[index--];    
  	if(index > (object_name->samples_no - 1u)) 
	{ 
		index = (object_name->samples_no - 1u);
	}
	sample++;
  }while(sample < object_name->samples_no);
  
 
  //avg_out = samples_sum >> object_name->samples_no; //Zaokraglenie w dol - rozwazyc zaokraglenie rownomierne 

  avg_out = samples_sum / object_name->samples_no; //Zaokraglenie w dol - rozwazyc zaokraglenie rownomierne 
  
  return(avg_out);
}

signed char MovingAvg_NoOfAvgSamples(MovingAvg_t * object_name)
{   
  return(object_name->samples_no);
}



signed int EDR_array[15] = {100,100,100,100,100,100,100,100,0,0,0,0,0,100,100};
signed int EDR_avg = 0;
void test(void);

MovingAvg_new(aaa , 5);


void test(void)
{
	static unsigned char i = 0;
	
	MovingAvg_SetSample( &aaa , EDR_array[i++]);
	EDR_avg = MovingAvg_GetAvg(&aaa , 3u);

	if (i > 10u){i = 0;};
	
}






