            "magnitude": {
                "type": "*cond(magnitude)",
*if(strcmp(cond(Magnitude),"File")==0)
                "filename": "*cond(file)"
*elseif(strcmp(cond(Magnitude),"Band_limited")==0)
                "frequencyMaximum": *cond(Frequency_maximum),
                "frequencyMinimum": *cond(Frequency_minimum)
*elseif(strcmp(cond(Magnitude),"Gaussian")==0))
                "frequencyMaximum": *cond(Frequency_maximum)
*else
*WarningBox "Unrecognized magnitude type"
                "_error": "invalidMagnitude"
*endif
            },