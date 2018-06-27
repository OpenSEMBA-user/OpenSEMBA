            "domain": {
*if(strcmp(cond(Time),"1")==0)
                "initialTime": *cond(Initial_time),
                "finalTime": *cond(Final_time),
*if(strcmp(cond(Frequency),"0")==0)
                "samplingPeriod": *cond(Sampling_period)
*else
                "samplingPeriod": *cond(Sampling_period),
*endif
*endif
*if(strcmp(cond(Frequency),"1")==0)
                "initialFrequency": *cond(Initial_Frequency),
                "finalFrequency": *cond(Final_Frequency),
                "frequencyStep": *cond(Frequency_step),
*if(strcmp(cond(Use_transfer_function),"1")==0)
                "transferFunctionFile": "*cond(transfer_function_file)",
*endif
*if(strcmp(cond(Log_frequency_sweep),"1")==0)                
                "logFrequencySweep": true
*else
                "logFrequencySweep": false
*endif  
*endif
            }, 