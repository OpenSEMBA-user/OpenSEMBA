            "domain": {
*if(strcmp(cond(Time),"1")==0)
                "time": "*cond(Time)",
                "initialTime": *cond(Initial_time),
                "finalTime": *cond(Final_time),
*if(strcmp(cond(Frequency),"1")!=0)
                "samplingPeriod": *cond(Sampling_period)
*else
				"samplingPeriod": *cond(Sampling_period),
*endif
*endif
*if(strcmp(cond(Frequency),"1")==0)
                "frequency": *cond(Frequency),
                "initialFrequency": *cond(Initial_Frequency),
                "finalFrequency": *cond(Final_Frequency),
                "frequencyStep": *cond(Frequency_step),
                "logFrequencySweep": *cond(Log_frequency_sweep),
                "useTransferFunction": *cond(Use_transfer_function),
                "transferFunctionFile": "*cond(transfer_function_file)"
*endif
            }, 