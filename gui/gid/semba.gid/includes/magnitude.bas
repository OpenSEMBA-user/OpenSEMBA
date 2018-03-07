            "magnitude": {
                "type": "*cond(magnitude)",
*if(strcmp(cond(Magnitude),"File")==0)
                "filename": "*cond(file)"
*else
                "gaussianSpread": *cond(Gaussian_spread),
                "gaussianDelay": *cond(Gaussian_delay)
*endif
            },